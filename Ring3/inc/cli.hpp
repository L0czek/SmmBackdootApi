#ifndef __CLI_HPP__
#define __CLI_HPP__

#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <unordered_map>

#include "api.hpp"

namespace po = boost::program_options;

class Cli {
public:
    Cli(int argc, char* argv[]);

    int handle();
private:
    BackdoorApi api;

    int argc;
    char** argv;

    static po::options_description global_opt_desc();
    static po::positional_options_description global_pos_desc();
    static po::options_description change_priv_opt_desc();
    static po::options_description dump_reg_opt_desc();
    static void print_help();

    po::variables_map parse_global_options() const;

    int handle_hello_world_test_subcommand() const;
    int handle_get_current_cpu_subcommand() const;
    int handle_change_priv_subcommand() const;
    int handle_dump_reg_subcommand() const;

    std::unordered_map<std::string, std::function<int()>> subcommands;
};

#endif
