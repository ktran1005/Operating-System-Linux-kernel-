## Overview

This assignment will be about kobjects. You will create a few **fakedrive** devices that will have entries in **sysfs** -- each exposing a few
attributes. Implement this as kernel module named **kobject.c** <br />

A **fakedrive** device, is described by the following struct: <br />

```c
**struct** fakedrive {
  **unsigned long** capacity;
  **const char** *model;
  **const char** *rev;
  **struct kobject** kobj;
  **int** powersave;
};
```
