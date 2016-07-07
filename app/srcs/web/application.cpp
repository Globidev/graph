#include "application.hpp"

namespace web {

    using Server = Handler::Server;

    Application::Application(const graph::Graph & graph,
                             const spatial::Index & index, ushort port):
        graph_ { graph }, index_ { index }, port_ { port }
    { }

    void Application::run() {
        Handler handler;
        Server::options options { handler };

        Server server {
            options.
            address("0.0.0.0").
            port(std::to_string(port_))
        };

        std::cout << "Listening on " << port_ << std::endl;

        server.run();
    }

    void Handler::operator()(const Server::request & request,
                             Server::connection_ptr connection) {
        std::ostringstream data;

        data << "Hello, World!";
        connection->write(data.str());
    }
}
