#pragma once

namespace osm {

    struct ParsedData {
        // TODO
        using NodeMap = std::unordered_map<int, int>;
        using Edges = std::vector<std::pair<int, int>>;

        NodeMap nodes_by_id;
        Edges edges;
    };

    ParsedData load_protobuf(const std::string & file_name);

}
