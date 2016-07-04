#pragma once

namespace osm {

    using NodeId = osmium::object_id_type;

    using Nodes = std::vector<std::pair<double, double>>;
    using Edges = std::vector<std::pair<uint, uint>>;

}
