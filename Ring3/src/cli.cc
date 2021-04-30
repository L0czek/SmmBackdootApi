#include "cli.hpp"
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <stdexcept>

Cli::Cli(int argc, char* argv[]) : argc(argc), argv(argv) {
    subcommands.insert(std::make_pair(
        "hello_world_test", 
        std::bind(&Cli::handle_hello_world_test_subcommand, this)
    ));    

    subcommands.insert(std::make_pair(
        "get_current_cpu",
        std::bind(&Cli::handle_get_current_cpu_subcommand, this)
    ));

    subcommands.insert(std::make_pair(
        "change_priv",
        std::bind(&Cli::handle_change_priv_subcommand, this)
    ));

    subcommands.insert(std::make_pair(
        "dump_reg",
        std::bind(&Cli::handle_dump_reg_subcommand, this)
    ));
}

int Cli::handle() {
    const auto global_map = parse_global_options();
    
    try {
        if (global_map.count("help")) {
            Cli::print_help();
            return 0;
        } else {
            const auto subcommand = global_map.at("subcommand").as<std::string>();
            return std::invoke(subcommands.at(subcommand));
        }
    } catch (std::out_of_range& e) {
        Cli::print_help();
        return -1;
    }
}

void Cli::print_help() {
    std::cerr << 
R"(Usage ./cli [subcommand] [subcommand's args...]
Subcommnds:
    hello_world_test
    get_current_cpu
    change_priv
    dump_reg

Subcommands specyfic options:

)";
    std::cerr << Cli::change_priv_opt_desc() << "\n";
    std::cerr << Cli::dump_reg_opt_desc() << "\n";
}
    
po::options_description Cli::global_opt_desc() {
    po::options_description global("Generic options");

    global.add_options()
        ("help", "Produces help message")
        ("subcommand", po::value<std::string>(), "API subcommand")
        ("subargs", po::value<std::vector<std::string>>(), "Args for subcommands");

    return global;
}

po::positional_options_description Cli::global_pos_desc() {
    po::positional_options_description pos;
    pos.add("subcommand", 1).add("subargs", -1);
    return pos;
}

po::variables_map Cli::parse_global_options() const {
    const auto global = Cli::global_opt_desc();
    const auto pos = Cli::global_pos_desc();

    po::parsed_options parsed = po::command_line_parser(argc, argv)
        .options(global)
        .positional(pos)
        .allow_unregistered()
        .run();

    po::variables_map map;
    po::store(parsed, map);

    return map;
}

int Cli::handle_hello_world_test_subcommand() const {
    api.hello_world_test();
    return 0;
}

int Cli::handle_get_current_cpu_subcommand() const {
    std::cout << "Current SMM CPU: " << api.get_current_cpu() << "\n";
    return 0;
}

po::options_description Cli::change_priv_opt_desc() {
    po::options_description opts("change_priv");
    opts.add_options()
        ("uid,u", po::value<std::size_t>()->default_value(0), "User id to switch to.")
        ("gid,g", po::value<std::size_t>()->default_value(0), "Group id to switch to.")
        ("shell,s", po::value<std::string>()->default_value("bash"), "Shell to spawn.");
    return opts; 
}

int Cli::handle_change_priv_subcommand() const {
    const auto opts = Cli::change_priv_opt_desc();

    po::variables_map map;
    po::store(po::command_line_parser(argc, argv).options(opts).run(), map);

    const auto uid = map.at("uid").as<std::size_t>();
    const auto gid = map.at("gid").as<std::size_t>();
    const auto shell = map.at("shell").as<std::string>();

    const auto error_code = api.change_priv(uid, gid);

    if (error_code == 0) {
        std::cout << "Operation completed successfully. Spawning shell...\n";
        system(shell.c_str());
    } else {
        std::cerr << "Operation failed with error code " << std::hex << error_code << "\n";
    }

    return error_code;
}

po::options_description Cli::dump_reg_opt_desc() {
    po::options_description opts("dump_reg");
    opts.add_options()
        ("reg_id", po::value<std::size_t>()->required(), "Register id to fetch.");
    return opts;
}

int Cli::handle_dump_reg_subcommand() const {
    const auto opts = Cli::dump_reg_opt_desc();

    po::variables_map map;
    po::store(po::command_line_parser(argc, argv).options(opts).run(), map);

    auto resp = api.dump_register(map.at("reg_id").as<std::size_t>());
    
    std::cout << "R (" << std::hex << resp.first << ", " << resp.second << ");\n";
    return 0;
}
