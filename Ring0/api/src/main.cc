#include <iostream>
#include "api.hpp"
#include <memory>

int main() {
    auto api = std::make_unique<SmmBackdoorApi>();
    api->hello_world_test();
}
