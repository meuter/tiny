	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main
	.align	4, 0x90
_main:                                  ## @main
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
	subq	$64, %rsp
	leaq	-16(%rbp), %rdi
	leaq	-20(%rbp), %rsi
	leaq	-24(%rbp), %rdx
	leaq	-28(%rbp), %rcx
	movl	$0, -4(%rbp)
	movl	$1, -20(%rbp)
	movl	$2, -24(%rbp)
	movl	$3, -28(%rbp)
	callq	__ZN4tiny4math6vectorIiLm3EEC1ERKiS4_S4_
	leaq	-40(%rbp), %rdi
	leaq	-44(%rbp), %rsi
	leaq	-48(%rbp), %rdx
	leaq	-52(%rbp), %rcx
	movl	$4, -44(%rbp)
	movl	$5, -48(%rbp)
	movl	$6, -52(%rbp)
	callq	__ZN4tiny4math6vectorIiLm3EEC1ERKiS4_S4_
	leaq	-16(%rbp), %rdi
	leaq	-40(%rbp), %rsi
	callq	__ZNK4tiny4math6vectorIiLm3EErmERKS2_
	addq	$64, %rsp
	popq	%rbp
	ret
	.cfi_endproc

	.section	__TEXT,__textcoal_nt,coalesced,pure_instructions
	.globl	__ZN4tiny4math6vectorIiLm3EEC1ERKiS4_S4_
	.weak_def_can_be_hidden	__ZN4tiny4math6vectorIiLm3EEC1ERKiS4_S4_
	.align	4, 0x90
__ZN4tiny4math6vectorIiLm3EEC1ERKiS4_S4_: ## @_ZN4tiny4math6vectorIiLm3EEC1ERKiS4_S4_
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
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	movq	-32(%rbp), %rcx
	callq	__ZN4tiny4math6vectorIiLm3EEC2ERKiS4_S4_
	addq	$32, %rsp
	popq	%rbp
	ret
	.cfi_endproc

	.globl	__ZNK4tiny4math6vectorIiLm3EErmERKS2_
	.weak_def_can_be_hidden	__ZNK4tiny4math6vectorIiLm3EErmERKS2_
	.align	4, 0x90
__ZNK4tiny4math6vectorIiLm3EErmERKS2_:  ## @_ZNK4tiny4math6vectorIiLm3EErmERKS2_
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp12:
	.cfi_def_cfa_offset 16
Ltmp13:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp14:
	.cfi_def_cfa_register %rbp
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rdi
	movq	-16(%rbp), %rsi
	callq	__ZN4tiny4math3dotIiLm3EEET_RKNS0_6vectorIS2_XT0_EEES6_
	addq	$16, %rsp
	popq	%rbp
	ret
	.cfi_endproc

	.globl	__ZN4tiny4math3dotIiLm3EEET_RKNS0_6vectorIS2_XT0_EEES6_
	.weak_def_can_be_hidden	__ZN4tiny4math3dotIiLm3EEET_RKNS0_6vectorIS2_XT0_EEES6_
	.align	4, 0x90
__ZN4tiny4math3dotIiLm3EEET_RKNS0_6vectorIS2_XT0_EEES6_: ## @_ZN4tiny4math3dotIiLm3EEET_RKNS0_6vectorIS2_XT0_EEES6_
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp17:
	.cfi_def_cfa_offset 16
Ltmp18:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp19:
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movl	$0, -20(%rbp)
	movq	$0, -32(%rbp)
LBB3_1:                                 ## =>This Inner Loop Header: Depth=1
	movq	-32(%rbp), %rax
	movq	-16(%rbp), %rdi
	movq	%rax, -40(%rbp)         ## 8-byte Spill
	callq	__ZNK4tiny4math6vectorIiLm3EE9dimensionEv
	movq	-40(%rbp), %rdi         ## 8-byte Reload
	cmpq	%rax, %rdi
	jae	LBB3_4
## BB#2:                                ##   in Loop: Header=BB3_1 Depth=1
	movq	-8(%rbp), %rdi
	movq	-32(%rbp), %rsi
	callq	__ZNK4tiny4math6vectorIiLm3EEclEm
	movl	(%rax), %ecx
	movq	-16(%rbp), %rdi
	movq	-32(%rbp), %rsi
	movl	%ecx, -44(%rbp)         ## 4-byte Spill
	callq	__ZNK4tiny4math6vectorIiLm3EEclEm
	movl	-44(%rbp), %ecx         ## 4-byte Reload
	imull	(%rax), %ecx
	movl	-20(%rbp), %edx
	addl	%ecx, %edx
	movl	%edx, -20(%rbp)
## BB#3:                                ##   in Loop: Header=BB3_1 Depth=1
	movq	-32(%rbp), %rax
	addq	$1, %rax
	movq	%rax, -32(%rbp)
	jmp	LBB3_1
LBB3_4:
	movl	-20(%rbp), %eax
	addq	$48, %rsp
	popq	%rbp
	ret
	.cfi_endproc

	.globl	__ZNK4tiny4math6vectorIiLm3EE9dimensionEv
	.weak_def_can_be_hidden	__ZNK4tiny4math6vectorIiLm3EE9dimensionEv
	.align	4, 0x90
__ZNK4tiny4math6vectorIiLm3EE9dimensionEv: ## @_ZNK4tiny4math6vectorIiLm3EE9dimensionEv
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp22:
	.cfi_def_cfa_offset 16
Ltmp23:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp24:
	.cfi_def_cfa_register %rbp
	movabsq	$3, %rax
	movq	%rdi, -8(%rbp)
	popq	%rbp
	ret
	.cfi_endproc

	.globl	__ZNK4tiny4math6vectorIiLm3EEclEm
	.weak_def_can_be_hidden	__ZNK4tiny4math6vectorIiLm3EEclEm
	.align	4, 0x90
__ZNK4tiny4math6vectorIiLm3EEclEm:      ## @_ZNK4tiny4math6vectorIiLm3EEclEm
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp27:
	.cfi_def_cfa_offset 16
Ltmp28:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp29:
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-8(%rbp), %rsi
	movq	-16(%rbp), %rdi
	shlq	$2, %rdi
	addq	%rdi, %rsi
	movq	%rsi, %rax
	popq	%rbp
	ret
	.cfi_endproc

	.globl	__ZN4tiny4math6vectorIiLm3EEC2ERKiS4_S4_
	.weak_def_can_be_hidden	__ZN4tiny4math6vectorIiLm3EEC2ERKiS4_S4_
	.align	4, 0x90
__ZN4tiny4math6vectorIiLm3EEC2ERKiS4_S4_: ## @_ZN4tiny4math6vectorIiLm3EEC2ERKiS4_S4_
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp32:
	.cfi_def_cfa_offset 16
Ltmp33:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp34:
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movq	-8(%rbp), %rcx
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	movq	-32(%rbp), %rdi
	movq	%rdi, -40(%rbp)         ## 8-byte Spill
	movq	%rcx, %rdi
	movq	-40(%rbp), %rcx         ## 8-byte Reload
	callq	__ZN4tiny4math6matrixIiLm1ELm3EEC2ERKiS4_S4_
	addq	$48, %rsp
	popq	%rbp
	ret
	.cfi_endproc

	.globl	__ZN4tiny4math6matrixIiLm1ELm3EEC2ERKiS4_S4_
	.weak_def_can_be_hidden	__ZN4tiny4math6matrixIiLm1ELm3EEC2ERKiS4_S4_
	.align	4, 0x90
__ZN4tiny4math6matrixIiLm1ELm3EEC2ERKiS4_S4_: ## @_ZN4tiny4math6matrixIiLm1ELm3EEC2ERKiS4_S4_
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp37:
	.cfi_def_cfa_offset 16
Ltmp38:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp39:
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movq	-8(%rbp), %rcx
	movq	-16(%rbp), %rsi
	movq	-24(%rbp), %rdx
	movq	-32(%rbp), %rdi
	movq	%rdi, -40(%rbp)         ## 8-byte Spill
	movq	%rcx, %rdi
	movq	-40(%rbp), %rcx         ## 8-byte Reload
	callq	__ZN4tiny4math8internal11matrix_dataIiLm1ELm3EEC2ERKiS5_S5_
	addq	$48, %rsp
	popq	%rbp
	ret
	.cfi_endproc

	.globl	__ZN4tiny4math8internal11matrix_dataIiLm1ELm3EEC2ERKiS5_S5_
	.weak_def_can_be_hidden	__ZN4tiny4math8internal11matrix_dataIiLm1ELm3EEC2ERKiS5_S5_
	.align	4, 0x90
__ZN4tiny4math8internal11matrix_dataIiLm1ELm3EEC2ERKiS5_S5_: ## @_ZN4tiny4math8internal11matrix_dataIiLm1ELm3EEC2ERKiS5_S5_
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp42:
	.cfi_def_cfa_offset 16
Ltmp43:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp44:
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movq	-8(%rbp), %rcx
	movq	-16(%rbp), %rdx
	movl	(%rdx), %eax
	movl	%eax, (%rcx)
	movq	-24(%rbp), %rdx
	movl	(%rdx), %eax
	movl	%eax, 4(%rcx)
	movq	-32(%rbp), %rdx
	movl	(%rdx), %eax
	movl	%eax, 8(%rcx)
	popq	%rbp
	ret
	.cfi_endproc


.subsections_via_symbols
