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

static void save_graph_and_exit(const graph::Graph & graph) {
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

static auto build_spatial_index(graph::Graph & graph) {
    std::vector<spatial::Index::value_type> coordinates;
    graph::Graph::vertex_iterator it, end;

    auto coordinates_map = bgl::get(graph::coordinates_t { }, graph);
    boost::tie(it, end) = bgl::vertices(graph);
    coordinates.reserve(bgl::num_vertices(graph));

    std::transform(
        it,
        end,
        std::back_inserter(coordinates),
        [&](auto v) { return std::make_pair(coordinates_map[v], v); }
    );

    // Using boost packing algorithm to bulk load the rtree
    return spatial::Index {
        coordinates
    };
}

#include "tools/benchmark.hpp"

void Application::run() {
    auto graph = get_graph();

    std::cout << "Using " << bgl::num_vertices(graph) << " vertices"
              << " and " << bgl::num_edges(graph) << " edges" << std::endl;

    if(!ProgramOptions::output_file.empty())
        save_graph_and_exit(graph);

    std::cout << "Building a spatial index..." << std::endl;

    auto spatial_index = build_spatial_index(graph);

    std::cout << "Dijkstra..." << std::endl;

    Benchmark b { "Dijkstra" };
    auto route = get_route(
        { 2.3653761, 48.8663557 },
        { 2.305031, 48.870813 },
        spatial_index,
        graph
    );
}
