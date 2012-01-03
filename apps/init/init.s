/*
 * Simple program to run on TempOS
 * Actually, this is our first user program!
 *
 * Author: Renê de Souza Pinto
 * Date:   03/01/2012
 */

.global _start

/* Note: Program starts at 12MB (linker will handle this)*/

.text
_start:
	movl $0x04, %eax   /* syscall number  */
	movl $0x01, %ebx   /* fd              */
	movl $msg,  %ecx   /* buffer          */
	movl $0x1D, %edx   /* size            */
	int  $0x85         /* make a syscall  */

/* Never exit from init.... */
loop:
	jmp loop

msg:
	.ascii "Hello world from Userspace!\n"

