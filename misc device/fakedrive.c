// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/string.h>

#define BUFFER "14377220"
#define MAXSIZE 1000

static ssize_t ecec421_read(struct file *filp, char __user *buf,
			    size_t size, loff_t *offset)
{
	int ret;

	ret = simple_read_from_buffer(buf, size, offset,
				      BUFFER, strlen(BUFFER));

	return ret;
}

static ssize_t ecec421_write(struct file *filp, const char __user *buf,
			     size_t size, loff_t *offset)
{
	int ret;
	char buf_kernel[MAXSIZE];

	ret = simple_write_to_buffer(buf_kernel, strlen(buf), offset, buf, size);
	if (strcmp(buf_kernel, BUFFER) == 0) {
		pr_info("fakedrive: initilized\n");
	} else {
		pr_info("fakedrive: initialization failed\n");
		return -EINVAL;
	}

	return ret;
}

static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = ecec421_read,
	.write = ecec421_write,
};

static struct miscdevice ecec421_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "fakedrive",
	.fops = &fops,
};

static int __init ecec421_init(void)
{
	int ret;

	pr_info("Module %s (v%s) loaded\n", THIS_MODULE->name, THIS_MODULE->version);

	ret = misc_register(&ecec421_device);
	if (ret)
		pr_info("misc_register failed\n");
	else
		pr_info("fakedrive: misc char successfully registered\n");

	return ret;
}

static void __exit ecec421_exit(void)
{
	misc_deregister(&ecec421_device);
	pr_info("misc char device module unloaded\n");
}

module_init(ecec421_init);
module_exit(ecec421_exit);

MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
MODULE_AUTHOR("Charles <kdt57@drexel.edu");
MODULE_DESCRIPTION("ECEC-421: HW5");
