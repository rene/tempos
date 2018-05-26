##
# Makefile for calc.
#
CC=gcc
CFLAGS=-m32 -Os -mpush-args -fno-pie -fomit-frame-pointer -fno-builtin -nostartfiles -nostdlib -nodefaultlibs
AS=as
LD=ld
OUTPUT=init

OBJECTS=crt0.o syscalls.o printf.o stdlib.o init.o

all: $(OBJECTS)
	$(LD) $(OBJECTS) -melf_i386 -Ttext=C0000C --oformat=binary -o $(OUTPUT)
	#$(LD) $(OBJECTS) -melf_i386 -o $(OUTPUT)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

%.o: %.s
	$(AS) --32 $< -o $@

clean:
	@rm $(OBJECTS) || true
	@[ -f $(OUTPUT) ] && rm $(OUTPUT) || true

