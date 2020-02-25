	.file	"tarc.c"
	.text
.Ltext0:
	.section	.rodata
.LC0:
	.string	"/"
	.text
	.globl	get_dir_suffix
	.type	get_dir_suffix, @function
get_dir_suffix:
.LFB2:
	.file 1 "tarc.c"
	.loc 1 30 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	.loc 1 31 0
	movq	-24(%rbp), %rax
	movl	$.LC0, %esi
	movq	%rax, %rdi
	call	strtok
	movq	%rax, -8(%rbp)
	.loc 1 35 0
	jmp	.L2
.L3:
	.loc 1 36 0
	movq	-8(%rbp), %rax
	movq	%rax, -16(%rbp)
	.loc 1 37 0
	movl	$.LC0, %esi
	movl	$0, %edi
	call	strtok
	movq	%rax, -8(%rbp)
.L2:
	.loc 1 35 0 discriminator 1
	cmpq	$0, -8(%rbp)
	jne	.L3
	.loc 1 41 0
	movq	-16(%rbp), %rax
	.loc 1 42 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	get_dir_suffix, .-get_dir_suffix
	.section	.rodata
	.align 8
.LC1:
	.string	"%s: No such file or directory\n"
.LC2:
	.string	"r"
	.text
	.globl	print_file
	.type	print_file, @function
print_file:
.LFB3:
	.loc 1 52 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	subq	$232, %rsp
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	movq	%rdi, -248(%rbp)
	movq	%rsi, -256(%rbp)
	movq	%rdx, -264(%rbp)
	movq	%rsp, %rax
	movq	%rax, %rbx
	.loc 1 55 0
	leaq	-224(%rbp), %rdx
	movq	-248(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	stat
	cmpl	$-1, %eax
	jne	.L6
	.loc 1 56 0
	movq	stderr(%rip), %rax
	movq	-248(%rbp), %rdx
	movl	$.LC1, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf
	.loc 1 57 0
	movl	$1, %edi
	call	exit
.L6:
	.loc 1 58 0
	movl	-200(%rbp), %eax
	andl	$61440, %eax
	cmpl	$40960, %eax
	jne	.L7
	.loc 1 60 0
	movl	$1, %eax
	jmp	.L8
.L7:
	.loc 1 64 0
	movq	-256(%rbp), %rax
	movq	%rax, %rdi
	call	strlen
	movl	%eax, -228(%rbp)
	.loc 1 65 0
	movq	stdout(%rip), %rdx
	leaq	-228(%rbp), %rax
	movq	%rdx, %rcx
	movl	$1, %edx
	movl	$4, %esi
	movq	%rax, %rdi
	call	fwrite
	.loc 1 66 0
	movq	stdout(%rip), %rcx
	movl	-228(%rbp), %eax
	movslq	%eax, %rdx
	movq	-256(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	fwrite
	.loc 1 69 0
	movq	stdout(%rip), %rax
	leaq	-224(%rbp), %rdx
	leaq	8(%rdx), %rdi
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$8, %esi
	call	fwrite
	.loc 1 72 0
	movq	-216(%rbp), %rax
	movl	%eax, %edx
	movq	-264(%rbp), %rax
	movl	%edx, %esi
	movq	%rax, %rdi
	call	jrb_find_int
	testq	%rax, %rax
	je	.L9
	.loc 1 73 0
	movl	$0, %eax
	jmp	.L8
.L9:
	.loc 1 75 0
	movq	-248(%rbp), %rax
	movq	%rax, %rdi
	call	new_jval_s
	movq	%rax, %rdx
	movq	-216(%rbp), %rax
	movl	%eax, %ecx
	movq	-264(%rbp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	call	jrb_insert_int
	.loc 1 78 0
	movq	stdout(%rip), %rax
	leaq	-224(%rbp), %rdx
	leaq	24(%rdx), %rdi
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$4, %esi
	call	fwrite
	.loc 1 79 0
	movq	stdout(%rip), %rax
	leaq	-224(%rbp), %rdx
	leaq	88(%rdx), %rdi
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$8, %esi
	call	fwrite
	.loc 1 82 0
	movl	-200(%rbp), %eax
	andl	$61440, %eax
	cmpl	$16384, %eax
	jne	.L10
	.loc 1 83 0
	movl	$0, %eax
	jmp	.L8
.L10:
	.loc 1 87 0
	movq	-176(%rbp), %rax
	leaq	-1(%rax), %rdx
	movq	%rdx, -56(%rbp)
	movq	%rax, %rdx
	movq	%rdx, %r14
	movl	$0, %r15d
	movq	%rax, %rdx
	movq	%rdx, %r12
	movl	$0, %r13d
	movl	$16, %edx
	subq	$1, %rdx
	addq	%rdx, %rax
	movl	$16, %esi
	movl	$0, %edx
	divq	%rsi
	imulq	$16, %rax, %rax
	subq	%rax, %rsp
	movq	%rsp, %rax
	addq	$0, %rax
	movq	%rax, -64(%rbp)
	.loc 1 88 0
	movq	-248(%rbp), %rax
	movl	$.LC2, %esi
	movq	%rax, %rdi
	call	fopen
	movq	%rax, -72(%rbp)
	.loc 1 89 0
	cmpq	$0, -72(%rbp)
	jne	.L11
	.loc 1 90 0
	movl	$1, %edi
	call	exit
.L11:
	.loc 1 94 0
	movq	-176(%rbp), %rax
	movq	%rax, %rdx
	movq	-64(%rbp), %rax
	movq	-72(%rbp), %rcx
	movl	$1, %esi
	movq	%rax, %rdi
	call	fread
	.loc 1 95 0
	movq	-72(%rbp), %rax
	movq	%rax, %rdi
	call	fclose
	.loc 1 98 0
	movq	stdout(%rip), %rax
	leaq	-224(%rbp), %rdx
	leaq	48(%rdx), %rdi
	movq	%rax, %rcx
	movl	$1, %edx
	movl	$8, %esi
	call	fwrite
	.loc 1 99 0
	movq	stdout(%rip), %rcx
	movq	-176(%rbp), %rax
	movq	%rax, %rdx
	movq	-64(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	fwrite
	.loc 1 102 0
	movl	$0, %eax
.L8:
	movq	%rbx, %rsp
	.loc 1 103 0
	leaq	-40(%rbp), %rsp
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	print_file, .-print_file
	.section	.rodata
.LC3:
	.string	"."
.LC4:
	.string	".."
.LC5:
	.string	"%s/%s"
	.text
	.globl	iterate_dir
	.type	iterate_dir, @function
iterate_dir:
.LFB4:
	.loc 1 112 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$720, %rsp
	movq	%rdi, -712(%rbp)
	movq	%rsi, -720(%rbp)
	.loc 1 114 0
	movq	-712(%rbp), %rax
	movq	%rax, %rdi
	call	opendir
	movq	%rax, -16(%rbp)
	.loc 1 116 0
	cmpq	$0, -16(%rbp)
	jne	.L14
	.loc 1 117 0
	movq	-712(%rbp), %rdx
	movq	stderr(%rip), %rax
	movl	$.LC1, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf
	.loc 1 118 0
	movl	$1, %edi
	call	exit
.L14:
	.loc 1 121 0
	movl	$0, %eax
	call	new_dllist
	movq	%rax, -24(%rbp)
	.loc 1 127 0
	jmp	.L15
.L21:
.LBB2:
	.loc 1 129 0
	movq	-32(%rbp), %rax
	addq	$19, %rax
	movl	$.LC3, %esi
	movq	%rax, %rdi
	call	strcmp
	testl	%eax, %eax
	je	.L16
	.loc 1 129 0 is_stmt 0 discriminator 1
	movq	-32(%rbp), %rax
	addq	$19, %rax
	movl	$.LC4, %esi
	movq	%rax, %rdi
	call	strcmp
	testl	%eax, %eax
	jne	.L17
.L16:
	.loc 1 130 0 is_stmt 1
	jmp	.L15
.L17:
	.loc 1 133 0
	movq	-32(%rbp), %rax
	leaq	19(%rax), %rcx
	movq	-712(%rbp), %rdx
	leaq	-304(%rbp), %rax
	movl	$.LC5, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	sprintf
	.loc 1 134 0
	movq	-32(%rbp), %rax
	leaq	19(%rax), %rcx
	movq	-712(%rbp), %rax
	leaq	256(%rax), %rdx
	leaq	-560(%rbp), %rax
	movl	$.LC5, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	sprintf
	.loc 1 137 0
	movq	-720(%rbp), %rdx
	leaq	-560(%rbp), %rcx
	leaq	-304(%rbp), %rax
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	print_file
	.loc 1 140 0
	leaq	-704(%rbp), %rdx
	leaq	-304(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	stat
	cmpl	$-1, %eax
	jne	.L19
	.loc 1 141 0
	jmp	.L15
.L19:
	.loc 1 144 0
	movl	-680(%rbp), %eax
	andl	$61440, %eax
	cmpl	$16384, %eax
	jne	.L15
.LBB3:
	.loc 1 145 0
	movl	$512, %edi
	call	malloc
	movq	%rax, -40(%rbp)
	.loc 1 147 0
	movq	-40(%rbp), %rax
	leaq	256(%rax), %rdx
	leaq	-560(%rbp), %rax
	movq	%rax, %rsi
	movq	%rdx, %rdi
	call	strcpy
	.loc 1 148 0
	movq	-40(%rbp), %rax
	leaq	-304(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcpy
	.loc 1 149 0
	movq	-40(%rbp), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	new_jval_v
	movq	%rax, %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	dll_append
.L15:
.LBE3:
.LBE2:
	.loc 1 127 0 discriminator 1
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	readdir
	movq	%rax, -32(%rbp)
	cmpq	$0, -32(%rbp)
	jne	.L21
	.loc 1 153 0
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	closedir
	.loc 1 157 0
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
	jmp	.L22
.L23:
.LBB4:
	.loc 1 158 0 discriminator 2
	movq	-8(%rbp), %rax
	movq	16(%rax), %rax
	movq	%rax, -48(%rbp)
	.loc 1 159 0 discriminator 2
	movq	-720(%rbp), %rdx
	movq	-48(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	iterate_dir
	.loc 1 160 0 discriminator 2
	movq	-48(%rbp), %rax
	movq	%rax, %rdi
	call	free
.LBE4:
	.loc 1 157 0 discriminator 2
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -8(%rbp)
.L22:
	.loc 1 157 0 is_stmt 0 discriminator 1
	movq	-8(%rbp), %rax
	cmpq	-24(%rbp), %rax
	jne	.L23
	.loc 1 163 0 is_stmt 1
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	free_dllist
	.loc 1 164 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	iterate_dir, .-iterate_dir
	.section	.rodata
.LC6:
	.string	"usage: tarc directory\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB5:
	.loc 1 166 0
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$304, %rsp
	movl	%edi, -292(%rbp)
	movq	%rsi, -304(%rbp)
	.loc 1 167 0
	cmpl	$2, -292(%rbp)
	je	.L25
	.loc 1 168 0
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$22, %edx
	movl	$1, %esi
	movl	$.LC6, %edi
	call	fwrite
	.loc 1 169 0
	movl	$1, %edi
	call	exit
.L25:
	.loc 1 172 0
	movq	-304(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	%rax, %rdi
	call	strdup
	movq	%rax, -8(%rbp)
	.loc 1 174 0
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	get_dir_suffix
	movq	%rax, %rdx
	leaq	-288(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcpy
	.loc 1 176 0
	movl	$0, %eax
	call	make_jrb
	movq	%rax, -16(%rbp)
	.loc 1 179 0
	movq	-304(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rax
	movq	-16(%rbp), %rdx
	leaq	-288(%rbp), %rcx
	movq	%rcx, %rsi
	movq	%rax, %rdi
	call	print_file
	testl	%eax, %eax
	je	.L26
	.loc 1 180 0
	movl	$1, %edi
	call	exit
.L26:
	.loc 1 183 0
	movl	$512, %edi
	call	malloc
	movq	%rax, -24(%rbp)
	.loc 1 184 0
	movq	-304(%rbp), %rax
	addq	$8, %rax
	movq	(%rax), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcpy
	.loc 1 185 0
	movq	-24(%rbp), %rax
	leaq	256(%rax), %rdx
	leaq	-288(%rbp), %rax
	movq	%rax, %rsi
	movq	%rdx, %rdi
	call	strcpy
	.loc 1 188 0
	movq	-16(%rbp), %rdx
	movq	-24(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	iterate_dir
	.loc 1 190 0
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	free
	.loc 1 191 0
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	free
	.loc 1 192 0
	movq	-16(%rbp), %rax
	movq	%rax, %rdi
	call	jrb_free_tree
	.loc 1 193 0
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	main, .-main
.Letext0:
	.file 2 "/usr/lib/gcc/x86_64-redhat-linux/4.8.5/include/stddef.h"
	.file 3 "/usr/include/bits/types.h"
	.file 4 "/usr/include/time.h"
	.file 5 "/usr/include/stdio.h"
	.file 6 "/usr/include/libio.h"
	.file 7 "/usr/include/bits/dirent.h"
	.file 8 "/usr/include/bits/stat.h"
	.file 9 "/home/jplank/cs360/include/jval.h"
	.file 10 "/home/jplank/cs360/include/jrb.h"
	.file 11 "/usr/include/dirent.h"
	.file 12 "/home/jplank/cs360/include/dllist.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x8ee
	.value	0x4
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF119
	.byte	0x1
	.long	.LASF120
	.long	.LASF121
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.long	.LASF7
	.byte	0x2
	.byte	0xd4
	.long	0x38
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.long	.LASF0
	.uleb128 0x4
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x3
	.byte	0x1
	.byte	0x8
	.long	.LASF1
	.uleb128 0x3
	.byte	0x2
	.byte	0x7
	.long	.LASF2
	.uleb128 0x3
	.byte	0x4
	.byte	0x7
	.long	.LASF3
	.uleb128 0x3
	.byte	0x1
	.byte	0x6
	.long	.LASF4
	.uleb128 0x3
	.byte	0x2
	.byte	0x5
	.long	.LASF5
	.uleb128 0x3
	.byte	0x8
	.byte	0x5
	.long	.LASF6
	.uleb128 0x2
	.long	.LASF8
	.byte	0x3
	.byte	0x85
	.long	0x38
	.uleb128 0x2
	.long	.LASF9
	.byte	0x3
	.byte	0x86
	.long	0x54
	.uleb128 0x2
	.long	.LASF10
	.byte	0x3
	.byte	0x87
	.long	0x54
	.uleb128 0x2
	.long	.LASF11
	.byte	0x3
	.byte	0x88
	.long	0x38
	.uleb128 0x2
	.long	.LASF12
	.byte	0x3
	.byte	0x8a
	.long	0x54
	.uleb128 0x2
	.long	.LASF13
	.byte	0x3
	.byte	0x8b
	.long	0x38
	.uleb128 0x2
	.long	.LASF14
	.byte	0x3
	.byte	0x8c
	.long	0x69
	.uleb128 0x2
	.long	.LASF15
	.byte	0x3
	.byte	0x8d
	.long	0x69
	.uleb128 0x5
	.long	0x3f
	.long	0xd8
	.uleb128 0x6
	.long	0xd8
	.byte	0x1
	.byte	0
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.long	.LASF16
	.uleb128 0x2
	.long	.LASF17
	.byte	0x3
	.byte	0x94
	.long	0x69
	.uleb128 0x7
	.byte	0x8
	.uleb128 0x2
	.long	.LASF18
	.byte	0x3
	.byte	0xa2
	.long	0x69
	.uleb128 0x2
	.long	.LASF19
	.byte	0x3
	.byte	0xa7
	.long	0x69
	.uleb128 0x2
	.long	.LASF20
	.byte	0x3
	.byte	0xb8
	.long	0x69
	.uleb128 0x8
	.byte	0x8
	.long	0x113
	.uleb128 0x3
	.byte	0x1
	.byte	0x6
	.long	.LASF21
	.uleb128 0x3
	.byte	0x8
	.byte	0x5
	.long	.LASF22
	.uleb128 0x9
	.long	.LASF27
	.byte	0x10
	.byte	0x4
	.byte	0x78
	.long	0x146
	.uleb128 0xa
	.long	.LASF23
	.byte	0x4
	.byte	0x7a
	.long	0xdf
	.byte	0
	.uleb128 0xa
	.long	.LASF24
	.byte	0x4
	.byte	0x7b
	.long	0x102
	.byte	0x8
	.byte	0
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.long	.LASF25
	.uleb128 0x5
	.long	0x113
	.long	0x15d
	.uleb128 0x6
	.long	0xd8
	.byte	0x7
	.byte	0
	.uleb128 0x2
	.long	.LASF26
	.byte	0x5
	.byte	0x30
	.long	0x168
	.uleb128 0x9
	.long	.LASF28
	.byte	0xd8
	.byte	0x6
	.byte	0xf6
	.long	0x2e9
	.uleb128 0xa
	.long	.LASF29
	.byte	0x6
	.byte	0xf7
	.long	0x3f
	.byte	0
	.uleb128 0xa
	.long	.LASF30
	.byte	0x6
	.byte	0xfc
	.long	0x10d
	.byte	0x8
	.uleb128 0xa
	.long	.LASF31
	.byte	0x6
	.byte	0xfd
	.long	0x10d
	.byte	0x10
	.uleb128 0xa
	.long	.LASF32
	.byte	0x6
	.byte	0xfe
	.long	0x10d
	.byte	0x18
	.uleb128 0xa
	.long	.LASF33
	.byte	0x6
	.byte	0xff
	.long	0x10d
	.byte	0x20
	.uleb128 0xb
	.long	.LASF34
	.byte	0x6
	.value	0x100
	.long	0x10d
	.byte	0x28
	.uleb128 0xb
	.long	.LASF35
	.byte	0x6
	.value	0x101
	.long	0x10d
	.byte	0x30
	.uleb128 0xb
	.long	.LASF36
	.byte	0x6
	.value	0x102
	.long	0x10d
	.byte	0x38
	.uleb128 0xb
	.long	.LASF37
	.byte	0x6
	.value	0x103
	.long	0x10d
	.byte	0x40
	.uleb128 0xb
	.long	.LASF38
	.byte	0x6
	.value	0x105
	.long	0x10d
	.byte	0x48
	.uleb128 0xb
	.long	.LASF39
	.byte	0x6
	.value	0x106
	.long	0x10d
	.byte	0x50
	.uleb128 0xb
	.long	.LASF40
	.byte	0x6
	.value	0x107
	.long	0x10d
	.byte	0x58
	.uleb128 0xb
	.long	.LASF41
	.byte	0x6
	.value	0x109
	.long	0x321
	.byte	0x60
	.uleb128 0xb
	.long	.LASF42
	.byte	0x6
	.value	0x10b
	.long	0x327
	.byte	0x68
	.uleb128 0xb
	.long	.LASF43
	.byte	0x6
	.value	0x10d
	.long	0x3f
	.byte	0x70
	.uleb128 0xb
	.long	.LASF44
	.byte	0x6
	.value	0x111
	.long	0x3f
	.byte	0x74
	.uleb128 0xb
	.long	.LASF45
	.byte	0x6
	.value	0x113
	.long	0xb2
	.byte	0x78
	.uleb128 0xb
	.long	.LASF46
	.byte	0x6
	.value	0x117
	.long	0x4d
	.byte	0x80
	.uleb128 0xb
	.long	.LASF47
	.byte	0x6
	.value	0x118
	.long	0x5b
	.byte	0x82
	.uleb128 0xb
	.long	.LASF48
	.byte	0x6
	.value	0x119
	.long	0x32d
	.byte	0x83
	.uleb128 0xb
	.long	.LASF49
	.byte	0x6
	.value	0x11d
	.long	0x33d
	.byte	0x88
	.uleb128 0xb
	.long	.LASF50
	.byte	0x6
	.value	0x126
	.long	0xbd
	.byte	0x90
	.uleb128 0xb
	.long	.LASF51
	.byte	0x6
	.value	0x12f
	.long	0xea
	.byte	0x98
	.uleb128 0xb
	.long	.LASF52
	.byte	0x6
	.value	0x130
	.long	0xea
	.byte	0xa0
	.uleb128 0xb
	.long	.LASF53
	.byte	0x6
	.value	0x131
	.long	0xea
	.byte	0xa8
	.uleb128 0xb
	.long	.LASF54
	.byte	0x6
	.value	0x132
	.long	0xea
	.byte	0xb0
	.uleb128 0xb
	.long	.LASF55
	.byte	0x6
	.value	0x133
	.long	0x2d
	.byte	0xb8
	.uleb128 0xb
	.long	.LASF56
	.byte	0x6
	.value	0x135
	.long	0x3f
	.byte	0xc0
	.uleb128 0xb
	.long	.LASF57
	.byte	0x6
	.value	0x137
	.long	0x343
	.byte	0xc4
	.byte	0
	.uleb128 0xc
	.long	.LASF122
	.byte	0x6
	.byte	0x9b
	.uleb128 0x9
	.long	.LASF58
	.byte	0x18
	.byte	0x6
	.byte	0xa1
	.long	0x321
	.uleb128 0xa
	.long	.LASF59
	.byte	0x6
	.byte	0xa2
	.long	0x321
	.byte	0
	.uleb128 0xa
	.long	.LASF60
	.byte	0x6
	.byte	0xa3
	.long	0x327
	.byte	0x8
	.uleb128 0xa
	.long	.LASF61
	.byte	0x6
	.byte	0xa7
	.long	0x3f
	.byte	0x10
	.byte	0
	.uleb128 0x8
	.byte	0x8
	.long	0x2f0
	.uleb128 0x8
	.byte	0x8
	.long	0x168
	.uleb128 0x5
	.long	0x113
	.long	0x33d
	.uleb128 0x6
	.long	0xd8
	.byte	0
	.byte	0
	.uleb128 0x8
	.byte	0x8
	.long	0x2e9
	.uleb128 0x5
	.long	0x113
	.long	0x353
	.uleb128 0x6
	.long	0xd8
	.byte	0x13
	.byte	0
	.uleb128 0xd
	.long	.LASF62
	.value	0x118
	.byte	0x7
	.byte	0x16
	.long	0x39d
	.uleb128 0xa
	.long	.LASF63
	.byte	0x7
	.byte	0x19
	.long	0x91
	.byte	0
	.uleb128 0xa
	.long	.LASF64
	.byte	0x7
	.byte	0x1a
	.long	0xb2
	.byte	0x8
	.uleb128 0xa
	.long	.LASF65
	.byte	0x7
	.byte	0x1f
	.long	0x4d
	.byte	0x10
	.uleb128 0xa
	.long	.LASF66
	.byte	0x7
	.byte	0x20
	.long	0x46
	.byte	0x12
	.uleb128 0xa
	.long	.LASF67
	.byte	0x7
	.byte	0x21
	.long	0x39d
	.byte	0x13
	.byte	0
	.uleb128 0x5
	.long	0x113
	.long	0x3ad
	.uleb128 0x6
	.long	0xd8
	.byte	0xff
	.byte	0
	.uleb128 0xe
	.string	"DIR"
	.byte	0xb
	.byte	0x80
	.long	0x3b8
	.uleb128 0xf
	.long	.LASF123
	.uleb128 0x9
	.long	.LASF68
	.byte	0x90
	.byte	0x8
	.byte	0x2e
	.long	0x47e
	.uleb128 0xa
	.long	.LASF69
	.byte	0x8
	.byte	0x30
	.long	0x70
	.byte	0
	.uleb128 0xa
	.long	.LASF70
	.byte	0x8
	.byte	0x35
	.long	0x91
	.byte	0x8
	.uleb128 0xa
	.long	.LASF71
	.byte	0x8
	.byte	0x3d
	.long	0xa7
	.byte	0x10
	.uleb128 0xa
	.long	.LASF72
	.byte	0x8
	.byte	0x3e
	.long	0x9c
	.byte	0x18
	.uleb128 0xa
	.long	.LASF73
	.byte	0x8
	.byte	0x40
	.long	0x7b
	.byte	0x1c
	.uleb128 0xa
	.long	.LASF74
	.byte	0x8
	.byte	0x41
	.long	0x86
	.byte	0x20
	.uleb128 0xa
	.long	.LASF75
	.byte	0x8
	.byte	0x43
	.long	0x3f
	.byte	0x24
	.uleb128 0xa
	.long	.LASF76
	.byte	0x8
	.byte	0x45
	.long	0x70
	.byte	0x28
	.uleb128 0xa
	.long	.LASF77
	.byte	0x8
	.byte	0x4a
	.long	0xb2
	.byte	0x30
	.uleb128 0xa
	.long	.LASF78
	.byte	0x8
	.byte	0x4e
	.long	0xec
	.byte	0x38
	.uleb128 0xa
	.long	.LASF79
	.byte	0x8
	.byte	0x50
	.long	0xf7
	.byte	0x40
	.uleb128 0xa
	.long	.LASF80
	.byte	0x8
	.byte	0x5b
	.long	0x121
	.byte	0x48
	.uleb128 0xa
	.long	.LASF81
	.byte	0x8
	.byte	0x5c
	.long	0x121
	.byte	0x58
	.uleb128 0xa
	.long	.LASF82
	.byte	0x8
	.byte	0x5d
	.long	0x121
	.byte	0x68
	.uleb128 0xa
	.long	.LASF83
	.byte	0x8
	.byte	0x6a
	.long	0x47e
	.byte	0x78
	.byte	0
	.uleb128 0x5
	.long	0x102
	.long	0x48e
	.uleb128 0x6
	.long	0xd8
	.byte	0x2
	.byte	0
	.uleb128 0x10
	.byte	0x8
	.byte	0x9
	.byte	0x2a
	.long	0x52b
	.uleb128 0x11
	.string	"i"
	.byte	0x9
	.byte	0x2b
	.long	0x3f
	.uleb128 0x11
	.string	"l"
	.byte	0x9
	.byte	0x2c
	.long	0x69
	.uleb128 0x11
	.string	"f"
	.byte	0x9
	.byte	0x2d
	.long	0x52b
	.uleb128 0x11
	.string	"d"
	.byte	0x9
	.byte	0x2e
	.long	0x532
	.uleb128 0x11
	.string	"v"
	.byte	0x9
	.byte	0x2f
	.long	0xea
	.uleb128 0x11
	.string	"s"
	.byte	0x9
	.byte	0x30
	.long	0x10d
	.uleb128 0x11
	.string	"c"
	.byte	0x9
	.byte	0x31
	.long	0x113
	.uleb128 0x11
	.string	"uc"
	.byte	0x9
	.byte	0x32
	.long	0x46
	.uleb128 0x11
	.string	"sh"
	.byte	0x9
	.byte	0x33
	.long	0x62
	.uleb128 0x11
	.string	"ush"
	.byte	0x9
	.byte	0x34
	.long	0x4d
	.uleb128 0x11
	.string	"ui"
	.byte	0x9
	.byte	0x35
	.long	0x54
	.uleb128 0x12
	.long	.LASF84
	.byte	0x9
	.byte	0x36
	.long	0xc8
	.uleb128 0x12
	.long	.LASF85
	.byte	0x9
	.byte	0x37
	.long	0x539
	.uleb128 0x12
	.long	.LASF86
	.byte	0x9
	.byte	0x38
	.long	0x14d
	.uleb128 0x12
	.long	.LASF87
	.byte	0x9
	.byte	0x39
	.long	0x549
	.byte	0
	.uleb128 0x3
	.byte	0x4
	.byte	0x4
	.long	.LASF88
	.uleb128 0x3
	.byte	0x8
	.byte	0x4
	.long	.LASF89
	.uleb128 0x5
	.long	0x52b
	.long	0x549
	.uleb128 0x6
	.long	0xd8
	.byte	0x1
	.byte	0
	.uleb128 0x5
	.long	0x46
	.long	0x559
	.uleb128 0x6
	.long	0xd8
	.byte	0x7
	.byte	0
	.uleb128 0x2
	.long	.LASF90
	.byte	0x9
	.byte	0x3a
	.long	0x48e
	.uleb128 0x9
	.long	.LASF91
	.byte	0x30
	.byte	0xa
	.byte	0x32
	.long	0x5dd
	.uleb128 0x13
	.string	"red"
	.byte	0xa
	.byte	0x33
	.long	0x46
	.byte	0
	.uleb128 0xa
	.long	.LASF92
	.byte	0xa
	.byte	0x34
	.long	0x46
	.byte	0x1
	.uleb128 0xa
	.long	.LASF93
	.byte	0xa
	.byte	0x35
	.long	0x46
	.byte	0x2
	.uleb128 0xa
	.long	.LASF94
	.byte	0xa
	.byte	0x36
	.long	0x46
	.byte	0x3
	.uleb128 0xa
	.long	.LASF95
	.byte	0xa
	.byte	0x37
	.long	0x5dd
	.byte	0x8
	.uleb128 0xa
	.long	.LASF96
	.byte	0xa
	.byte	0x38
	.long	0x5dd
	.byte	0x10
	.uleb128 0xa
	.long	.LASF97
	.byte	0xa
	.byte	0x39
	.long	0x5dd
	.byte	0x18
	.uleb128 0x13
	.string	"key"
	.byte	0xa
	.byte	0x3a
	.long	0x559
	.byte	0x20
	.uleb128 0x13
	.string	"val"
	.byte	0xa
	.byte	0x3b
	.long	0x559
	.byte	0x28
	.byte	0
	.uleb128 0x8
	.byte	0x8
	.long	0x564
	.uleb128 0xe
	.string	"JRB"
	.byte	0xa
	.byte	0x3c
	.long	0x5dd
	.uleb128 0x9
	.long	.LASF98
	.byte	0x18
	.byte	0xc
	.byte	0x2a
	.long	0x61f
	.uleb128 0xa
	.long	.LASF95
	.byte	0xc
	.byte	0x2b
	.long	0x61f
	.byte	0
	.uleb128 0xa
	.long	.LASF96
	.byte	0xc
	.byte	0x2c
	.long	0x61f
	.byte	0x8
	.uleb128 0x13
	.string	"val"
	.byte	0xc
	.byte	0x2d
	.long	0x559
	.byte	0x10
	.byte	0
	.uleb128 0x8
	.byte	0x8
	.long	0x5ee
	.uleb128 0x2
	.long	.LASF99
	.byte	0xc
	.byte	0x2e
	.long	0x61f
	.uleb128 0xd
	.long	.LASF100
	.value	0x200
	.byte	0x1
	.byte	0x12
	.long	0x657
	.uleb128 0xa
	.long	.LASF101
	.byte	0x1
	.byte	0x13
	.long	0x39d
	.byte	0
	.uleb128 0x14
	.long	.LASF102
	.byte	0x1
	.byte	0x14
	.long	0x39d
	.value	0x100
	.byte	0
	.uleb128 0x2
	.long	.LASF100
	.byte	0x1
	.byte	0x15
	.long	0x630
	.uleb128 0x15
	.long	.LASF105
	.byte	0x1
	.byte	0x1e
	.long	0x10d
	.quad	.LFB2
	.quad	.LFE2-.LFB2
	.uleb128 0x1
	.byte	0x9c
	.long	0x6ae
	.uleb128 0x16
	.string	"dir"
	.byte	0x1
	.byte	0x1e
	.long	0x10d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x17
	.long	.LASF103
	.byte	0x1
	.byte	0x1f
	.long	0x10d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x17
	.long	.LASF104
	.byte	0x1
	.byte	0x20
	.long	0x10d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0x15
	.long	.LASF106
	.byte	0x1
	.byte	0x34
	.long	0x3f
	.quad	.LFB3
	.quad	.LFE3-.LFB3
	.uleb128 0x1
	.byte	0x9c
	.long	0x73a
	.uleb128 0x18
	.long	.LASF101
	.byte	0x1
	.byte	0x34
	.long	0x10d
	.uleb128 0x3
	.byte	0x91
	.sleb128 -264
	.uleb128 0x18
	.long	.LASF102
	.byte	0x1
	.byte	0x34
	.long	0x10d
	.uleb128 0x3
	.byte	0x91
	.sleb128 -272
	.uleb128 0x18
	.long	.LASF107
	.byte	0x1
	.byte	0x34
	.long	0x5e3
	.uleb128 0x3
	.byte	0x91
	.sleb128 -280
	.uleb128 0x17
	.long	.LASF108
	.byte	0x1
	.byte	0x36
	.long	0x3bd
	.uleb128 0x3
	.byte	0x91
	.sleb128 -240
	.uleb128 0x17
	.long	.LASF109
	.byte	0x1
	.byte	0x40
	.long	0x3f
	.uleb128 0x3
	.byte	0x91
	.sleb128 -244
	.uleb128 0x19
	.string	"buf"
	.byte	0x1
	.byte	0x57
	.long	0x73a
	.uleb128 0x4
	.byte	0x91
	.sleb128 -80
	.byte	0x6
	.uleb128 0x17
	.long	.LASF110
	.byte	0x1
	.byte	0x58
	.long	0x74e
	.uleb128 0x3
	.byte	0x91
	.sleb128 -88
	.byte	0
	.uleb128 0x5
	.long	0x46
	.long	0x74e
	.uleb128 0x1a
	.long	0xd8
	.uleb128 0x4
	.byte	0x91
	.sleb128 -72
	.byte	0x6
	.byte	0
	.uleb128 0x8
	.byte	0x8
	.long	0x15d
	.uleb128 0x1b
	.long	.LASF124
	.byte	0x1
	.byte	0x70
	.quad	.LFB4
	.quad	.LFE4-.LFB4
	.uleb128 0x1
	.byte	0x9c
	.long	0x84a
	.uleb128 0x18
	.long	.LASF111
	.byte	0x1
	.byte	0x70
	.long	0x84a
	.uleb128 0x3
	.byte	0x91
	.sleb128 -728
	.uleb128 0x18
	.long	.LASF107
	.byte	0x1
	.byte	0x70
	.long	0x5e3
	.uleb128 0x3
	.byte	0x91
	.sleb128 -736
	.uleb128 0x19
	.string	"dp"
	.byte	0x1
	.byte	0x71
	.long	0x850
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x19
	.string	"dfd"
	.byte	0x1
	.byte	0x72
	.long	0x856
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x17
	.long	.LASF112
	.byte	0x1
	.byte	0x79
	.long	0x625
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x17
	.long	.LASF101
	.byte	0x1
	.byte	0x7c
	.long	0x39d
	.uleb128 0x3
	.byte	0x91
	.sleb128 -320
	.uleb128 0x17
	.long	.LASF102
	.byte	0x1
	.byte	0x7d
	.long	0x39d
	.uleb128 0x3
	.byte	0x91
	.sleb128 -576
	.uleb128 0x19
	.string	"tmp"
	.byte	0x1
	.byte	0x9c
	.long	0x625
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x1c
	.quad	.LBB2
	.quad	.LBE2-.LBB2
	.long	0x829
	.uleb128 0x17
	.long	.LASF108
	.byte	0x1
	.byte	0x8b
	.long	0x3bd
	.uleb128 0x3
	.byte	0x91
	.sleb128 -720
	.uleb128 0x1d
	.quad	.LBB3
	.quad	.LBE3-.LBB3
	.uleb128 0x19
	.string	"dir"
	.byte	0x1
	.byte	0x91
	.long	0x84a
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.byte	0
	.byte	0
	.uleb128 0x1d
	.quad	.LBB4
	.quad	.LBE4-.LBB4
	.uleb128 0x19
	.string	"dir"
	.byte	0x1
	.byte	0x9e
	.long	0x84a
	.uleb128 0x2
	.byte	0x91
	.sleb128 -64
	.byte	0
	.byte	0
	.uleb128 0x8
	.byte	0x8
	.long	0x657
	.uleb128 0x8
	.byte	0x8
	.long	0x353
	.uleb128 0x8
	.byte	0x8
	.long	0x3ad
	.uleb128 0x15
	.long	.LASF113
	.byte	0x1
	.byte	0xa6
	.long	0x3f
	.quad	.LFB5
	.quad	.LFE5-.LFB5
	.uleb128 0x1
	.byte	0x9c
	.long	0x8d5
	.uleb128 0x18
	.long	.LASF114
	.byte	0x1
	.byte	0xa6
	.long	0x3f
	.uleb128 0x3
	.byte	0x91
	.sleb128 -308
	.uleb128 0x18
	.long	.LASF115
	.byte	0x1
	.byte	0xa6
	.long	0x8d5
	.uleb128 0x3
	.byte	0x91
	.sleb128 -320
	.uleb128 0x19
	.string	"dir"
	.byte	0x1
	.byte	0xac
	.long	0x10d
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x17
	.long	.LASF116
	.byte	0x1
	.byte	0xad
	.long	0x39d
	.uleb128 0x3
	.byte	0x91
	.sleb128 -304
	.uleb128 0x17
	.long	.LASF107
	.byte	0x1
	.byte	0xb0
	.long	0x5e3
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x17
	.long	.LASF111
	.byte	0x1
	.byte	0xb7
	.long	0x84a
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.byte	0
	.uleb128 0x8
	.byte	0x8
	.long	0x10d
	.uleb128 0x1e
	.long	.LASF117
	.byte	0x5
	.byte	0xa9
	.long	0x327
	.uleb128 0x1e
	.long	.LASF118
	.byte	0x5
	.byte	0xaa
	.long	0x327
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0x5
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x13
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x17
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1d
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.byte	0
	.byte	0
	.uleb128 0x1e
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0
	.value	0
	.value	0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	0
	.quad	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF14:
	.string	"__off_t"
.LASF10:
	.string	"__gid_t"
.LASF30:
	.string	"_IO_read_ptr"
.LASF42:
	.string	"_chain"
.LASF82:
	.string	"st_ctim"
.LASF98:
	.string	"dllist"
.LASF7:
	.string	"size_t"
.LASF94:
	.string	"roothead"
.LASF48:
	.string	"_shortbuf"
.LASF36:
	.string	"_IO_buf_base"
.LASF25:
	.string	"long long unsigned int"
.LASF79:
	.string	"st_blocks"
.LASF121:
	.string	"/home/jlemon3/cs360/lab5a"
.LASF99:
	.string	"Dllist"
.LASF116:
	.string	"dir_suffix"
.LASF105:
	.string	"get_dir_suffix"
.LASF68:
	.string	"stat"
.LASF22:
	.string	"long long int"
.LASF4:
	.string	"signed char"
.LASF70:
	.string	"st_ino"
.LASF12:
	.string	"__mode_t"
.LASF111:
	.string	"directory"
.LASF43:
	.string	"_fileno"
.LASF31:
	.string	"_IO_read_end"
.LASF97:
	.string	"parent"
.LASF19:
	.string	"__blkcnt_t"
.LASF6:
	.string	"long int"
.LASF65:
	.string	"d_reclen"
.LASF29:
	.string	"_flags"
.LASF37:
	.string	"_IO_buf_end"
.LASF46:
	.string	"_cur_column"
.LASF108:
	.string	"statbuf"
.LASF107:
	.string	"inodes"
.LASF80:
	.string	"st_atim"
.LASF45:
	.string	"_old_offset"
.LASF50:
	.string	"_offset"
.LASF67:
	.string	"d_name"
.LASF75:
	.string	"__pad0"
.LASF124:
	.string	"iterate_dir"
.LASF8:
	.string	"__dev_t"
.LASF73:
	.string	"st_uid"
.LASF58:
	.string	"_IO_marker"
.LASF3:
	.string	"unsigned int"
.LASF86:
	.string	"carray"
.LASF0:
	.string	"long unsigned int"
.LASF34:
	.string	"_IO_write_ptr"
.LASF102:
	.string	"shortname"
.LASF95:
	.string	"flink"
.LASF60:
	.string	"_sbuf"
.LASF2:
	.string	"short unsigned int"
.LASF64:
	.string	"d_off"
.LASF87:
	.string	"ucarray"
.LASF104:
	.string	"last"
.LASF38:
	.string	"_IO_save_base"
.LASF13:
	.string	"__nlink_t"
.LASF123:
	.string	"__dirstream"
.LASF49:
	.string	"_lock"
.LASF44:
	.string	"_flags2"
.LASF56:
	.string	"_mode"
.LASF117:
	.string	"stdout"
.LASF77:
	.string	"st_size"
.LASF83:
	.string	"__unused"
.LASF72:
	.string	"st_mode"
.LASF24:
	.string	"tv_nsec"
.LASF16:
	.string	"sizetype"
.LASF109:
	.string	"name_length"
.LASF23:
	.string	"tv_sec"
.LASF20:
	.string	"__syscall_slong_t"
.LASF35:
	.string	"_IO_write_end"
.LASF62:
	.string	"dirent"
.LASF85:
	.string	"farray"
.LASF63:
	.string	"d_ino"
.LASF122:
	.string	"_IO_lock_t"
.LASF28:
	.string	"_IO_FILE"
.LASF18:
	.string	"__blksize_t"
.LASF88:
	.string	"float"
.LASF61:
	.string	"_pos"
.LASF103:
	.string	"token"
.LASF41:
	.string	"_markers"
.LASF110:
	.string	"file"
.LASF100:
	.string	"Directory"
.LASF91:
	.string	"jrb_node"
.LASF71:
	.string	"st_nlink"
.LASF1:
	.string	"unsigned char"
.LASF66:
	.string	"d_type"
.LASF5:
	.string	"short int"
.LASF78:
	.string	"st_blksize"
.LASF84:
	.string	"iarray"
.LASF119:
	.string	"GNU C 4.8.5 20150623 (Red Hat 4.8.5-39) -mtune=generic -march=x86-64 -g"
.LASF27:
	.string	"timespec"
.LASF47:
	.string	"_vtable_offset"
.LASF26:
	.string	"FILE"
.LASF112:
	.string	"dirs"
.LASF92:
	.string	"internal"
.LASF11:
	.string	"__ino_t"
.LASF106:
	.string	"print_file"
.LASF76:
	.string	"st_rdev"
.LASF21:
	.string	"char"
.LASF9:
	.string	"__uid_t"
.LASF59:
	.string	"_next"
.LASF15:
	.string	"__off64_t"
.LASF32:
	.string	"_IO_read_base"
.LASF40:
	.string	"_IO_save_end"
.LASF74:
	.string	"st_gid"
.LASF51:
	.string	"__pad1"
.LASF52:
	.string	"__pad2"
.LASF53:
	.string	"__pad3"
.LASF54:
	.string	"__pad4"
.LASF55:
	.string	"__pad5"
.LASF17:
	.string	"__time_t"
.LASF96:
	.string	"blink"
.LASF57:
	.string	"_unused2"
.LASF118:
	.string	"stderr"
.LASF115:
	.string	"argv"
.LASF120:
	.string	"tarc.c"
.LASF69:
	.string	"st_dev"
.LASF101:
	.string	"longname"
.LASF39:
	.string	"_IO_backup_base"
.LASF81:
	.string	"st_mtim"
.LASF114:
	.string	"argc"
.LASF90:
	.string	"Jval"
.LASF113:
	.string	"main"
.LASF33:
	.string	"_IO_write_base"
.LASF93:
	.string	"left"
.LASF89:
	.string	"double"
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-39)"
	.section	.note.GNU-stack,"",@progbits
