#include "graph.hpp"

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

    Graph load_from_osm_data(const osm::ParsedData & data) {
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
            // TODO: adjust distance
            weights[*it] = bgeo::distance(
                data.nodes[bgl::source(*it, graph)],
                data.nodes[bgl::target(*it, graph)]
            );
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

}
