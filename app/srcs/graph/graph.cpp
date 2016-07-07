#include "graph.hpp"

#include "spatial/constants.hpp"

namespace graph {

    Maybe<Graph> load_serialized(const std::string & file_name) {
        Graph graph;
        std::ifstream ifs { file_name };

        if (!ifs.is_open())
            return boost::none;

        try {
            archive::binary_iarchive ia { ifs };
            ia >> graph;
        }
        catch(const archive::archive_exception &) {
            return boost::none;
        }

        return graph;
    }

    bool save_serialized(const Graph & graph, const std::string & file_name) {
        std::ofstream ofs { file_name };

        if (!ofs.is_open())
            return false;

        try {
            archive::binary_oarchive oa { ofs };
            oa << graph;
        }
        catch(const archive::archive_exception &) {
            return false;
        }

        return true;
    }

    struct VertexVisitor: bgl::base_visitor<VertexVisitor> {
        using event_filter = bgl::on_discover_vertex;
        using Visited = std::set<uint>;

        VertexVisitor(Visited & visited):
            visited_ { visited }
        { }

        void operator()(auto s, auto &) {
            visited_.emplace(s);
        }

        Visited & visited_;
    };

    auto connected_nodes(const osm::ParsedData & data) {
        Graph graph {
            data.edges.cbegin(),
            data.edges.cend(),
            data.nodes.size()
        };

        VertexVisitor::Visited visited;
        VertexVisitor visitor { visited };

        auto start = bgl::vertex(0, graph); // TODO: let users pick a reference
                                            // vertex ?
        bgl::breadth_first_search(
            graph,
            start,
            bgl::visitor(make_bfs_visitor(visitor))
        );

        return visited;
    }

    static auto stripped_osm_data(const osm::ParsedData & data) {
        auto connected = connected_nodes(data);

        osm::ParsedData stripped;
        std::unordered_map<uint, uint> node_index_map;

        // Removing unconnected nodes and mapping their new indexes
        uint count = 0;
        for (uint i = 0; i < data.nodes.size(); ++i) {
            if (connected.count(i)) {
                stripped.nodes.emplace_back(std::move(data.nodes[i]));
                node_index_map.emplace(i, count++);
            }
        }

        // Removing unconnected edges and remapping node indexes
        for (const auto & edge: data.edges) {
            auto start_it = node_index_map.find(edge.first);
            auto end_it = node_index_map.find(edge.second);

            if (start_it != node_index_map.end()
                && end_it != node_index_map.end()) {

                stripped.edges.emplace_back(
                    start_it->second,
                    end_it->second
                );
            }
        }

        return stripped;
    }

    Graph load_from_osm_data(const osm::ParsedData & raw_data) {
        auto data = stripped_osm_data(raw_data);

        Graph graph {
            data.edges.cbegin(),
            data.edges.cend(),
            data.nodes.size()
        };

        // Add vertex properties (coordinates)
        auto coordinates = bgl::get(coordinates_t { }, graph);
        for (uint i = 0; i < data.nodes.size(); ++i)
            coordinates[i] = data.nodes[i];

        // Add edge properties (distances)
        auto weights = bgl::get(bgl::edge_weight, graph);
        Graph::edge_iterator it, end;
        boost::tie(it, end) = bgl::edges(graph);
        for (; it != end; ++it) {
            auto distance = bgeo::distance(
                data.nodes[bgl::source(*it, graph)],
                data.nodes[bgl::target(*it, graph)]
            );
            weights[*it] = distance * spatial::EARTH_RADIUS;
        }

        return graph;
    }

    struct RouteVisitor: bgl::base_visitor<RouteVisitor> {
        using event_filter = bgl::on_examine_vertex;

        struct DestinationReached { };

        RouteVisitor(Graph::vertex_descriptor destination):
            destination_ { destination }
        { }

        void operator()(auto vertex, auto &) {
            if (vertex == destination_)
                throw DestinationReached { };
        }

        Graph::vertex_descriptor destination_;
    };

    using Predecessors = std::vector<graph::Graph::vertex_descriptor>;

    static auto backtrack(const Predecessors & predecessors,
                          Graph::vertex_descriptor end, const Graph & graph) {
        Route::Steps steps;
        Graph::vertex_descriptor it, previous;

        auto coordinates = bgl::get(coordinates_t { }, graph);
        previous = end;
        do {
            it = previous;
            steps.emplace_back(coordinates[it]);
            previous = predecessors[it];
        } while (previous != it);

        std::reverse(steps.begin(), steps.end());

        return steps;
    }

    Maybe<Route> get_route(Graph::vertex_descriptor origin_v,
                           Graph::vertex_descriptor destination_v,
                           const Graph & graph) {
        using Distances = std::vector<double>;

        Distances vertex_distances(bgl::num_vertices(graph));
        Predecessors predecessors(bgl::num_vertices(graph));

        auto vertex_indexes = bgl::get(boost::vertex_index, graph);
        auto distance_map = boost::make_iterator_property_map(
            vertex_distances.begin(),
            vertex_indexes
        );
        auto predecessor_map = boost::make_iterator_property_map(
            predecessors.begin(),
            vertex_indexes
        );

        try {
            bgl::dijkstra_shortest_paths(
                graph,
                origin_v,
                bgl::predecessor_map(predecessor_map).
                distance_map(distance_map).
                visitor(
                    bgl::make_dijkstra_visitor(RouteVisitor { destination_v })
                )
            );
        }
        catch (const RouteVisitor::DestinationReached &) {
            return Route {
                vertex_distances[destination_v],
                backtrack(predecessors, destination_v, graph)
            };
        }

        return boost::none;
    }

}
