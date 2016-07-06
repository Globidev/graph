#include "loader.hpp"

#include "constants.hpp"

namespace osm {

    struct Visitor: public osmium::handler::Handler {

        Visitor(ParsedData & data):
            data_ { data }
        { }

        ~Visitor() {
            uint count = 0;
            std::unordered_map<NodeId, uint> node_id_map;

            // Mapping node ids to a sequence
            for (const auto & edge: raw_edges) {
                if (!node_id_map.count(edge.first))
                    node_id_map.emplace(edge.first, count++);
                if (!node_id_map.count(edge.second))
                    node_id_map.emplace(edge.second, count++);
            }

            // Building the edge vector
            for (const auto & edge: raw_edges)
                data_.edges.emplace_back(
                    node_id_map[edge.first],
                    node_id_map[edge.second]
                );

            // // Building the node vector
            data_.nodes.resize(count);
            for (const auto & pair: node_id_map)
                data_.nodes[pair.second] = nodes_by_id[pair.first];
        }

        void node(osmium::Node & node) {
            nodes_by_id.emplace(
                std::piecewise_construct,
                std::forward_as_tuple(node.id()),
                std::forward_as_tuple(
                    node.location().lon(),
                    node.location().lat()
                )
            );
        }

        void way(osmium::Way & way) {
            auto & tags = way.tags();
            auto road_type = tags["highway"];

            if (!road_type || !ROAD_FILTERS.count(road_type))
                return ;

            auto & nodes = way.nodes();

            std::transform(
                std::next(nodes.begin()),
                nodes.end(),
                nodes.begin(),
                std::back_inserter(raw_edges),
                [](auto & n2, auto & n1) {
                    return std::make_pair(n1.ref(), n2.ref());
                }
            );

            auto one_way = tags["oneway"];
            if(!one_way || strcmp(one_way, "yes") != 0) {
                std::transform(
                    std::next(nodes.begin()),
                    nodes.end(),
                    nodes.begin(),
                    std::back_inserter(raw_edges),
                    [](auto & n2, auto & n1) {
                        return std::make_pair(n2.ref(), n1.ref());
                    }
                );
            }
        }

    private:
        using NodesById = std::unordered_map<NodeId, spatial::Coordinates>;

        std::vector<std::pair<NodeId, NodeId>> raw_edges;
        NodesById nodes_by_id;

        ParsedData & data_;
    };

    ParsedData load_protobuf(const std::string & file_name) {
        ParsedData data;

        osmium::io::File ifs { file_name };
        osmium::io::Reader reader { ifs };

        {
            Visitor v { data };
            osmium::apply(reader, v);
        }

        reader.close();

        return data;
    }

}
