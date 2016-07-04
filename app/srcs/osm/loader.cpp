#include "loader.hpp"

#include "constants.hpp"

namespace osm {

    struct Visitor: public osmium::handler::Handler {

        Visitor(ParsedData & data):
            data_ { data }
        { }

        void node(osmium::Node & node) {
            data_.nodes.emplace_back(
                node.location().lat(),
                node.location().lon()
            );
            nodes_by_id.emplace(
                node.id(),
                data_.nodes.size() - 1
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
                std::back_inserter(data_.edges),
                [this](auto n2, auto n1) {
                    return std::make_pair(
                        nodes_by_id[n1.ref()],
                        nodes_by_id[n2.ref()]
                    );
                }
            );
        }

    private:
        using NodesById = std::unordered_map<NodeId, uint>;

        ParsedData & data_;
        NodesById nodes_by_id;
    };

    ParsedData load_protobuf(const std::string & file_name) {
        ParsedData data;
        Visitor v { data };

        osmium::io::File ifs { file_name };
        osmium::io::Reader reader { ifs };

        osmium::apply(reader, v);
        reader.close();

        return data;
    }

}
