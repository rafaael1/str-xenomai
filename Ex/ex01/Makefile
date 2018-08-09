XENO_CONFIG := /usr/xenomai/bin/xeno-config
CFLAGS := $(shell $(XENO_CONFIG) --alchemy --cobalt --cflags)
LDFLAGS := $(shell $(XENO_CONFIG) --alchemy --cobalt --ldflags)
EXTRA :=
CC := gcc

EXECUTABLE := ex01 ex01b

all: $(EXECUTABLE)

%: %.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS) $(EXTRA)
clean:
	rm -f $(EXECUTABLE)
