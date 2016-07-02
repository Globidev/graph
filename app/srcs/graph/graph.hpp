#pragma once

#include "osm/loader.hpp"

namespace graph {

    using VertexProperties = boost::no_property; // TODO: store coordinates ?
    using EdgeProperties = boost::no_property; // TODO: store distances ?

    using Graph = bgl::adjacency_list<
        bgl::listS,
        bgl::vecS,
        bgl::directedS,
        VertexProperties,
        EdgeProperties
    >;

    Maybe<Graph> load_serialized(const std::string & file_name);
    Graph load_from_osm_data(const osm::ParsedData & data);

    bool save_serialized(const Graph & graph, const std::string & file_name);
}
