#ifndef __S3EXPLOIT_IOCTL__
#define __S3EXPLOIT_IOCTL__

#include <linux/ioctl.h>
#include <linux/types.h>

#define IOCTL_BASE 'E'
#define SMM_BACKDOOR_CALL       _IOW(IOCTL_BASE, 0x01, struct BackdoorParams*)

#define API_BACKDOOR_CALL 137

#define BACKDOOR_HELLO_WORLD_TEST 0x10

#define BACKDOOR_MAGIC1 0x4141414141414141
#define BACKDOOR_MAGIC2 0x4242424242424242

struct BackdoorParams {
    uint64_t r[6];
};

#endif
