#---------Makefile ---------#
obj-m := test.o hello-2.o term.o origin.o

KERNEL_DIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default :
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules
clean :
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
