#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>

#define MAX_MESSAGE_SIZE 100
static char kernel_buffer[MAX_MESSAGE_SIZE];

static ssize_t write_to_kernel(struct file *file, const char __user *user_buffer, size_t count, loff_t *ppos)
{
    // TODO Bonus 1: check if the user is trying to write more than MAX_MESSAGE_SIZE bytes
    // if so, return -EINVAL (Invalid argument)
    if (strlen(user_buffer) > 100)
	return -EINVAL;
    // TODO 1: copy from the user buffer to kernel buffer by using copy_from_user()
    // Hint: Only one line of code is needed
    printk("count: %d", count);
    copy_from_user(kernel_buffer, user_buffer, count);

    pr_info("Received from user space: %s", kernel_buffer);
    return count;
}

static ssize_t read_from_kernel(struct file *file, char __user *user_buffer, size_t count, loff_t *ppos)
{
    ssize_t retval;

    if (*ppos > strlen(kernel_buffer))
        return -EINVAL;

    // TODO 2:
    // General solution: initialize retval to count
    // Bonus solution for Test Case 5: initialize retval to the minimum value between count and the length of the message
    // Hint: Only one line of code is needed
    retval = min(strlen(kernel_buffer) - *ppos, count);

    // TODO 3: copy from the kernel buffer to user buffer by using copy_to_user()
    // Hint: Only one line of code is needed
    copy_to_user(user_buffer, kernel_buffer + *ppos, count);

    pr_info("Read %zd bytes from kernel space starting at position %lld\n", retval, *ppos);
    pr_info("Read from kernel space: %.*s\n", (int)retval, kernel_buffer + *ppos);

    // TODO 4: update the current position of the file
    // Hint: Only one line of code is needed
    *ppos += count;

    return retval;
}

static const struct proc_ops proc_operations = {
    // TODO 5: assign the correct function pointers to the correct functions
    // Hint: Only two lines of code is needed
    .proc_read = read_from_kernel,
    .proc_write = write_to_kernel,
};

static int __init vfs_init(void)
{
    // TODO 6: create a proc entry at /proc/my_proc_file
    // Hint: Only one line of code is needed

    proc_create("my_proc_file", 0777, NULL, &proc_operations);
    pr_info("Module loaded\n");
    return 0;
}

static void __exit vfs_exit(void)
{
    // TODO 7: remove the proc entry at /proc/my_proc_file
    // Hint: Only one line of code is needed
    remove_proc_entry("my_proc_file", NULL);

    pr_info("Module unloaded\n");
}

module_init(vfs_init);
module_exit(vfs_exit);

MODULE_VERSION("0.1");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("CSE330 Project 3 Virtual File System\n");

