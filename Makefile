LIBOBJS = series.o

grapt: $(LIBOBJS) grapt.o
	$(CC) -o $@ $^ -lm `pkg-config --cflags --libs cairo cairo-png`

libgrapt.a: $(LIBOBJS)
	$(AR) rcs $@ $^

test: libgrapt.a
	$(MAKE) -C t/ test

clean:
	rm -f *.o *.a && $(MAKE) -C t/ clean
tags: *.c
	etags *.c

.PHONY: tags clean
