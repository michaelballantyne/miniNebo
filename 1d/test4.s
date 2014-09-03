	.section	__TEXT,__text,regular,pure_instructions
	.globl	__ZplRK5FieldS1_
	.align	4, 0x90
__ZplRK5FieldS1_:                       ## @_ZplRK5FieldS1_
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp2:
	.cfi_def_cfa_offset 16
Ltmp3:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp4:
	.cfi_def_cfa_register %rbp
	leaq	-8(%rbp), %rax
	movq	%rax, (%rdi)
	leaq	-16(%rbp), %rax
	movq	%rax, 8(%rdi)
	movq	%rdi, %rax
	popq	%rbp
	ret
	.cfi_endproc

	.globl	__ZmlRK5FieldS1_
	.align	4, 0x90
__ZmlRK5FieldS1_:                       ## @_ZmlRK5FieldS1_
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp7:
	.cfi_def_cfa_offset 16
Ltmp8:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp9:
	.cfi_def_cfa_register %rbp
	leaq	-8(%rbp), %rax
	movq	%rax, (%rdi)
	leaq	-16(%rbp), %rax
	movq	%rax, 8(%rdi)
	movq	%rdi, %rax
	popq	%rbp
	ret
	.cfi_endproc

	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:                                ## %_ZN5FieldD1Ev.exit2
	pushq	%rbp
Ltmp12:
	.cfi_def_cfa_offset 16
Ltmp13:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp14:
	.cfi_def_cfa_register %rbp
	movq	__ZNSt3__14coutE@GOTPCREL(%rip), %rdi
	movl	$12, %esi
	callq	__ZNSt3__113basic_ostreamIcNS_11char_traitsIcEEElsEi
	xorl	%eax, %eax
	popq	%rbp
	ret
	.cfi_endproc


.subsections_via_symbols
