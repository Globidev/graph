#include "graph.hpp"

namespace graph {

    Graph load_serialized(const std::string & file_name) {
        Graph graph;
        std::ifstream ifs { file_name };

        archive::binary_iarchive ia { ifs };
        ia >> graph;

        return graph;
    }

    Graph load_from_osm_data(const osm::ParsedData & data) {
        return { };
    }

    bool save_serialized(const Graph & graph, const std::string & file_name) {
        std::ofstream ofs { file_name };

        if (!ofs.is_open())
            return false;

        archive::binary_oarchive oa { ofs };
        oa << graph;

        return true;
    }

}
