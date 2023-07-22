
bits 64

section .text

global _start
; global _init
; global _fini
extern __libc_global_fini
extern __libc_global_init
extern __libc_start_main
extern main

; _init:
;     push ebp
;     mov ebp, esp
; _fini:

_start:
    xor ebp, ebp
    ;; Save rtld_fini address to r9
    mov r9, rdx
    ;; Get argc and argv from the stack
    pop rsi
    mov rdx, qword [rsp]
    ;; Align stack to 16, push junk and stack ptr
    and rsp, ~0xf
    push rax
    push rsp
    ;; Push fini and init sections
    mov rcx, __libc_global_init wrt ..got
    mov r8, __libc_global_fini wrt ..got
    mov rdi, main
    ;; Call start main
    call __libc_start_main wrt ..plt
    ;; No idea why halt it, I guess that's a funny
    ;; way to crash your application if the function we called
    ;; returns instead of calling the exit syscall
    hlt
