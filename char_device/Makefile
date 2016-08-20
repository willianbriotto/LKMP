obj-m += char_device.o

KDIR = /usr/src/linux-headers-4.4.0-31-generic

all:
	$(MAKE) -C $(KDIR) M=${CURDIR} modules

clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order
