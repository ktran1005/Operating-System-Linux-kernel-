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
    * **Write Behavior**: Writing a string of characters to this file will cause your module to allocate a new **structvix_dev**. The **name** memeber should be
        initialized to point to a string that contains the same characters that were written to the file.
