
bits 64

section .text
    default rel
    global _start
    extern __libc_global_fini
    extern __libc_global_init
    extern __libc_start_main
    extern main
_start:
    xor ebp, ebp
    ;; Save rtld_fini address to r9
    mov r9, rdx
    ;; Get argc and argv from the stack
    pop rsi
    mov rdx, rsp
    ;; Align stack to 16, push junk and stack ptr
    and rsp, ~0xf
    push rax
    push rsp
    ;; Load fini and init initializers as function parameters
    push rbx
    lea rbx, [__libc_global_init wrt ..plt]
    mov rcx, rbx
    lea rbx, [__libc_global_fini wrt ..plt]
    mov r8, rbx
    lea rbx, [main wrt ..plt]
    mov rdi, rbx
    pop rbx
    ;; Call start main
    call __libc_start_main wrt ..plt
    ;; No idea why halt it, I guess that's a funny
    ;; way to crash your application if the function we called
    ;; returns instead of calling the exit syscall
    hlt
