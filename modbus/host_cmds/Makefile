
all : libmodbus libleddrv led-server led-client

LIBMODBUS_VER = 3.1.6

ROOT_BASE = $(PWD)/..

BUILD_BASE = $(ROOT_BASE)/host_cmds/build

EMULATE_BASE = $(ROOT_BASE)/emulate

LIBS_BASE = $(ROOT_BASE)/libs

CMDS_BASE = $(ROOT_BASE)/cmds

LIBDRV_INCLUDES_BASE = $(BUILD_BASE)/libleddrv/include

LD_FLAGS_LIBDRV = -L$(BUILD_BASE)/libleddrv -lleddrv
LD_FLAGS_LIBMODBUS = -L$(BUILD_BASE)/libmodbus/lib -lmodbus

CFLAGS_LIBDRV_INCLUDES = -I$(LIBDRV_INCLUDES_BASE)
CFLAGS_LIBMODBUS_INCLUDES = -I$(BUILD_BASE)/libmodbus/include/modbus

libmodbus-$(LIBMODBUS_VER).tar.gz:
	wget http://libmodbus.org/releases/libmodbus-$(LIBMODBUS_VER).tar.gz

$(BUILD_BASE)/libmodbus/lib/pkgconfig/libmodbus.pc : libmodbus-$(LIBMODBUS_VER).tar.gz
	tar -xf libmodbus-$(LIBMODBUS_VER).tar.gz
	cd libmodbus-$(LIBMODBUS_VER); \
	./configure --prefix=$(BUILD_BASE)/libmodbus --enable-static --disable-shared; \
	make install; cd ..;

libmodbus: $(BUILD_BASE)/libmodbus/lib/pkgconfig/libmodbus.pc

libleddrv:
	mkdir -p $(BUILD_BASE)/libleddrv
	mkdir -p $(LIBDRV_INCLUDES_BASE)
	mkdir -p $(BUILD_BASE)/libleddrv/objs
	cp $(LIBS_BASE)/libleddrv/libleddrv.h $(BUILD_BASE)/libleddrv/include
	cp $(LIBS_BASE)/libleddrv_ll/libleddrv_ll.h $(BUILD_BASE)/libleddrv/include
	cp $(LIBS_BASE)/libled_names/led_names.h $(BUILD_BASE)/libleddrv/include
	gcc $(CFLAGS_LIBDRV_INCLUDES) $(LIBS_BASE)/libleddrv/libleddrv.c -c -o $(BUILD_BASE)/libleddrv/objs/libleddrv.o
	gcc $(CFLAGS_LIBDRV_INCLUDES) $(LIBS_BASE)/libleddrv_ll/libleddrv_ll_stub.c -c -o $(BUILD_BASE)/libleddrv/objs/libleddrv_ll_stub.o
	gcc $(CFLAGS_LIBDRV_INCLUDES) $(LIBS_BASE)/libled_names/libled_names.c -c -o $(BUILD_BASE)/libleddrv/objs/libled_names.o
	ar rcs $(BUILD_BASE)/libleddrv/libleddrv.a \
			$(BUILD_BASE)/libleddrv/objs/libleddrv.o \
			$(BUILD_BASE)/libleddrv/objs/libleddrv_ll_stub.o \
			$(BUILD_BASE)/libleddrv/objs/libled_names.o

led-server led-client : CFLAGS += -Wall -g3 -O0 $(CFLAGS_LIBDRV_INCLUDES) $(CFLAGS_LIBMODBUS_INCLUDES)

$(EMULATE_BASE)/leds.txt:
	mkdir -p $(EMULATE_BASE)
	cp leds.txt $(EMULATE_BASE)
	
led-client : libmodbus
	gcc $(CFLAGS) led-client.c -o $(EMULATE_BASE)/$@ $(LD_FLAGS_LIBMODBUS)

led-server : libmodbus libleddrv $(EMULATE_BASE)/leds.txt
led-server : $(CMDS_BASE)/modbus_server/modbus_server.c
	$(CC) $(CFLAGS) $(CMDS_BASE)/modbus_server/modbus_server.c -o $(EMULATE_BASE)/$@ $(LD_FLAGS_LIBMODBUS) $(LD_FLAGS_LIBDRV)

clean :
	rm -rf $(BUILD_BASE) $(EMULATE_BASE)/led-server $(EMULATE_BASE)/led-client $(EMULATE_BASE)/leds.txt

