KDIR ?= /lib/modules/$(shell uname -r)/build

obj-m += vix.o

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
