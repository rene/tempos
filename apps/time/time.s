	.file	"time.c"
	.text
	.type	numtostr, @function
numtostr:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -88(%rbp)
	movl	%esi, %eax
	movq	%rdx, -104(%rbp)
	movl	%ecx, -96(%rbp)
	movl	%r8d, -108(%rbp)
	movl	%r9d, %edx
	movb	%al, -92(%rbp)
	movl	%edx, %eax
	movb	%al, -112(%rbp)
	movq	-88(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -16(%rbp)
	movq	$0, -56(%rbp)
	cmpb	$0, -112(%rbp)
	je	.L2
	movq	-104(%rbp), %rax
	sarq	$63, %rax
	movq	%rax, %rdx
	xorq	-104(%rbp), %rdx
	subq	%rax, %rdx
	movq	%rdx, %rax
	movq	%rax, -48(%rbp)
	movq	$1, -40(%rbp)
	movq	$0, -8(%rbp)
	cmpl	$16, -96(%rbp)
	je	.L7
	jmp	.L4
.L5:
	movl	-96(%rbp), %eax
	movslq	%eax, %rsi
	movq	-48(%rbp), %rax
	cqto
	idivq	%rsi
	movq	%rax, -40(%rbp)
	movl	-96(%rbp), %eax
	movslq	%eax, %rcx
	movq	-48(%rbp), %rax
	cqto
	idivq	%rcx
	movq	%rdx, %rax
	movq	%rax, -8(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, -48(%rbp)
	movq	-8(%rbp), %rax
	leal	48(%rax), %ecx
	movq	-56(%rbp), %rdx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	movl	%ecx, %edx
	movb	%dl, (%rax)
	addq	$1, -56(%rbp)
.L4:
	cmpq	$0, -40(%rbp)
	jne	.L5
	jmp	.L6
.L10:
	movl	-96(%rbp), %eax
	movslq	%eax, %rdi
	movq	-48(%rbp), %rax
	cqto
	idivq	%rdi
	movq	%rax, -40(%rbp)
	movl	-96(%rbp), %eax
	movslq	%eax, %rcx
	movq	-48(%rbp), %rax
	cqto
	idivq	%rcx
	movq	%rdx, %rax
	movq	%rax, -8(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, -48(%rbp)
	cmpq	$9, -8(%rbp)
	jbe	.L8
	movq	-8(%rbp), %rax
	leal	55(%rax), %ecx
	movq	-56(%rbp), %rdx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	movl	%ecx, %edx
	movb	%dl, (%rax)
	jmp	.L9
.L8:
	movq	-8(%rbp), %rax
	leal	48(%rax), %ecx
	movq	-56(%rbp), %rdx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	movl	%ecx, %edx
	movb	%dl, (%rax)
.L9:
	addq	$1, -56(%rbp)
.L7:
	cmpq	$0, -40(%rbp)
	jne	.L10
	jmp	.L6
.L2:
	movq	-104(%rbp), %rax
	movl	%eax, %eax
	movq	%rax, -32(%rbp)
	movq	$1, -24(%rbp)
	movq	$0, -8(%rbp)
	cmpl	$16, -96(%rbp)
	je	.L14
	jmp	.L12
.L13:
	movl	-96(%rbp), %eax
	movslq	%eax, %rsi
	movq	-32(%rbp), %rax
	movl	$0, %edx
	divq	%rsi
	movq	%rax, -24(%rbp)
	movl	-96(%rbp), %eax
	movslq	%eax, %rcx
	movq	-32(%rbp), %rax
	movl	$0, %edx
	divq	%rcx
	movq	%rdx, -8(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -32(%rbp)
	movq	-8(%rbp), %rax
	leal	48(%rax), %ecx
	movq	-56(%rbp), %rdx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	movl	%ecx, %edx
	movb	%dl, (%rax)
	addq	$1, -56(%rbp)
.L12:
	cmpq	$0, -24(%rbp)
	jne	.L13
	jmp	.L6
.L17:
	movl	-96(%rbp), %eax
	movslq	%eax, %rdi
	movq	-32(%rbp), %rax
	movl	$0, %edx
	divq	%rdi
	movq	%rax, -24(%rbp)
	movl	-96(%rbp), %eax
	movslq	%eax, %rcx
	movq	-32(%rbp), %rax
	movl	$0, %edx
	divq	%rcx
	movq	%rdx, -8(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, -32(%rbp)
	cmpq	$9, -8(%rbp)
	jbe	.L15
	movq	-8(%rbp), %rax
	leal	55(%rax), %ecx
	movq	-56(%rbp), %rdx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	movl	%ecx, %edx
	movb	%dl, (%rax)
	jmp	.L16
.L15:
	movq	-8(%rbp), %rax
	leal	48(%rax), %ecx
	movq	-56(%rbp), %rdx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	movl	%ecx, %edx
	movb	%dl, (%rax)
.L16:
	addq	$1, -56(%rbp)
.L14:
	cmpq	$0, -24(%rbp)
	jne	.L17
.L6:
	subq	$1, -56(%rbp)
	movl	$0, -68(%rbp)
	jmp	.L18
.L19:
	movl	-68(%rbp), %eax
	movslq	%eax, %rdx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	movb	%al, -69(%rbp)
	movl	-68(%rbp), %eax
	cltq
	movq	-56(%rbp), %rdx
	subq	%rax, %rdx
	movq	%rdx, %rax
	movq	%rax, %rdx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	movl	-68(%rbp), %edx
	movslq	%edx, %rcx
	movq	-16(%rbp), %rdx
	addq	%rcx, %rdx
	movzbl	(%rax), %eax
	movb	%al, (%rdx)
	movl	-68(%rbp), %eax
	cltq
	movq	-56(%rbp), %rdx
	subq	%rax, %rdx
	movq	%rdx, %rax
	movq	%rax, %rdx
	movq	-16(%rbp), %rax
	addq	%rax, %rdx
	movzbl	-69(%rbp), %eax
	movb	%al, (%rdx)
	addl	$1, -68(%rbp)
.L18:
	movl	-68(%rbp), %eax
	movslq	%eax, %rdx
	movq	-56(%rbp), %rax
	movq	%rax, %rcx
	shrq	$63, %rcx
	addq	%rcx, %rax
	sarq	%rax
	cmpq	%rax, %rdx
	jle	.L19
	movl	$0, -64(%rbp)
	movb	$0, -71(%rbp)
	movzbl	-92(%rbp), %eax
	sarb	$3, %al
	movsbl	%al, %eax
	andl	$1, %eax
	testl	%eax, %eax
	jne	.L20
	movzbl	-92(%rbp), %eax
	sarb	$4, %al
	movsbl	%al, %eax
	andl	$1, %eax
	testl	%eax, %eax
	je	.L21
.L20:
	movl	$1, -64(%rbp)
	cmpq	$0, -104(%rbp)
	js	.L22
	movzbl	-92(%rbp), %eax
	sarb	$4, %al
	movsbl	%al, %eax
	andl	$1, %eax
	testl	%eax, %eax
	je	.L23
	movb	$43, -71(%rbp)
	jmp	.L21
.L23:
	movb	$32, -71(%rbp)
	jmp	.L21
.L22:
	cmpb	$0, -112(%rbp)
	je	.L21
	movb	$45, -71(%rbp)
.L21:
	movb	$32, -70(%rbp)
	addq	$1, -56(%rbp)
	cmpl	$0, -108(%rbp)
	jle	.L24
	movl	-108(%rbp), %eax
	movq	-56(%rbp), %rdx
	subl	%edx, %eax
	movl	%eax, -60(%rbp)
	cmpl	$0, -60(%rbp)
	jle	.L25
	movl	-60(%rbp), %eax
	addl	%eax, -64(%rbp)
.L25:
	movzbl	-92(%rbp), %eax
	sarb	%al
	movsbl	%al, %eax
	andl	$1, %eax
	testl	%eax, %eax
	je	.L24
	movb	$48, -70(%rbp)
.L24:
	cmpl	$0, -64(%rbp)
	jle	.L26
	movq	-56(%rbp), %rax
	movl	%eax, -68(%rbp)
	jmp	.L27
.L28:
	movl	-68(%rbp), %eax
	movslq	%eax, %rdx
	movq	-16(%rbp), %rax
	addq	%rdx, %rax
	movl	-68(%rbp), %ecx
	movl	-64(%rbp), %edx
	addl	%ecx, %edx
	movslq	%edx, %rcx
	movq	-16(%rbp), %rdx
	addq	%rcx, %rdx
	movzbl	(%rax), %eax
	movb	%al, (%rdx)
	subl	$1, -68(%rbp)
.L27:
	cmpl	$0, -68(%rbp)
	jns	.L28
	movl	$0, -68(%rbp)
	jmp	.L29
.L30:
	movl	-68(%rbp), %eax
	movslq	%eax, %rdx
	movq	-16(%rbp), %rax
	addq	%rax, %rdx
	movzbl	-70(%rbp), %eax
	movb	%al, (%rdx)
	addl	$1, -68(%rbp)
.L29:
	movl	-68(%rbp), %eax
	cmpl	-64(%rbp), %eax
	jl	.L30
	cmpb	$0, -71(%rbp)
	je	.L26
	movzbl	-92(%rbp), %eax
	sarb	%al
	movsbl	%al, %eax
	andl	$1, %eax
	testl	%eax, %eax
	je	.L31
	movq	-16(%rbp), %rax
	movzbl	-71(%rbp), %edx
	movb	%dl, (%rax)
	jmp	.L26
.L31:
	movl	-64(%rbp), %eax
	cltq
	leaq	-1(%rax), %rdx
	movq	-16(%rbp), %rax
	addq	%rax, %rdx
	movzbl	-71(%rbp), %eax
	movb	%al, (%rdx)
.L26:
	movq	-88(%rbp), %rax
	movq	(%rax), %rax
	movl	-64(%rbp), %edx
	movslq	%edx, %rcx
	movq	-56(%rbp), %rdx
	addq	%rcx, %rdx
	addq	%rax, %rdx
	movq	-88(%rbp), %rax
	movq	%rdx, (%rax)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	numtostr, .-numtostr
	.globl	vsprintf
	.type	vsprintf, @function
vsprintf:
.LFB1:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$96, %rsp
	movq	%rdi, -72(%rbp)
	movq	%rsi, -80(%rbp)
	movq	%rdx, -88(%rbp)
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movq	-80(%rbp), %rax
	movq	%rax, -48(%rbp)
	movq	-72(%rbp), %rax
	movq	%rax, -56(%rbp)
	movb	$0, -61(%rbp)
	jmp	.L33
.L74:
	movq	-48(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$37, %al
	je	.L77
	movq	-48(%rbp), %rdx
	leaq	1(%rdx), %rax
	movq	%rax, -48(%rbp)
	movq	-56(%rbp), %rax
	leaq	1(%rax), %rcx
	movq	%rcx, -56(%rbp)
	movzbl	(%rdx), %edx
	movb	%dl, (%rax)
	jmp	.L33
.L78:
	nop
.L36:
	movq	-48(%rbp), %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	subl	$32, %eax
	cmpl	$16, %eax
	ja	.L37
	movl	%eax, %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L39(%rip), %rax
	movl	(%rdx,%rax), %eax
	cltq
	leaq	.L39(%rip), %rdx
	addq	%rdx, %rax
	notrack jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L39:
	.long	.L43-.L39
	.long	.L37-.L39
	.long	.L37-.L39
	.long	.L42-.L39
	.long	.L37-.L39
	.long	.L37-.L39
	.long	.L37-.L39
	.long	.L37-.L39
	.long	.L37-.L39
	.long	.L37-.L39
	.long	.L37-.L39
	.long	.L41-.L39
	.long	.L37-.L39
	.long	.L40-.L39
	.long	.L37-.L39
	.long	.L37-.L39
	.long	.L38-.L39
	.text
.L42:
	orb	$1, -61(%rbp)
	addq	$1, -48(%rbp)
	jmp	.L36
.L38:
	orb	$2, -61(%rbp)
	addq	$1, -48(%rbp)
	jmp	.L36
.L40:
	orb	$4, -61(%rbp)
	addq	$1, -48(%rbp)
	jmp	.L36
.L43:
	orb	$8, -61(%rbp)
	addq	$1, -48(%rbp)
	jmp	.L36
.L41:
	orb	$16, -61(%rbp)
	addq	$1, -48(%rbp)
	jmp	.L36
.L37:
	movl	$0, -60(%rbp)
	jmp	.L44
.L46:
	movq	-48(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -48(%rbp)
	movl	-60(%rbp), %edx
	leal	1(%rdx), %ecx
	movl	%ecx, -60(%rbp)
	movzbl	(%rax), %ecx
	movslq	%edx, %rax
	movb	%cl, -32(%rbp,%rax)
.L44:
	movq	-48(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	je	.L45
	movq	-48(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$47, %al
	jle	.L45
	movq	-48(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$57, %al
	jle	.L46
.L45:
	movl	-60(%rbp), %eax
	cltq
	movb	$0, -32(%rbp,%rax)
	movq	-48(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$46, %al
	jne	.L47
	addq	$1, -48(%rbp)
	jmp	.L48
.L50:
	movq	-48(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -48(%rbp)
	movl	-60(%rbp), %edx
	leal	1(%rdx), %ecx
	movl	%ecx, -60(%rbp)
	movzbl	(%rax), %ecx
	movslq	%edx, %rax
	movb	%cl, -32(%rbp,%rax)
.L48:
	movq	-48(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	je	.L49
	movq	-48(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$47, %al
	jle	.L49
	movq	-48(%rbp), %rax
	movzbl	(%rax), %eax
	cmpb	$57, %al
	jle	.L50
.L49:
	movl	-60(%rbp), %eax
	cltq
	movb	$0, -32(%rbp,%rax)
.L47:
	movq	-48(%rbp), %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	cmpl	$108, %eax
	jne	.L51
	addq	$1, -48(%rbp)
	nop
.L51:
	movq	-48(%rbp), %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	subl	$99, %eax
	cmpl	$21, %eax
	ja	.L33
	movl	%eax, %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L54(%rip), %rax
	movl	(%rdx,%rax), %eax
	cltq
	leaq	.L54(%rip), %rdx
	addq	%rdx, %rax
	notrack jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L54:
	.long	.L59-.L54
	.long	.L58-.L54
	.long	.L33-.L54
	.long	.L33-.L54
	.long	.L33-.L54
	.long	.L33-.L54
	.long	.L58-.L54
	.long	.L33-.L54
	.long	.L33-.L54
	.long	.L33-.L54
	.long	.L33-.L54
	.long	.L33-.L54
	.long	.L57-.L54
	.long	.L33-.L54
	.long	.L33-.L54
	.long	.L33-.L54
	.long	.L56-.L54
	.long	.L33-.L54
	.long	.L55-.L54
	.long	.L33-.L54
	.long	.L33-.L54
	.long	.L53-.L54
	.text
.L58:
	leaq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	atoi@PLT
	movl	%eax, %edi
	movq	-88(%rbp), %rax
	movl	(%rax), %eax
	cmpl	$47, %eax
	ja	.L60
	movq	-88(%rbp), %rax
	movq	16(%rax), %rdx
	movq	-88(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %eax
	addq	%rdx, %rax
	movq	-88(%rbp), %rdx
	movl	(%rdx), %edx
	leal	8(%rdx), %ecx
	movq	-88(%rbp), %rdx
	movl	%ecx, (%rdx)
	jmp	.L61
.L60:
	movq	-88(%rbp), %rax
	movq	8(%rax), %rax
	leaq	8(%rax), %rcx
	movq	-88(%rbp), %rdx
	movq	%rcx, 8(%rdx)
.L61:
	movl	(%rax), %eax
	movslq	%eax, %rdx
	movsbl	-61(%rbp), %esi
	leaq	-56(%rbp), %rax
	movl	$1, %r9d
	movl	%edi, %r8d
	movl	$10, %ecx
	movq	%rax, %rdi
	call	numtostr
	addq	$1, -48(%rbp)
	jmp	.L52
.L55:
	leaq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	atoi@PLT
	movl	%eax, %edi
	movq	-88(%rbp), %rax
	movl	(%rax), %eax
	cmpl	$47, %eax
	ja	.L62
	movq	-88(%rbp), %rax
	movq	16(%rax), %rdx
	movq	-88(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %eax
	addq	%rdx, %rax
	movq	-88(%rbp), %rdx
	movl	(%rdx), %edx
	leal	8(%rdx), %ecx
	movq	-88(%rbp), %rdx
	movl	%ecx, (%rdx)
	jmp	.L63
.L62:
	movq	-88(%rbp), %rax
	movq	8(%rax), %rax
	leaq	8(%rax), %rcx
	movq	-88(%rbp), %rdx
	movq	%rcx, 8(%rdx)
.L63:
	movl	(%rax), %eax
	movl	%eax, %edx
	movsbl	-61(%rbp), %esi
	leaq	-56(%rbp), %rax
	movl	$0, %r9d
	movl	%edi, %r8d
	movl	$10, %ecx
	movq	%rax, %rdi
	call	numtostr
	addq	$1, -48(%rbp)
	jmp	.L52
.L57:
	leaq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	atoi@PLT
	movl	%eax, %edi
	movq	-88(%rbp), %rax
	movl	(%rax), %eax
	cmpl	$47, %eax
	ja	.L64
	movq	-88(%rbp), %rax
	movq	16(%rax), %rdx
	movq	-88(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %eax
	addq	%rdx, %rax
	movq	-88(%rbp), %rdx
	movl	(%rdx), %edx
	leal	8(%rdx), %ecx
	movq	-88(%rbp), %rdx
	movl	%ecx, (%rdx)
	jmp	.L65
.L64:
	movq	-88(%rbp), %rax
	movq	8(%rax), %rax
	leaq	8(%rax), %rcx
	movq	-88(%rbp), %rdx
	movq	%rcx, 8(%rdx)
.L65:
	movl	(%rax), %eax
	movslq	%eax, %rdx
	movsbl	-61(%rbp), %esi
	leaq	-56(%rbp), %rax
	movl	$1, %r9d
	movl	%edi, %r8d
	movl	$8, %ecx
	movq	%rax, %rdi
	call	numtostr
	addq	$1, -48(%rbp)
	jmp	.L52
.L53:
	leaq	-32(%rbp), %rax
	movq	%rax, %rdi
	call	atoi@PLT
	movl	%eax, %edi
	movq	-88(%rbp), %rax
	movl	(%rax), %eax
	cmpl	$47, %eax
	ja	.L66
	movq	-88(%rbp), %rax
	movq	16(%rax), %rdx
	movq	-88(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %eax
	addq	%rdx, %rax
	movq	-88(%rbp), %rdx
	movl	(%rdx), %edx
	leal	8(%rdx), %ecx
	movq	-88(%rbp), %rdx
	movl	%ecx, (%rdx)
	jmp	.L67
.L66:
	movq	-88(%rbp), %rax
	movq	8(%rax), %rax
	leaq	8(%rax), %rcx
	movq	-88(%rbp), %rdx
	movq	%rcx, 8(%rdx)
.L67:
	movl	(%rax), %eax
	movslq	%eax, %rdx
	movsbl	-61(%rbp), %esi
	leaq	-56(%rbp), %rax
	movl	$1, %r9d
	movl	%edi, %r8d
	movl	$16, %ecx
	movq	%rax, %rdi
	call	numtostr
	addq	$1, -48(%rbp)
	jmp	.L52
.L59:
	movq	-88(%rbp), %rax
	movl	(%rax), %eax
	cmpl	$47, %eax
	ja	.L68
	movq	-88(%rbp), %rax
	movq	16(%rax), %rdx
	movq	-88(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %eax
	addq	%rdx, %rax
	movq	-88(%rbp), %rdx
	movl	(%rdx), %edx
	leal	8(%rdx), %ecx
	movq	-88(%rbp), %rdx
	movl	%ecx, (%rdx)
	jmp	.L69
.L68:
	movq	-88(%rbp), %rax
	movq	8(%rax), %rax
	leaq	8(%rax), %rcx
	movq	-88(%rbp), %rdx
	movq	%rcx, 8(%rdx)
.L69:
	movl	(%rax), %ecx
	movq	-56(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -56(%rbp)
	movl	%ecx, %edx
	movb	%dl, (%rax)
	addq	$1, -48(%rbp)
	jmp	.L52
.L56:
	movq	-88(%rbp), %rax
	movl	(%rax), %eax
	cmpl	$47, %eax
	ja	.L70
	movq	-88(%rbp), %rax
	movq	16(%rax), %rdx
	movq	-88(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %eax
	addq	%rdx, %rax
	movq	-88(%rbp), %rdx
	movl	(%rdx), %edx
	leal	8(%rdx), %ecx
	movq	-88(%rbp), %rdx
	movl	%ecx, (%rdx)
	jmp	.L71
.L70:
	movq	-88(%rbp), %rax
	movq	8(%rax), %rax
	leaq	8(%rax), %rcx
	movq	-88(%rbp), %rdx
	movq	%rcx, 8(%rdx)
.L71:
	movq	(%rax), %rax
	movq	%rax, -40(%rbp)
	jmp	.L72
.L73:
	movq	-40(%rbp), %rdx
	leaq	1(%rdx), %rax
	movq	%rax, -40(%rbp)
	movq	-56(%rbp), %rax
	leaq	1(%rax), %rcx
	movq	%rcx, -56(%rbp)
	movzbl	(%rdx), %edx
	movb	%dl, (%rax)
.L72:
	movq	-40(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L73
	addq	$1, -48(%rbp)
	nop
.L52:
	jmp	.L33
.L77:
	nop
	movq	-48(%rbp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -48(%rbp)
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L78
.L33:
	movq	-48(%rbp), %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L74
	movq	-56(%rbp), %rax
	movb	$0, (%rax)
	movq	-56(%rbp), %rax
	subq	-72(%rbp), %rax
	movq	-8(%rbp), %rsi
	xorq	%fs:40, %rsi
	je	.L76
	call	__stack_chk_fail@PLT
.L76:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	vsprintf, .-vsprintf
	.globl	sprintf
	.type	sprintf, @function
sprintf:
.LFB2:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$240, %rsp
	movq	%rdi, -232(%rbp)
	movq	%rsi, -240(%rbp)
	movq	%rdx, -160(%rbp)
	movq	%rcx, -152(%rbp)
	movq	%r8, -144(%rbp)
	movq	%r9, -136(%rbp)
	testb	%al, %al
	je	.L80
	movaps	%xmm0, -128(%rbp)
	movaps	%xmm1, -112(%rbp)
	movaps	%xmm2, -96(%rbp)
	movaps	%xmm3, -80(%rbp)
	movaps	%xmm4, -64(%rbp)
	movaps	%xmm5, -48(%rbp)
	movaps	%xmm6, -32(%rbp)
	movaps	%xmm7, -16(%rbp)
.L80:
	movq	%fs:40, %rax
	movq	%rax, -184(%rbp)
	xorl	%eax, %eax
	movl	$16, -208(%rbp)
	movl	$48, -204(%rbp)
	leaq	16(%rbp), %rax
	movq	%rax, -200(%rbp)
	leaq	-176(%rbp), %rax
	movq	%rax, -192(%rbp)
	leaq	-208(%rbp), %rdx
	movq	-240(%rbp), %rcx
	movq	-232(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	vsprintf
	movl	%eax, -212(%rbp)
	movl	-212(%rbp), %eax
	movq	-184(%rbp), %rsi
	xorq	%fs:40, %rsi
	je	.L82
	call	__stack_chk_fail@PLT
.L82:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	sprintf, .-sprintf
	.globl	_printf
	.type	_printf, @function
_printf:
.LFB3:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$1264, %rsp
	movq	%rdi, -1256(%rbp)
	movq	%rsi, -168(%rbp)
	movq	%rdx, -160(%rbp)
	movq	%rcx, -152(%rbp)
	movq	%r8, -144(%rbp)
	movq	%r9, -136(%rbp)
	testb	%al, %al
	je	.L84
	movaps	%xmm0, -128(%rbp)
	movaps	%xmm1, -112(%rbp)
	movaps	%xmm2, -96(%rbp)
	movaps	%xmm3, -80(%rbp)
	movaps	%xmm4, -64(%rbp)
	movaps	%xmm5, -48(%rbp)
	movaps	%xmm6, -32(%rbp)
	movaps	%xmm7, -16(%rbp)
.L84:
	movq	%fs:40, %rax
	movq	%rax, -184(%rbp)
	xorl	%eax, %eax
	movl	$8, -1240(%rbp)
	movl	$48, -1236(%rbp)
	leaq	16(%rbp), %rax
	movq	%rax, -1232(%rbp)
	leaq	-176(%rbp), %rax
	movq	%rax, -1224(%rbp)
	leaq	-1240(%rbp), %rdx
	movq	-1256(%rbp), %rcx
	leaq	-1216(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	vsprintf
	movl	%eax, -1244(%rbp)
	movl	$0, -1248(%rbp)
	jmp	.L85
.L87:
	leaq	-1216(%rbp), %rdx
	movl	-1248(%rbp), %eax
	cltq
	addq	%rdx, %rax
	movl	$1, %edx
	movq	%rax, %rsi
	movl	$1, %edi
	call	write@PLT
	addl	$1, -1248(%rbp)
.L85:
	movl	-1248(%rbp), %eax
	cltq
	movzbl	-1216(%rbp,%rax), %eax
	testb	%al, %al
	je	.L86
	movl	-1248(%rbp), %eax
	cmpl	-1244(%rbp), %eax
	jl	.L87
.L86:
	movl	-1248(%rbp), %eax
	movq	-184(%rbp), %rcx
	xorq	%fs:40, %rcx
	je	.L89
	call	__stack_chk_fail@PLT
.L89:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	_printf, .-_printf
	.section	.rodata
	.align 8
.LC0:
	.string	"%03i-%03i-'%03i %03i:%03i:%03i UTC\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB4:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movl	%edi, -36(%rbp)
	movl	$0, %eax
	movl	$112, %edx
#APP
# 38 "time.c" 1
	outb %al,%dx
	jmp 1f
1:	jmp 1f
1:
# 0 "" 2
#NO_APP
	movl	$113, %eax
	movl	%eax, %edx
#APP
# 38 "time.c" 1
	inb %dx,%al
	jmp 1f
1:	jmp 1f
1:
# 0 "" 2
#NO_APP
	movb	%al, -30(%rbp)
	movzbl	-30(%rbp), %eax
	movzbl	%al, %eax
	movl	%eax, -20(%rbp)
	movl	-20(%rbp), %eax
	movl	$0, %esi
	movl	%eax, %edi
	call	bcd_to_bin
	movl	%eax, -20(%rbp)
	movl	$2, %eax
	movl	$112, %edx
#APP
# 41 "time.c" 1
	outb %al,%dx
	jmp 1f
1:	jmp 1f
1:
# 0 "" 2
#NO_APP
	movl	$113, %eax
	movl	%eax, %edx
#APP
# 41 "time.c" 1
	inb %dx,%al
	jmp 1f
1:	jmp 1f
1:
# 0 "" 2
#NO_APP
	movb	%al, -29(%rbp)
	movzbl	-29(%rbp), %eax
	movzbl	%al, %eax
	movl	%eax, -16(%rbp)
	movl	-16(%rbp), %eax
	movl	$0, %esi
	movl	%eax, %edi
	call	bcd_to_bin
	movl	%eax, -16(%rbp)
	movl	$4, %eax
	movl	$112, %edx
#APP
# 44 "time.c" 1
	outb %al,%dx
	jmp 1f
1:	jmp 1f
1:
# 0 "" 2
#NO_APP
	movl	$113, %eax
	movl	%eax, %edx
#APP
# 44 "time.c" 1
	inb %dx,%al
	jmp 1f
1:	jmp 1f
1:
# 0 "" 2
#NO_APP
	movb	%al, -28(%rbp)
	movzbl	-28(%rbp), %eax
	movzbl	%al, %eax
	movl	%eax, -24(%rbp)
	movl	-24(%rbp), %eax
	movl	$1, %esi
	movl	%eax, %edi
	call	bcd_to_bin
	movl	%eax, -24(%rbp)
	movl	$7, %eax
	movl	$112, %edx
#APP
# 47 "time.c" 1
	outb %al,%dx
	jmp 1f
1:	jmp 1f
1:
# 0 "" 2
#NO_APP
	movl	$113, %eax
	movl	%eax, %edx
#APP
# 47 "time.c" 1
	inb %dx,%al
	jmp 1f
1:	jmp 1f
1:
# 0 "" 2
#NO_APP
	movb	%al, -27(%rbp)
	movzbl	-27(%rbp), %eax
	movzbl	%al, %eax
	movl	%eax, -12(%rbp)
	movl	-12(%rbp), %eax
	movl	$0, %esi
	movl	%eax, %edi
	call	bcd_to_bin
	movl	%eax, -12(%rbp)
	movl	$8, %eax
	movl	$112, %edx
#APP
# 50 "time.c" 1
	outb %al,%dx
	jmp 1f
1:	jmp 1f
1:
# 0 "" 2
#NO_APP
	movl	$113, %eax
	movl	%eax, %edx
#APP
# 50 "time.c" 1
	inb %dx,%al
	jmp 1f
1:	jmp 1f
1:
# 0 "" 2
#NO_APP
	movb	%al, -26(%rbp)
	movzbl	-26(%rbp), %eax
	movzbl	%al, %eax
	movl	%eax, -8(%rbp)
	movl	-8(%rbp), %eax
	movl	$0, %esi
	movl	%eax, %edi
	call	bcd_to_bin
	movl	%eax, -8(%rbp)
	movl	$9, %eax
	movl	$112, %edx
#APP
# 53 "time.c" 1
	outb %al,%dx
	jmp 1f
1:	jmp 1f
1:
# 0 "" 2
#NO_APP
	movl	$113, %eax
	movl	%eax, %edx
#APP
# 53 "time.c" 1
	inb %dx,%al
	jmp 1f
1:	jmp 1f
1:
# 0 "" 2
#NO_APP
	movb	%al, -25(%rbp)
	movzbl	-25(%rbp), %eax
	movzbl	%al, %eax
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	movl	$0, %esi
	movl	%eax, %edi
	call	bcd_to_bin
	movl	%eax, -4(%rbp)
	cmpl	$1, -36(%rbp)
	jne	.L91
	movl	-24(%rbp), %eax
	movl	%eax, %edi
	call	convert_24h
	movl	%eax, -24(%rbp)
.L91:
	movl	-16(%rbp), %r8d
	movl	-24(%rbp), %edi
	movl	-4(%rbp), %ecx
	movl	-8(%rbp), %edx
	movl	-12(%rbp), %eax
	subq	$8, %rsp
	movl	-20(%rbp), %esi
	pushq	%rsi
	movl	%r8d, %r9d
	movl	%edi, %r8d
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	_printf
	addq	$16, %rsp
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	main, .-main
	.globl	bcd_to_bin
	.type	bcd_to_bin, @function
bcd_to_bin:
.LFB5:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	movl	-4(%rbp), %eax
	andl	$15, %eax
	movl	%eax, %ecx
	movl	-4(%rbp), %eax
	leal	15(%rax), %edx
	testl	%eax, %eax
	cmovs	%edx, %eax
	sarl	$4, %eax
	movl	%eax, %edx
	movl	%edx, %eax
	sall	$2, %eax
	addl	%edx, %eax
	addl	%eax, %eax
	addl	%ecx, %eax
	movl	%eax, -4(%rbp)
	cmpl	$0, -8(%rbp)
	jne	.L93
	movl	-4(%rbp), %eax
	jmp	.L94
.L93:
	movl	-4(%rbp), %eax
.L94:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	bcd_to_bin, .-bcd_to_bin
	.globl	convert_24h
	.type	convert_24h, @function
convert_24h:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -20(%rbp)
	movl	$11, %eax
	movl	$112, %edx
#APP
# 73 "time.c" 1
	outb %al,%dx
	jmp 1f
1:	jmp 1f
1:
# 0 "" 2
#NO_APP
	movl	$113, %eax
	movl	%eax, %edx
#APP
# 73 "time.c" 1
	inb %dx,%al
	jmp 1f
1:	jmp 1f
1:
# 0 "" 2
#NO_APP
	movb	%al, -1(%rbp)
	movzbl	-1(%rbp), %eax
	movzbl	%al, %eax
	andl	$2, %eax
	testl	%eax, %eax
	jne	.L96
	movl	-20(%rbp), %eax
	andl	$128, %eax
	testl	%eax, %eax
	je	.L96
	movl	-20(%rbp), %eax
	andl	$127, %eax
	leal	12(%rax), %ecx
	movslq	%ecx, %rax
	imulq	$715827883, %rax, %rax
	shrq	$32, %rax
	movl	%eax, %edx
	sarl	$2, %edx
	movl	%ecx, %eax
	sarl	$31, %eax
	subl	%eax, %edx
	movl	%edx, %eax
	addl	%eax, %eax
	addl	%edx, %eax
	sall	$3, %eax
	subl	%eax, %ecx
	movl	%ecx, %edx
	movl	%edx, %eax
	jmp	.L95
.L96:
.L95:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	convert_24h, .-convert_24h
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
