#ifndef __API_HPP__
#define __API_HPP__

class SmmBackdoorApi {
    int fd;
public:
    SmmBackdoorApi();
    SmmBackdoorApi(const SmmBackdoorApi& ) = delete;
    ~SmmBackdoorApi();

    void hello_world_test();
};

#endif
