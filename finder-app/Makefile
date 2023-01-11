# A default target which builds the “writer” application
# A clean target which removes the “writer” application and all .o files
# CROSS_COMPILE aarch64-none-linux-gnu-
# aarch64-none-linux-gnu-gcc -g -Wall -c -o writer.o writer.c

ifeq ($(CROSS_COMPILE), aarch64-none-linux-gnu-)
	CC := $(CROSS_COMPILE)gcc
endif
writer: writer.o
	$(CC) -o writer writer.o


.PHONY: clean
clean:
	rm -f writer.o writer
