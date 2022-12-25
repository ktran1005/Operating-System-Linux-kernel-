// SPDX-License-Identifier: GPL-2.0

#include <linux/list.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/spinlock.h>

static struct dentry *dir;
static char kbuffer[PAGE_SIZE];
static int count;
static int index;


struct vix_dev {
	struct list_head list;
	int id;
	char *name;
};

static LIST_HEAD(vix_list);
static DEFINE_SPINLOCK(vix_spinlock);

static int vix_create(char *s)
{
	struct vix_dev *new = kmalloc(sizeof(*new), GFP_KERNEL);

	if (!new)
		return -ENOMEM;
	new->name = kmalloc(strlen(s) + 1, GFP_KERNEL);
	strcpy(new->name, s);
	new->id = count;
	count++;
	list_add_tail(&new->list, &vix_list);

	return 0;
}

static ssize_t vix_read_devices(struct file *filp, char __user *buf,
		size_t count, loff_t *ppos)
{
	ssize_t ret;
	char id_string[20];
	struct vix_dev *cur = NULL;
	unsigned long vix_flag;

	spin_lock_irqsave(&vix_spinlock, vix_flag);
	list_for_each_entry(cur, &vix_list, list) {
		snprintf(id_string, 20, "%03d: ", cur->id);
		strcat(kbuffer, id_string);
		strcat(kbuffer, cur->name);
		strcat(kbuffer, "\n");
	}

	spin_unlock_irqrestore(&vix_spinlock, vix_flag);
	ret = simple_read_from_buffer(buf, count, ppos, kbuffer, strlen(kbuffer));
	memset(kbuffer, 0, strlen(kbuffer));

	return ret;
}

static ssize_t vix_write_devices(struct file *filp, const char __user *buf,
		size_t count, loff_t *ppos)
{
	ssize_t ret;
	unsigned long vix_flag;

	spin_lock_irqsave(&vix_spinlock, vix_flag);
	ret = simple_write_to_buffer(kbuffer, PAGE_SIZE, ppos, buf, count);
	kbuffer[strlen(buf)] = '\0';
	if (!vix_create(kbuffer)) {
		memset(kbuffer, 0, strlen(kbuffer));
		spin_unlock_irqrestore(&vix_spinlock, vix_flag);
		return ret;

	} else {
		spin_unlock_irqrestore(&vix_spinlock, vix_flag);
		return -EINVAL;
	}
}

static ssize_t vix_write_eject(struct file *filp, const char __user *buf,
		size_t count, loff_t *ppos)
{
	struct vix_dev *cur  = NULL;
	struct vix_dev *next = NULL;
	ssize_t ret;

	ret = kstrtoint_from_user(buf, count, 10, &index);

	if (ret)
		goto out;
	ret = count;

	list_for_each_entry_safe(cur, next, &vix_list, list) {
		if (cur->id == index) {
			list_del(&cur->list);
			kfree(cur->name);
			kfree(cur);
			goto out;
		}
	}
	return -EINVAL;
out:
	return ret;
}

static const struct file_operations devices_fops = {
	.owner = THIS_MODULE,
	.read  = vix_read_devices,
	.write = vix_write_devices,
};

static const struct file_operations eject_fops = {
	.owner = THIS_MODULE,
	.write = vix_write_eject,
};

static int __init vix_init(void)
{
	struct dentry *devices;
	struct dentry *eject;

	dir = debugfs_create_dir("vix", NULL);
	if (!dir)
		goto error;

	devices = debugfs_create_file("devices", 0600, dir, NULL, &devices_fops);
	if (!devices)
		goto error;

	eject = debugfs_create_file("eject", 0200, dir, NULL, &eject_fops);
	if (!eject)
		goto error;

	pr_info("debugfs example loaded\n");

	return 0;

error:
	pr_info("debugfs failed to load\n");
	debugfs_remove_recursive(dir);
	return -ENOMEM;
}

static void __exit vix_exit(void)
{
	struct vix_dev *cur = NULL;
	struct vix_dev *next = NULL;

	list_for_each_entry_safe(cur, next, &vix_list, list) {
		list_del(&cur->list);
		kfree(cur->name);
		kfree(cur);
	}

	debugfs_remove_recursive(dir);
	pr_info("HW6: debugfs module unloaded\n");
}

module_init(vix_init);
module_exit(vix_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Charles Tran");
MODULE_DESCRIPTION("HW6");
