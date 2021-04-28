#include "api.hpp"

#include <iostream>

void BackdoorApi::smm_call(BackdoorApi::Params& params) {
    asm volatile (
        "                        \n\
            mov %6, %%rsi        \n\
            mov %7, %%rdi        \n\
            mov %8, %%rcx        \n\
            mov %9, %%rdx        \n\
            mov %10, %%r8        \n\
            mov %11, %%r9        \n\
                                 \n\
            xor %%rax, %%rax     \n\
        backdoor_loop_%=:        \n\
            test %%rax, %%rax    \n\
            jz backdoor_loop_%=  \n\
                                 \n\
            mov %%r9 , %5        \n\
            mov %%r8 , %4        \n\
            mov %%rdx, %3        \n\
            mov %%rcx, %2        \n\
            mov %%rdi, %1        \n\
            mov %%rsi, %0        \n\
        "
        :   "=g"(params.r[0]), "=g"(params.r[1]), "=g"(params.r[2]), \
            "=g"(params.r[3]), "=g"(params.r[4]), "=g"(params.r[5])
        :   "g"(params.r[0]), "g"(params.r[1]), "g"(params.r[2]),    \
            "g"(params.r[3]), "g"(params.r[4]), "g"(params.r[5])   
        :    "rsi", "rdi","rcx", "rdx", "r8", "r9"
    );
}

void BackdoorApi::hello_world_test() const {
    BackdoorApi::Params params = {
        static_cast<std::uint64_t>(BackdoorApi::Function::HelloWorldTest),
        0,
        0,
        0,
        BackdoorApi::BACKDOOR_MAGIC1,
        BackdoorApi::BACKDOOR_MAGIC2
    };

    smm_call(params);

    std::cout << "Backdoor returned:\n" << std::hex;

    for (std::size_t i=0; i < 6; ++i) {
        std::cout << "r[" << i << "] = " << params.r[i] << "\n";
    }
}
