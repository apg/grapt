CFLAGS = -g
LDFLAGS = -L.. 
LDLIBS = -lgrapt

# list all the test outputs here. 
# each test should be a single file identically named
TESTS = 01_series_test

test: $(TESTS)
	./run.sh

clean:
	rm $(TESTS)

.PHONY: test clean
