	.section	__TEXT,__text,regular,pure_instructions
	.section	__TEXT,__literal4,4byte_literals
	.align	2
LCPI0_0:
	.long	1077936128              ## float 3
LCPI0_1:
	.long	1082130432              ## float 4
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
	cvtsi2ssl	%edi, %xmm0
	movss	LCPI0_0(%rip), %xmm1
	mulss	%xmm0, %xmm1
	movss	LCPI0_1(%rip), %xmm2
	mulss	%xmm0, %xmm2
	addss	%xmm0, %xmm0
	movaps	%xmm0, %xmm3
	addss	%xmm3, %xmm3
	addss	%xmm1, %xmm1
	addss	%xmm2, %xmm2
	mulss	%xmm0, %xmm0
	xorps	%xmm4, %xmm4
	addss	%xmm0, %xmm4
	mulss	%xmm3, %xmm3
	addss	%xmm4, %xmm3
	mulss	%xmm1, %xmm1
	addss	%xmm3, %xmm1
	mulss	%xmm2, %xmm2
	addss	%xmm1, %xmm2
	xorps	%xmm0, %xmm0
	sqrtss	%xmm2, %xmm0
	cvttss2si	%xmm0, %eax
	popq	%rbp
	ret
	.cfi_endproc


.subsections_via_symbols
