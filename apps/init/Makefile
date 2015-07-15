##
# Makefile for init.
#

AS=as
LD=ld
OUTPUT=init

all: init.o
	$(LD) $< -melf_i386 -Ttext=C0000C --oformat=binary -o $(OUTPUT)

init.o: init.s
	$(AS) --32 $< -o $@ 

clean:
	@[ -f init.o ] && rm init.o || true
	@[ -f $(OUTPUT) ] && rm $(OUTPUT) || true

