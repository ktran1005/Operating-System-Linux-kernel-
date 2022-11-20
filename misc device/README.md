# Overview
This assignment covers a simple kernel module that implements a misc device.

## Implement Misc device
This module will do the following:
	The misc device should be created with a dynamic minor number.
	The misc device should implement the read and write file operations.
	The misc device node should show up in /dev/fakedrive
	When the character device is read from, it should populate the user spacebuffer with your Drexel student ID number encoded in ASCII and provide thecorrect return value for read()
		When the character device is written to, the data sent to the kernel moduleneeds to be checked. If it matches your ASCII encoded Drexel student ID number, then return the correct return value for write() and print "fakedrive:initialized" to the kernel log (ring buffer).
		If it does not match your ASCII encoded Drexel student ID number,then return -EINVAL and print "fakedrive: initialization failed" to the kernel log.
	The misc device should be registered when your module is loaded andunregistered when your module is unloaded.
