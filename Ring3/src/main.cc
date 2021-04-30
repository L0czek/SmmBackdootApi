#include <iostream>

#include "cli.hpp"

int main(int argc, char* argv[]) {
    return Cli(argc, argv).handle();
}
