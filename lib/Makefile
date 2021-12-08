CFLAGS  = -g -O3 -march=i386 -ggdb3 -m32 -std=gnu99 -fshort-wchar -Wno-multichar -Iloadlibrary/include -mstackrealign
CPPFLAGS=-DNDEBUG -D_GNU_SOURCE -I. -Iloadlibrary/intercept -Iloadlibrary/peloader
LDFLAGS = $(CFLAGS) -m32 -lm -Wl,--dynamic-list=loadlibrary/exports.lst
LDLIBS  = loadlibrary/intercept/libdisasm.a -Wl,--whole-archive,loadlibrary/peloader/libpeloader.a,--no-whole-archive

.PHONY: clean peloader

TARGETS=libsamwid.so

all: $(TARGETS)

peloader:
	make -C loadlibrary all

libsamwid.so: samwid.o loadlibrary/intercept/hook.o | peloader
	$(CC) $(CFLAGS) $^ -shared -o $@ $(LDLIBS) $(LDFLAGS)

clean:
	rm -f a.out core *.o core.* vgcore.* gmon.out libsamwid.so

loadlibrary/intercept/hook.o: peloader
