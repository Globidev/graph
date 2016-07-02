#pragma once

struct ProgramOptions {

    static std::string input_file;
    static std::string output_file;
    static std::string protobuf_file;

    static ushort web_server_port;

    static void parse_from_command_line(int argc, char **argv);
};
