#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h> //file operations
#include <linux/cdev.h> // char driver; makes cdev available
#include <linux/semaphore.h> // synchronization behaviors
#include <linux/uaccess.h> // copy_to_user;copy_from_user

#define DRIVER_NAME "SOMEDRIVERNAME"
#define M_AUTHOR "Willian Briotto"
#define M_DESC "Some desc"

struct fake_device {
    char data[100];
    struct semaphore sem;
} virtual_device;

struct cdev *mcdev;
int major_number; //will store major number extracted form dev_t
int ret; //will be used to hold return values of function;
dev_t dev_num; //will hold major number that kernel gives us

int device_open(struct inode *inode, struct file* filp) {
    //only allow one proccess to open this device by using a semaphore as mutual exclusive lock-mutex
    if(down_interruptible(&virtual_device.sem) != 0) {
        printk(KERN_ALERT "driver: cloud not lock device during open");
        return -1;
    }

    printk(KERN_INFO "driver: opened device");
    return 0;
}

ssize_t device_read(struct file* filp, char* bufStoreData, size_t bufCount, loff_t* curOffset) {
    printk(KERN_INFO "Reading from device");
    ret = copy_to_user(bufStoreData, virtual_device.data, bufCount);
    return ret;
}

ssize_t device_write(struct file* filp, const char* bufSourceData, size_t bufCount, loff_t* curOffset) {
    printk(KERN_INFO "Writing to device");
    ret = copy_from_user(virtual_device.data, bufSourceData, bufCount);
    return ret;
}

int device_close(struct inode *inode, struct file *filp) {
    up(&virtual_device.sem);
    printk(KERN_INFO "Close device");
    return 0;
}

//Tell the kernel wich functions to call when user operates on our device file
struct file_operations fops = {
    .owner = THIS_MODULE, //prevent unloadind of this module when operations are in use
    .open = device_open, // points to the method to call when opening the device
    .release = device_close, 
    .write = device_write, 
    .read = device_read
};

static int __init driver_entry(void) {
    //major number -- alloc_chrdev_region(dev_t*, uint fminor, uint count, char* name)
    ret = alloc_chrdev_region(&dev_num, 0, 1, DRIVER_NAME);
    if(ret < 0) {
        printk(KERN_ALERT "driver: failed to allocate a major number");
        return ret;
    }

    major_number = MAJOR(dev_num);
    printk(KERN_INFO "driver: major number is %d", major_number);
    printk(KERN_INFO "driver: use mknod /dev/%s c %d for device file", DRIVER_NAME, major_number);

    mcdev = cdev_alloc();
    mcdev->ops = &fops;
    mcdev->owner = THIS_MODULE;

    //cdev_add(struct cdev* dev, dev_t num, unsigned int count)
    ret = cdev_add(mcdev, dev_num, 1);
    if(ret < 0) {
        printk(KERN_ALERT "driver: unable to add cdev to kernel");
        return ret;
    }
    sema_init(&virtual_device.sem, 1);

    return 0;
}

static void __exit driver_exit(void) {
    cdev_del(mcdev);
    
    unregister_chrdev_region(dev_num, 1);
    printk(KERN_ALERT "driver: unloaded module");
}

module_init(driver_entry);
module_exit(driver_exit);


MODULE_LICENSE("GPL");

MODULE_AUTHOR(M_AUTHOR);
MODULE_DESCRIPTION(M_DESC);
