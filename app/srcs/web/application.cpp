// Silicon
#include <silicon/api.hh>
#include <silicon/backends/mhd.hh>

#include "application.hpp"

#include "symbols.hh"

namespace web {

    Application::Application(const graph::Graph & graph,
                             const spatial::Index & index, ushort port):
        graph_ { graph }, index_ { index }, port_ { port }
    { }

    void Application::run() {
        using namespace sl; // Silicon namespace
        using namespace s; // Symbols namespace

        auto route_api = http_api(

            GET / _route
                * get_parameters(
                    _olat = double { },
                    _olng = double { },
                    _dlat = double { },
                    _dlng = double { }
                )
                = [](auto p) {
                    return D(
                        _olat = p.olat,
                        _olng = p.olng,
                        _dlat = p.dlat,
                        _dlng = p.dlng
                    );
                }

        );

        std::cout << "Listening on " << port_ << std::endl;

        sl::mhd_json_serve(route_api, port_);
    }

}
