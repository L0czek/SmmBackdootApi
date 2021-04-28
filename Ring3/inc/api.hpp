#ifndef __API_HPP__
#define __API_HPP__

#include <cstdint>

class BackdoorApi {
public:
    void hello_world_test() const;

private:
    struct Params {
        std::uint64_t r[6];
    };

    static void smm_call(Params& params);

    enum class Function : std::size_t {
        HelloWorldTest = 0x10
    };

    static constexpr const std::size_t BACKDOOR_MAGIC1 = 0x4141414141414141;
    static constexpr const std::size_t BACKDOOR_MAGIC2 = 0x4242424242424242;
};

#endif
