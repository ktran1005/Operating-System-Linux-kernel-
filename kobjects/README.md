## Overview

This assignment will be about **kobjects**. You will create a few **fakedrive** devices that will have entries in **sysfs** -- each exposing a few
attributes. Implement this as kernel module named **kobject.c** <br />

A **fakedrive** device, is described by the following struct: <br />

```c
struct fakedrive {
  unsigned long capacity;
  const char *model;
  const char *rev;
  struct kobject kobj;
  int powersave;
};
```

## Implementation
Design and implement a **struct kobj_type** in your kernel module that exposes the **capacity, model, rev, and powersave** members to sysfs as attributes. In 
your module's init function, create four (4) fakedrive devices named **device0, device1, device2, and device3** and add them to the system. These devices should 
be initialized with the following values upon creation: <br />

[![Capture.png](https://i.postimg.cc/4NZ5Jh2B/Capture.png)](https://postimg.cc/MXPR5XSQ)

These four (4) fakedrive devices should belong to a kset named **"fakedrive"**. This kset should be visible to userspace at **/sys/kernel/fakedrive.** <br />



Setup the attributes of your **kobj_type** so that once all four (4) **fakedrives** have been created, the directory structure organization looks like this: <br />

[![Capture-1.png](https://i.postimg.cc/NFW7v5Vv/Capture-1.png)](https://postimg.cc/D4Pbqynx)

As shown, the attributes **capacity, model, and rev** should all show up in a subdirectory named **info** under a given device's directory and should all 
be read-only by owner, group, and world -- i.e. [-r--r--r--]. <br />

The **powersave** attribute is special -- it should not live in the info subdirectory. Instead, it will live directly in the device's directory, parallel to the
info subdirectory. The **powersave** attribute should be both readable & writable by owner and group (and world should only be able to read) -- i.e. [-rw-rw-r--]. <br />

Here is the directory tree, again, but this time showing the required filepermissions to the left of the item they apply to: <br />
[![Capture-2.png](https://i.postimg.cc/d0BbQYwx/Capture-2.png)](https://postimg.cc/R3WsG237)

Writes to the **powersave** attribute should be stored in the **powersave** member of the corresponding **fakedrive** device. Subsequently reading the **powersave** attribute on sysfs should produce the current value of the corresponding device's **powersave** member. Only the values **0 or 1** can be written to **powersave** via userspace. Attempting to write any othervalue results in **write()** returning **-EINVAL**. <br />

Reading any of the other read-only attributes should always produce the default values provided in the table provided near the top of this assignment, which were used to initialize the **fakedrive** device object. <br />

When a new fakedrive device is added to the system, it should generate an add uevent -- announcing to userspace that the device exists. Likewise, when a fakedrive
device is removed from the system, it should generate a remove uevent -- announcing to userspace that the deviceno longer exists. These events can be captured with the following command: **udevadm monitor** <br />

## Demo
[![asciicast](https://asciinema.org/a/551617.svg)](https://asciinema.org/a/551617)


