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
        return { };
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
