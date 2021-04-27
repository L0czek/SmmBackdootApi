#include "api.hpp"
#include <stdexcept>
#include <iostream>

#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "ioctl.h"

SmmBackdoorApi::SmmBackdoorApi() {
    fd = ::open("/dev/backdoor_api", O_RDWR);
    if (fd < 0)
        throw std::runtime_error("Cannot open /dev/backdoor_api");
}

SmmBackdoorApi::~SmmBackdoorApi() {
    ::close(fd);
}

void SmmBackdoorApi::hello_world_test() {
    BackdoorParams params;
    
    params.r[0] = BACKDOOR_HELLO_WORLD_TEST; 
    params.r[1] = 0;
    params.r[2] = 0;
    params.r[3] = 0;
    params.r[4] = BACKDOOR_MAGIC1;
    params.r[5] = BACKDOOR_MAGIC2;

    int status = ioctl(fd, SMM_BACKDOOR_CALL, &params);
    if (status < 0)
        throw std::runtime_error("Error in kernel module");

    std::cout << "Backdoor returned:\n" << std::hex;

    for (std::size_t i=0; i < 6; ++i) {
        std::cout << "r[" << i << "] = " << params.r[i] << "\n";
    }
}
