LIBOBJS = series.o
LDFLAGS = -lcairo

grapt: $(LIBOBJS) grapt.o

libgrapt.a: $(LIBOBJS)
	$(AR) rcs $@ $^

test: libgrapt.a
	$(MAKE) -C t/ test

clean:
	rm -f *.o *.a && $(MAKE) -C t/ clean
tags: *.c
	etags *.c

.PHONY: tags clean
