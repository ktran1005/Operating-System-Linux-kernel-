## Overview 

This assignment will write a simple kernel module called **vix**.
This kernel module will manage a linked list of the following struct: <br />

```c
struct vix_dev {
  int id;
  char *name;
};
```
The kernel module will also expose a **debugfs** interface that will allow you to manipulate this linked list while the module is loaded. <br />

The module will create a folder in **/sys/kernel/debug** named **vix**. Insidethis folder, the module will create two files: <br />
  * **devices** -- This file should only be readable and writable by theowner (-rw-------).
    * **Write Behavior**: Writing a string of characters to this file will cause your module to allocate a new **struct vix_dev**. The **name** memeber should be
        initialized to point to a string that contains the same characters that were written to the file. The **id** member should be set to the smallest unsigned
        integer that has never been used as an number that gets assigned to a device should be zero. id numbers are never reused. This new struct vix_dev should be 
        added to the linked list managed by the module.
    * **Read Behavior**: Reading from **devices** should produce a list of the current devices that have been added to the linked list managed by the module. The format should look like the following: <br />
    000: webcam <br />
    001: keyboard <br />
    002: mouse <br />
    003: monitor <br />
  * **eject** -- This file should only be writable by the owner(--w-------). Writing a device's id number to this file will cause your module to remove the 
  corresponding struct vix_dev from the linked list and free any memory used by that vix_dev. Writing an id number that is currently not assigned to a vix device      should result in returning **-EINVAL**.
