	push rbp
	mov rbp,rsp
	push rax
	push rdi
	push rsi
	push rdx
	push rcx
	push r11
	pushfq

	xor rax,rax
	push rax
	mov rax, 0x0a2e2e2e2e59
	push rax
	mov rax, 0x444f4f572e2e2e2e
	push rax
	xor rdx,rdx
	xor rsi,rsi
	xor rdi,rdi
	xor rax,rax
	mov rdx, 14
	mov rsi, rsp
	mov rdi, 1
	mov al, 1
	syscall

	xor rax,rax
	sub r12,4104
	mov rax,r12
	mov rdx,0
loop:
	xor byte [rax],0x13
	add rax,1
	add	rdx,1
	cmp rdx,389
	jl	loop

	pop rax
	pop rax
	pop rax
	popfq
	pop r11
	pop rcx
	pop rdx
	pop rsi
	pop rdi
	pop rax
	mov rsp,rbp
	pop rbp

