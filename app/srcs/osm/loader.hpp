#pragma once

#include "types.hpp"

namespace osm {

    struct ParsedData {
        Nodes nodes;
        Edges edges;
    };

    ParsedData load_protobuf(const std::string & file_name);

}
