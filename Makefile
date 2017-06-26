obj-m += ransomguard.o

USER_COMPILE = ./userProcess/
ransomguard-objs += main.o hook_func.o file.o parse.o TTN.o log.o

KERNDIR=/lib/modules/$(shell uname -r)/build/

all:
	$(MAKE) -C $(KERNDIR) M=$(PWD) modules
	mv *.ko ./build/
	make -C $(USER_COMPILE) all

clean:
	$(MAKE) -C $(KERNDIR) M=$(PWD) clean
	make -C $(USER_COMPILE) clean
