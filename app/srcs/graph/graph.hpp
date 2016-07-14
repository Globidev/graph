#pragma once

#include "osm/loader.hpp"

namespace graph {

    struct coordinates_t {
        using kind = bgl::vertex_property_tag;
    };

    using VertexProperties = boost::property<
        coordinates_t,
        spatial::Coordinates
    >;
    using EdgeProperties = boost::property<
        bgl::edge_weight_t,
        double
    >;

    using Graph = bgl::adjacency_list<
        bgl::listS,
        bgl::vecS,
        bgl::directedS,
        VertexProperties,
        EdgeProperties
    >;

    bool save_serialized(const Graph & graph, const std::string & file_name);
    Maybe<Graph> load_serialized(const std::string & file_name);
    Graph load_from_osm_data(const osm::ParsedData & data);

    struct Route {
        using Steps = std::vector<spatial::Coordinates>;

        double distance;
        Steps steps;
    };

    Maybe<Route> get_route(Graph::vertex_descriptor origin_v,
                           Graph::vertex_descriptor destination_v,
                           const Graph &);

    Maybe<Route> get_route_a_star(Graph::vertex_descriptor origin_v,
                                  Graph::vertex_descriptor destination_v,
                                  const Graph &);
}
