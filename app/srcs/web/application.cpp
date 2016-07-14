// Silicon
#include <silicon/api.hh>
#include <silicon/backends/mhd.hh>

#include "application.hpp"
#include "tools/benchmark.hpp"

#include "symbols.hh"

static auto nearest(const spatial::Coordinates & coords,
                    const spatial::Index & index) {
    spatial::Index::value_type nearest;

    bgi::query(
        index,
        bgi::nearest(coords, 1),
        boost::make_function_output_iterator([&](const auto & node) {
            nearest = node;
        })
    );

    return nearest;
}

static auto coordinate_object(const spatial::Coordinates & coords) {
    using namespace s;

    return D(
        _latitude = bgeo::get<1>(coords),
        _longitude = bgeo::get<0>(coords)
    );
}

static auto route_object(const graph::Route & route) {
    using namespace s;

    std::vector<decltype(coordinate_object({}))> coords;
    std::transform(
        route.steps.begin(),
        route.steps.end(),
        std::back_inserter(coords),
        [](auto & c) { return coordinate_object(c); }
    );

    return D(
        _distance = route.distance,
        _steps = coords
    );
}

static auto get_api(const graph::Graph & graph, const spatial::Index & index) {
    using namespace s;
    using namespace sl;

    auto home_r = (
        GET = [] {
            std::ifstream ifs { "html/index.html" };
            std::stringstream buffer;
            buffer << ifs.rdbuf();
            return buffer.str();
        }
    );

    auto bounds_r = (
        GET / _bounds = [&] {
            auto bounds = index.bounds();

            return D(
                _minCorner = coordinate_object(bounds.min_corner()),
                _maxCorner = coordinate_object(bounds.max_corner())
            );
        }
    );

    auto route_params = get_parameters(
        _olat = double { }, _olng = double { },
        _dlat = double { }, _dlng = double { }
    );
    auto route_r = (
        GET / _route * route_params = [&](auto p) {
            std::ostringstream oss;
            oss << "route from [" << p.olat << ", " << p.olng << "]"
                       << " to [" << p.dlat << ", " << p.dlng << "]";
            Benchmark b { oss.str() };

            auto origin = nearest({ p.olng, p.olat }, index);
            auto destination = nearest({ p.dlng, p.dlat }, index);

            auto route = graph::get_route(origin.id, destination.id, graph);

            return D(
                _origin = coordinate_object(origin.coordinates),
                _destination = coordinate_object(destination.coordinates),
                _route = route_object(route ? *route : graph::Route { })
            );
        }
    );

    return http_api(
        home_r,
        route_r,
        bounds_r
    );
}

namespace web {

    Application::Application(const graph::Graph & graph,
                             const spatial::Index & index, ushort port):
        graph_ { graph }, index_ { index }, port_ { port }
    { }

    void Application::run() {
        auto api = get_api(graph_, index_);

        std::cout << "Listening on " << port_ << std::endl;

        sl::mhd_json_serve(api, port_);
    }

}
