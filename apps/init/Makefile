##
# Makefile for init.
#

AS=as
LD=ld
OUTPUT=hello

all: init.o
	$(LD) $< -melf_i386 -Ttext=A00000 --oformat=binary -o $(OUTPUT)

init.o: init.s
	$(AS) --32 $< -o $@ 

clean:
	@[ -f init.o ] && rm init.o || true
	@[ -f $(OUTPUT) ] && rm $(OUTPUT) || true

