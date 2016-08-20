#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define M_AUTHOR "Willian Briotto"
#define M_DESC "Some desc"

int int_param = 0;
int array_param[3] = {0,0,0};
module_param(int_param, int, S_IRUSR | S_IWUSR);
module_param_array(array_param, int, NULL, S_IRUSR | S_IWUSR);

static int __init hello_init(void) {
    printk(KERN_ALERT "test: params");
    
    printk(KERN_ALERT "test: param int is %d", int_param);
    printk(KERN_ALERT "test: param array int[0] %d", array_param[0]);
    printk(KERN_ALERT "test: param array int[1] %d", array_param[1]);
    printk(KERN_ALERT "test: param array int[2] %d", array_param[2]);
    
    return 0;
}

static void __exit hello_exit(void) {
    printk(KERN_ALERT "test: exit params");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR(M_AUTHOR);
MODULE_DESCRIPTION(M_DESC);

