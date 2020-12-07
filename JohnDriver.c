/* JohnDriver.c */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/reboot.h>
#include <linux/string.h>

#define John_MAJOR 60
#define John_NAME "JohnDevice"
MODULE_LICENSE("Dual BSD/GPL");


static int JohnDriver_open(struct inode *inode, struct file *filp) {
    printk("<1>JohnDriver: open\n");
    return 0;
}

static int JohnDriver_close(struct inode *inode, struct file *filp) {
    printk("<1>JohnDriver: close\n");
    return 0;
}

static ssize_t JohnDriver_read(struct file *filp, char *buf, size_t size, loff_t *f_pos) {
    printk("<1>JohnDriver: read  (size=%zu)\n", size);
    return 0;
}

static ssize_t JohnDriver_write(struct file *filp, const char *buf, size_t size, loff_t *f_pos) {
    char *temp = (char *)kmalloc(sizeof(char) * size + 1, GFP_KERNEL);
    printk("<1>JohnDriver: write  (size=%zu)\n", size);
    printk("<1>JohnDriver: write  %s\n", buf);

    memset(temp, 0, size + 1);
    strncpy(temp, buf, size);
    if (strcmp(temp, "Hello") == 0) {
	kfree(temp);
	//kernel_power_off();
	printk("<1>JohnDriver: Reboot");
	kernel_restart("Restarting kernel!");
    }
    kfree(temp);
    return size;
}

static struct file_operations JohnDriver_fops = {
    .open = JohnDriver_open,
    .release = JohnDriver_close,
    .read = JohnDriver_read,
    .write = JohnDriver_write,
};

static int JohnDriver_init(void) {
    int result;
    printk("<1>JohnDriver: init\n");

    /* Register character device */
    result = register_chrdev(John_MAJOR, John_NAME, &JohnDriver_fops);
    if (result < 0) {
        printk("<1>JohnDriver: Failed to register character device\n");
        return result;
    }

    return 0;
}

static void JohnDriver_exit(void) {
    printk("<1>JohnDriver: exit\n");

    /* Unregister character device */
    unregister_chrdev(John_MAJOR, John_NAME);
}

module_init(JohnDriver_init);
module_exit(JohnDriver_exit);

