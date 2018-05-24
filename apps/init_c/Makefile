##
# Makefile for init.
#
CC=gcc
AS=as
LD=ld
OUTPUT=init

OBJECTS=crt0.o write.o init.o

all: $(OBJECTS)
	$(LD) $(OBJECTS) -melf_i386 -Ttext=C0000C --oformat=binary -o $(OUTPUT)

init.o: init.c
	$(CC) -m32 -mpush-args -nostdlib -c $<

crt0.o: crt0.s
	$(AS) --32 $< -o $@

write.o: write.s
	$(AS) --32 $< -o $@ 

clean:
	@rm $(OBJECTS) || true
	@[ -f $(OUTPUT) ] && rm $(OUTPUT) || true

