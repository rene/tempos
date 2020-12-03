.global _start,main

.text
_start:
	call main

loop:
	jmp loop

