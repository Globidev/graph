#include "application.hpp"

#include "options/program_options.hpp"

#include "graph/graph.hpp"

Application::Application(int argc, char **argv) {
    ProgramOptions::parse_from_command_line(argc, argv);
}

static auto get_graph() {
    // Load serialized graph
    if (!ProgramOptions::input_file.empty()) {
        std::cout << "Loading serialized graph..." << std::endl;

        auto mb_graph = graph::load_serialized(ProgramOptions::input_file);
        if (mb_graph)
            return *mb_graph;
        else {
            std::cerr << "Unable to load the graph from "
                      << ProgramOptions::input_file
                      << std::endl;
            exit(1);
        }
    }
    // Generate the graph
    else if (!ProgramOptions::protobuf_file.empty()) {
        std::cout << "Loading protobuf data..." << std::endl;
        auto osm_data = osm::load_protobuf(ProgramOptions::protobuf_file);

        std::cout << "Building graph..." << std::endl;
        return graph::load_from_osm_data(osm_data);
    }
    // Error
    else {
        std::cerr << "No input graph specified" << std::endl;
        exit(1);
    }
}

void Application::run() {
    auto graph = get_graph();

    std::cout << "Using " << bgl::num_vertices(graph) << " vertices"
              << " and " << bgl::num_edges(graph) << " edges" << std::endl;

    // Save the graph and exit
    if(!ProgramOptions::output_file.empty()) {
        std::cout << "Saving graph to " << ProgramOptions::output_file
                  << "..." << std::endl;
        if (graph::save_serialized(graph, ProgramOptions::output_file)) {
            std::cout << "Graph successfully saved to "
                      << ProgramOptions::output_file
                      << std::endl;
            exit(0);
        }
        else {
            std::cerr << "Unable to save the graph to "
                      << ProgramOptions::output_file
                      << std::endl;
            exit(1);
        }
    }
}
