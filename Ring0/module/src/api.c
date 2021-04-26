#include "linux/device/class.h"
#include "linux/mutex.h"
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include "ioctl.h"


MODULE_LICENSE("GPL");

struct module_struct_t {
    dev_t dev;
    struct cdev *cdev;
    struct class *class;
    struct device *device;
    struct file_operations fops;
};

DEFINE_MUTEX(dev_lock);

static int device_open(struct inode *inode, struct file *filp);
static int device_close(struct inode *inode, struct file *filp);
static long device_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

static struct module_struct_t module_struct = {
    .fops = {
        .owner = THIS_MODULE,
        .open = &device_open,
        .release = &device_close,
        .unlocked_ioctl = &device_ioctl
    }
};

static const char *DEVICE_NAME = "backdoor_api";
static const char *DEVICE_CLASS = "bootkit";

int __init init_module(void) {
    int status;

    mutex_init(&dev_lock);

    status = alloc_chrdev_region(&module_struct.dev, 0, 1, DEVICE_NAME);
    if (status < 0)
        goto error_free_backdoor_api;
    
    module_struct.cdev = cdev_alloc();
    if (!module_struct.cdev) {
        status = -ENOMEM;
        goto error_free_chrdev_region;
    }
    module_struct.cdev->ops = &module_struct.fops;

    module_struct.class = class_create(THIS_MODULE, DEVICE_CLASS);
    if (IS_ERR(module_struct.class)) {
        status = PTR_ERR(module_struct.class);
        goto error_free_cdev;
    }

    module_struct.device = device_create(module_struct.class, NULL, 
            module_struct.dev, NULL, DEVICE_NAME);
    if (IS_ERR(module_struct.device)) {
        status = PTR_ERR(module_struct.device);
        goto error_free_class;
    }

    status = cdev_add(module_struct.cdev, module_struct.dev, 1);
    if (status < 0)
        goto error_free_device;

    printk(KERN_INFO "backdoor_api: Module loaded\n");
    return 0;

error_free_device:
    device_destroy(module_struct.class, module_struct.dev);
    class_unregister(module_struct.class);

error_free_class:
    class_destroy(module_struct.class);

error_free_cdev:
    cdev_del(module_struct.cdev);

error_free_chrdev_region:
    unregister_chrdev_region(module_struct.dev, 1);

error_free_backdoor_api:

    mutex_destroy(&dev_lock);

    return status;
}

void __exit cleanup_module(void) {
    device_destroy(module_struct.class, module_struct.dev);
    class_unregister(module_struct.class);
    class_destroy(module_struct.class);
    cdev_del(module_struct.cdev);
    unregister_chrdev_region(module_struct.dev, 1);
    mutex_destroy(&dev_lock);
    printk(KERN_INFO "backdoor_api: Module unloaded\n");
}

static int device_open(struct inode *inode, struct file *filp) {
    if (mutex_trylock(&dev_lock) == 0) 
        return -EBUSY;

    return 0;
}

static int device_close(struct inode *inode, struct file *filp) {
    mutex_unlock(&dev_lock);
    return 0;
}

static long device_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {

    switch (cmd) {

    }

    return 0;
}

