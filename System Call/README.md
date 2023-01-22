# Overview

In this assignment, I learned how to add a system call to the kernel, commit my changes to the repository of Linux development team  and create a patch file from my commit.

## Implementation
This custom system call should: <br />
	* Be named **bitmarge**
	* Accept two 32-bit parameters called **high_bits** and **low_bits**.
	* Return a 64-bit value whose upper 32 bits are equal to **high_bits** and whose lower 32 bits are equal to **low_bits**.
	* If the most significant bit of the resulting merged parameters is set, the system call should instead return an error code signifying that an invalid value passed to it.

## Test from userspace program
To test this custom system call, I had to write some non-kernel code that tests that my system call works correctly. There is a C function for calling arbitrary system calls nam **syscall()**, so I can write my test program in C. (see **man 2 syscall**)
