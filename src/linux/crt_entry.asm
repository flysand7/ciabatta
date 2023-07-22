
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
    mov rdx, rsp
    ;; Align stack to 16, push junk and stack ptr
    and rsp, ~0xf
    push rax
    push rsp
    ;; Load fini and init initializers as function parameters
    %ifdef CIA_SHARED
        mov rcx, __libc_global_init wrt ..got
        mov r8, __libc_global_fini wrt ..got
    %else
        mov rcx, __libc_global_init
        mov r8, __libc_global_fini
    %endif
    mov rdi, main
    ;; Call start main
    %ifdef CIA_SHARED
        call __libc_start_main wrt ..plt
    %else
        call __libc_start_main
    %endif
    ;; No idea why halt it, I guess that's a funny
    ;; way to crash your application if the function we called
    ;; returns instead of calling the exit syscall
    hlt
