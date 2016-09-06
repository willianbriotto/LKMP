obj-m += info_headers.o

KDIR = /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=${CURDIR} modules

clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order
