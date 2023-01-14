// SPDX-License-Identifier: GPL-2.0

#include <linux/kobject.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/sysfs.h>
#include <linux/string.h>
#include <linux/slab.h>

struct fakedrive {
	unsigned long capacity;
	const char *model;
	const char *rev;
	struct kobject kobj;
	int powersave;
};

struct fakedrive_attr {
	struct attribute attr;
	ssize_t (*show)(struct fakedrive *foo, struct fakedrive_attr *attr, char *buf);
	ssize_t (*store)(struct fakedrive *foo, struct fakedrive_attr *attr, char const *buf, size_t count);
};

static ssize_t capacity_show(struct fakedrive *fobj, struct fakedrive_attr *fattr,
			     char *buf)
{
	return sprintf(buf, "%ld\n", fobj->capacity);

}

static ssize_t capacity_store(struct fakedrive *fobj, struct fakedrive_attr *fattr,
			      const char *buf, size_t len)
{
	return len;
}

static ssize_t model_show(struct fakedrive *fobj, struct fakedrive_attr *fattr, char *buf)
{
	return sprintf(buf, "%s\n", fobj->model);
}

static ssize_t model_store(struct fakedrive *fobj, struct fakedrive_attr *fattr,
			   const char *buf, size_t len)
{
	return len;
}

static ssize_t rev_show(struct fakedrive *fobj, struct fakedrive_attr *fattr, char *buf)
{
	return sprintf(buf, "%s\n", fobj->rev);
}


static ssize_t rev_store(struct fakedrive *fobj, struct fakedrive_attr *fattr,
			   const char *buf, size_t len)
{
	return len;
}


static ssize_t powersave_show(struct fakedrive *fobj, struct fakedrive_attr *fattr,
			     char *buf)
{
	return sprintf(buf, "%d\n", fobj->powersave);
}

static ssize_t powersave_store(struct fakedrive *fobj, struct fakedrive_attr *fattr,
			       const char *buf, size_t len)
{
	int ret;

	if ((buf[0] == '0') || (buf[0] == '1')) {
		ret = kstrtoint(buf, 10, &fobj->powersave);
		if (ret < 0)
			return  ret;

		return len;
	} else
		return -EINVAL;
}

static ssize_t fakedrive_attr_show(struct kobject *kobj, struct attribute *attr, char *buf)
{
	struct fakedrive_attr *fattr = container_of(attr, struct fakedrive_attr, attr);
	struct fakedrive *fobj = container_of(kobj, struct fakedrive, kobj);

	if (!fattr->show)
		return -EIO;

	return fattr->show(fobj, fattr, buf);
}

static ssize_t fakedrive_attr_store(struct kobject *kobj,
				    struct attribute *attr, const char *buf, size_t len)
{
	struct fakedrive_attr *fattr = container_of(attr, struct fakedrive_attr, attr);
	struct fakedrive *fobj = container_of(kobj, struct fakedrive, kobj);

	if (!fattr->store)
		return -EIO;

	return fattr->store(fobj, fattr, buf, len);
}

static struct fakedrive_attr capacity_attr =
	__ATTR(capacity, 0444, capacity_show, capacity_store);
static struct fakedrive_attr model_attr =
	__ATTR(model, 0444, model_show, model_store);
static struct fakedrive_attr rev_attr =
	__ATTR(rev, 0444, rev_show, rev_store);
static struct fakedrive_attr powersave_attr =
	__ATTR(powersave, 0664, powersave_show, powersave_store);

static struct attribute *fakedrive_default_attrs[] = {
	&capacity_attr.attr,
	&model_attr.attr,
	&rev_attr.attr,
	NULL
};

static struct attribute *fakedrive_special_attrs[] = {
	&powersave_attr.attr,
	NULL
};

static struct attribute_group fakedrive_group = {
	.name = "info",
	.attrs = fakedrive_default_attrs,
};

static struct attribute_group fakedrive_special = {
	.name = NULL,
	.attrs = fakedrive_special_attrs,
};

static const struct attribute_group *fakedrive_default_groups[] = {
	&fakedrive_group,
	&fakedrive_special,
	NULL
};

static const struct sysfs_ops fakedrive_sysfs_ops = {
	.show = fakedrive_attr_show,
	.store = fakedrive_attr_store,
};

void fakedrive_release(struct kobject *kobj)
{
	struct fakedrive *fobj;

	fobj = container_of(kobj, struct fakedrive, kobj);
	kfree(fobj);
}

static struct kobj_type fakedrive_ktype = {
	.sysfs_ops = &fakedrive_sysfs_ops,
	.release = fakedrive_release,
	.default_groups = fakedrive_default_groups,
};

static struct kset *fakedrive_kset;

static struct fakedrive *fakedrive_create(const char *name)
{
	struct fakedrive *new;
	int ret;

	new = kzalloc(sizeof(*new), GFP_KERNEL);
	if (!new)
		return NULL;

	new->kobj.kset = fakedrive_kset;

	ret = kobject_init_and_add(&new->kobj, &fakedrive_ktype, NULL, "%s", name);
	if (ret) {
		kobject_put(&new->kobj);
		return NULL;
	}

	kobject_uevent(&new->kobj, KOBJ_ADD);

	return new;

}

static int __init fakedrive_init(void)
{
	static struct fakedrive *fak_device0, *fak_device1, *fak_device2, *fak_device3;

	fakedrive_kset = kset_create_and_add("fakedrive", NULL, kernel_kobj);
	if (!fakedrive_kset)
		return -ENOMEM;

	fak_device0 = fakedrive_create("device0");
	if (!fak_device0)
		goto dev0_error;
	fak_device0->capacity = 1922320888;
	fak_device0->model = "Samsung EVO SSD";
	fak_device0->rev = "e7va";
	fak_device0->powersave = 0;

	fak_device1 = fakedrive_create("device1");
	if (!fak_device1)
		goto dev1_error;
	fak_device1->capacity = 2884153776;
	fak_device1->model = "Western Digital Green HD";
	fak_device1->rev = "wd3922123b";
	fak_device1->powersave = 1;

	fak_device2 = fakedrive_create("device2");
	if (!fak_device2)
		goto dev2_error;
	fak_device2->capacity = 229701780;
	fak_device2->model = "Crucial SSD";
	fak_device2->rev = "sg41";
	fak_device2->powersave = 1;

	fak_device3 = fakedrive_create("device3");
	if (!fak_device3)
		goto dev3_error;
	fak_device3->capacity = 205937544;
	fak_device3->model = "Samsung EVO NVMe";
	fak_device3->rev = "e9nv1";
	fak_device3->powersave = 0;

	return 0;

dev3_error:
	kobject_put(&fak_device2->kobj);
dev2_error:
	kobject_put(&fak_device1->kobj);
dev1_error:
	kobject_put(&fak_device0->kobj);
dev0_error:
	kset_unregister(fakedrive_kset);
	return -EINVAL;
}

static void __exit fakedrive_exit(void)
{
	struct kobject *entry, *sav;

	list_for_each_entry_safe(entry, sav, &fakedrive_kset->list, entry) {
		kobject_put(entry);
	}

	kset_unregister(fakedrive_kset);
}

module_init(fakedrive_init);
module_exit(fakedrive_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Charles Tran");
MODULE_DESCRIPTION("HW7");
