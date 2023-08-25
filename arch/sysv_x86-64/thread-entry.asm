
bits 64

section .text
global _cia_clone

; flags, &stack[-2], &parent_tid, &child_tid, 0

; DESCRIPTION:
;   Function to be called when using the _clone syscall
;   We don't just call the _clone syscall directly because
;   if we call the clone syscall wrapper that's defined in
;   linux/sys, our stack will be bad and we won't be able
;   to return from the syscall wrapper
; PARAMETERS:
;   rdi - flags
;   rsi - stack_base
;   rdx - parent_tid_ptr
;   rcx - child_tid_ptr
;   r8  - tls
; RETURNS:
;   i64 value
;     0 if returning as a parent
;     1 if returning as a child
;     negative value if there was an error making the thread
_cia_clone:
    mov r10, rcx
    sub rsi, 8
    ; As a child thread we wanna return to the same place as the parent
    mov rax, qword [rsp]
    mov qword [rsi], rax
    ; ; Copy child tid ptr
    ; mov qword [rsi + 0], rcx
    ; Call syscall right away, since the order of the first 5 arguments
    ; matches with the argument order of the function
    mov rax, 56 ; SYS_CLONE
    syscall
    ; Check to see if we're child
    test eax, eax
    jnz .exit
.exit:
    ret
