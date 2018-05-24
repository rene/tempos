/* 
 * write() system call implementation
 */
.global write

.text
write:
	/* pop return address */
	popl %eax
	/* pop arguments from stack */
	popl %ebx
	popl %ecx
	popl %edx
	/* push return address */
	pushl %eax
	/* do the system call */
	movl $0x04, %eax   /* write() syscall number  */
	int  $0x85
	ret

