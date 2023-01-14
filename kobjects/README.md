## Overview

This assignment will be about kobjects. You will create a few **fakedrive** devices that will have entries in **sysfs** -- each exposing a few
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
