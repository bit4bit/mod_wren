MODNAME=mod_wren.so
MODOBJ=mod_wren.o
MODCFLAGS=-Wall -Werror
TESTS=test/test_*.c

CC=gcc
CFLAGS=$(MODCFLAGS) `pkg-config freeswitch --cflags`
LDFLAGS=`pkg-config freeswitch --libs`

.PHONY: all
all: $(MODNAME)
 
$(MODNAME): $(MODOBJ)
	$(CC) -shared -o $@ $(MODOBJ) $(LDFLAGS)

.c.o: $<
	$(CC) $(CFLAGS) -fPIC -o $@ -c $<
 
.PHONY: clean
clean:
	rm -f $(MODNAME) $(MODOBJ)
 
.PHONY: install
install: $(MODNAME)
	install $(MODNAME) $(FS_MODULES)

.libs/mod_wren.so: $(MODOBJ)
	mkdir -p .libs
	$(CC) -fPIC -shared -o $@ $^ $(LDFLAGS)

.PHONY: check
check: $(TESTS) .libs/mod_wren.so
	$(CC)  $(CFLAGS) -DSWITCH_TEST_BASE_DIR_FOR_CONF=\"/usr/src/mod_wren/test\" -DSWITCH_TEST_BASE_DIR_OVERRIDE=\"/usr/src/mod_wren/test\" -o .check $(TESTS) $(LDFLAGS) /usr/lib/libfreeswitch.so && ./.check
