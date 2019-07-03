xor rax,rax
push rax
mov rax, 0x2e2e2e2e2e59444f
push rax
mov rax, 0x4f572e2e2e2e
push rax
xor rax,rax
mov rdx, 14
mov rsi, rsp
mov rdi, 1
mov al, 1
syscall
