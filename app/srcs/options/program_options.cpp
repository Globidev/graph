#include "program_options.hpp"

std::string ProgramOptions::input_file      { "" };
std::string ProgramOptions::output_file     { "" };
std::string ProgramOptions::protobuf_file   { "" };

ushort      ProgramOptions::web_server_port { 8080 };

static auto getUsage() {
    po::options_description usage { "Available options" };

    usage.add_options()
        ("help,h",      "Show this help message")

        ("input,i",     po::value(&ProgramOptions::input_file),
                        "Load the graph from the specified input file")
        ("output,o",    po::value(&ProgramOptions::output_file),
                        "Save the graph to the specified output file")
        ("generate,g",  po::value(&ProgramOptions::protobuf_file),
                        "Generate a graph from the specified protobuf file")

        ("port,p",      po::value(&ProgramOptions::web_server_port),
                        "Port for the web server to listen on")
    ;

    return usage;
}

void ProgramOptions::parse_from_command_line(int argc, char **argv) {
    po::variables_map options;
    po::command_line_parser parser { argc, argv };

    auto usage = getUsage();
    try {
        po::store(
            parser.options(usage)
                  .run(),
            options
        );
        if (options.count("help")) {
            std::cerr << usage << std::endl;
            exit(0);
        }
        po::notify(options);
    }
    catch (const po::error & exception) {
        std::cerr << exception.what() << std::endl << std::endl;
        std::cerr << usage << std::endl;
        exit(1);
    }
}
