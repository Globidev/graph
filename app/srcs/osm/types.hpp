#pragma once

#include "spatial/types.hpp"

namespace osm {

    using NodeId = osmium::object_id_type;

    using Nodes = std::vector<spatial::Coordinates>;
    using Edges = std::vector<std::pair<uint, uint>>;

}
