# Overview
This assignment covers a simple kernel module that implements a misc device.

## Implementation
This module will do the following: <br />

    * The misc device should be created with a dynamic minor number. <br />
    * The misc device should implement the read and write file operations. <br />
    * The misc device node should show up in **/dev/fakedrive** <br />
    * When the character device is read from, it should populate the user spacebuffer with your Drexel student ID number encoded in ASCII and provide thecorrect return value for read() <br />
    * When the character device is written to, the data sent to the kernel moduleneeds to be checked. If it matches your ASCII encoded Drexel student ID number, then return the correct return value for write() and print "fakedrive:initialized" to the kernel log (ring buffer). <br />
          * If it does not match your ASCII encoded Drexel student ID number,then return -EINVAL and print "fakedrive: initialization failed" to the kernel log. <br />
    * The misc device should be registered when your module is loaded andunregistered when your module is unloaded. <br />
 
## Test from user space program
This program is called **test.c** that produces the output similar to that shown in the demo video. <br />

This is a very simple program that opens **/dev/fakedrive** that does the following: <br />
  * Tests reading by doing the following:


## Demo
[![asciicast](https://asciinema.org/a/wbdJOrpQEANF3NnuRklDHG4A5.svg)](https://asciinema.org/a/wbdJOrpQEANF3NnuRklDHG4A5)
