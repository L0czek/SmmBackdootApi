#ifndef __API_HPP__
#define __API_HPP__

#include <cstdint>
#include <utility>

class BackdoorApi {
public:
    BackdoorApi();

    void hello_world_test() const;
    std::size_t get_current_cpu() const;
    std::size_t change_priv(std::uint32_t uid, std::uint32_t gid) const;
    std::pair<std::size_t, std::size_t> dump_register(std::size_t id) const;

private:
    struct Params {
        std::uint64_t r[6];
    };

    static void smm_call(Params& params);
    static void switch_to_cpu(std::size_t cpu);

    enum class Function : std::size_t {
        GetCurrentCpu = 0x02,
        ChangePriv = 0x03,
        DumpReg = 0x04,
        HelloWorldTest = 0x10
    };

    static constexpr const std::size_t BACKDOOR_MAGIC1 = 0x4141414141414141;
    static constexpr const std::size_t BACKDOOR_MAGIC2 = 0x4242424242424242;
};

#endif
