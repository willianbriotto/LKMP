#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define M_AUTHOR "Willian Briotto"
#define M_DESC "Some desc"

static int __init hello_init(void) {
    printk(KERN_ALERT "test: hello world");
    return 0;
}

static void __exit hello_exit(void) {
    printk(KERN_ALERT "test: exit hello world");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR(M_AUTHOR);
MODULE_DESCRIPTION(M_DESC);

