CC ?= cc
AR ?= ar
CFLAGS ?= -std=c11 -Wall -Wextra -O2
PREFIX ?= /usr/local

BUILD := build
LIB := $(BUILD)/libDoubleSeaLib.a
SHARED := $(BUILD)/libDoubleSeaLib.so
TEST := $(BUILD)/SeaTrials

.PHONY: all test install clean

all: $(LIB) $(SHARED)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/DoubleSeaLib.o: src/DoubleSeaLib.c include/DoubleSeaLib.h | $(BUILD)
	$(CC) $(CFLAGS) -fPIC -Iinclude -c $< -o $@

$(LIB): $(BUILD)/DoubleSeaLib.o
	$(AR) rcs $@ $^

$(SHARED): $(BUILD)/DoubleSeaLib.o
	$(CC) -shared $^ -o $@

$(TEST): tests/SeaTrials.c $(LIB)
	$(CC) $(CFLAGS) -Iinclude $^ -o $@

test: $(TEST)
	$(TEST)

install: $(LIB) $(SHARED)
	install -d $(DESTDIR)$(PREFIX)/lib $(DESTDIR)$(PREFIX)/include
	install -m 644 $(LIB) $(SHARED) $(DESTDIR)$(PREFIX)/lib
	install -m 644 include/DoubleSeaLib.h $(DESTDIR)$(PREFIX)/include

clean:
	rm -rf $(BUILD)
