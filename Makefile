MODNAME=mod_wren.so
MODOBJ=mod_wren.o wren_wrap.o /usr/src/wren/amalgamation/wren.o
MODCFLAGS=-Wall -Werror
TESTS=test/test_*.c

CC=gcc
CFLAGS=`pkg-config freeswitch --cflags`
LDFLAGS=`pkg-config freeswitch --libs`

.PHONY: all
all: $(MODNAME)
 

wren-base/bin/wren_cli.a:  wren-base/src/freeswitch/*.c wren-base/src/module/* wren-base/src/module-freeswitch/*
	cd wren-base/projects/make.freeswitch && $(MAKE)
wren_cli.a: wren-base/bin/wren_cli.a
	cp wren-base/bin/wren_cli.a $@
wren_wrap.o: wren_wrap.c
	$(CC) -Iwren-base/deps/libuv/include -Iwren-base/deps/wren/include -Iwren-base/src/freeswitch $(CFLAGS) -fPIC -o $@ -c $<

$(MODNAME): $(MODOBJ)
	$(CC) -shared -o $@ $(MODOBJ) $(LDFLAGS)

.c.o: $<
	$(CC) $(CFLAGS) -fPIC -o $@ -c $<
 
.PHONY: clean
clean:
	rm -f $(MODNAME) $(MODOBJ)
	rm -rf wren-base/projects/make.freeswitch/obj
	rm -rf wren-base/bin/*

.PHONY: install
install: $(MODNAME)
	install $(MODNAME) $(FS_MODULES)

.libs/mod_wren.so: $(MODOBJ) wren_cli.a
	mkdir -p .libs
	$(CC) $(MODCFLAGS) $(CFLAGS) -fPIC -shared -o $@ $^ $(LDFLAGS)

.PHONY: check
check: $(TESTS) .libs/mod_wren.so
	$(CC) $(CFLAGS) -DSWITCH_TEST_BASE_DIR_FOR_CONF=\"/usr/src/mod_wren/test\" -DSWITCH_TEST_BASE_DIR_OVERRIDE=\"/usr/src/mod_wren/test\" -o .check $(TESTS) $(LDFLAGS) && ./.check
