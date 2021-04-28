extern "C" {

#include <efi.h>
#include <efilib.h>
}

#include <functional>
#include <cstdint>

EFI_STATUS cxx_main(EFI_HANDLE, EFI_SYSTEM_TABLE *);

extern "C" {

EFI_STATUS
EFIAPI
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);
    SystemTable->BootServices->SetWatchdogTimer(
        0, 0, 0, nullptr); // Disable UEFI's timer which after ~5min would reset the computer.
    return cxx_main(ImageHandle, SystemTable);
}
}

#define API_BACKDOOR_CALL 137

#define BACKDOOR_HELLO_WORLD_TEST 0x10

#define BACKDOOR_MAGIC1 0x4141414141414141
#define BACKDOOR_MAGIC2 0x4242424242424242

struct BackdoorParams {
    UINT64 r[6];
};

void smm_call(BackdoorParams& params) {
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

static void smm_backdoor_call(struct BackdoorParams* params) {

    const uint16_t SMM_PORT = 0xB2;
    const uint8_t SMI_BACKDOOR_SMI = API_BACKDOOR_CALL;

    asm volatile (
        "                        \n\
            mov %6, %%rsi        \n\
            mov %7, %%rdi        \n\
            mov %8, %%rcx        \n\
            mov %9, %%rdx        \n\
            mov %10, %%r8        \n\
            mov %11, %%r9        \n\
            outb %12, %13        \n\
            mov %%r9 , %5        \n\
            mov %%r8 , %4        \n\
            mov %%rdx, %3        \n\
            mov %%rcx, %2        \n\
            mov %%rdi, %1        \n\
            mov %%rsi, %0        \n\
        "
        :   "=g"(params->r[0]), "=g"(params->r[1]), "=g"(params->r[2]), \
            "=g"(params->r[3]), "=g"(params->r[4]), "=g"(params->r[5])
        :   "g"(params->r[0]), "g"(params->r[1]), "g"(params->r[2]),    \
            "g"(params->r[3]), "g"(params->r[4]), "g"(params->r[5]),    \
            "a"(SMI_BACKDOOR_SMI), "Nd"(SMM_PORT)
        :   "rsi", "rdi", "rcx", "rdx", "r8", "r9"
    );
}

template <typename RetVal, typename... Args> auto uefi(RetVal (*ptr)(Args...), Args... args)
{
    typedef RetVal (*FuncPtr)(Args...);
    typedef __attribute__((ms_abi)) FuncPtr MSFuncPtr;
    MSFuncPtr mptr = reinterpret_cast<MSFuncPtr>(ptr);
    return std::invoke(mptr, args...);
}

EFI_STATUS cxx_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    BackdoorParams params = {
        BACKDOOR_HELLO_WORLD_TEST,
        0,
        0,
        0,
        BACKDOOR_MAGIC1,
        BACKDOOR_MAGIC2
    };

    smm_call(params);

    Print((CHAR16*)
        L"Backdoor returned:\n"
    );

    for (std::uint32_t i=0; i < 6; ++i) {
        Print((CHAR16*)L"r[%d] = 0x%llX\n", i, params.r[i]);
    }

    for(;;);
     
    return EFI_SUCCESS;
}
