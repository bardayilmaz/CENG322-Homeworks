obj-m += proc_info_module.o

#KDIR := /lib/modules/$(shell uname -r)/build
#PWD := $(shell pwd)

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

