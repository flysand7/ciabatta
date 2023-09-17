
bits 64

section .text
global _rt_thread_start

extern _rt_thread_setup
extern _rt_thread_finish

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
_rt_thread_start:
    mov r10, rcx
    ; Setup child stack
    sub rsi, 24
    mov rax, [rsp]
    mov [rsi+16], rax
    mov rax, [rsp+8]
    mov [rsi+8], rax
    mov [rsi], r9
    ; Call syscall right away, since the order of the first 5 arguments
    ; matches with the argument order of the function
    mov rax, 56 ; SYS_CLONE
    syscall
    ; Check to see if we're child
    test eax, eax
    jnz .exit
    ; If child, jump to thread function
    pop rdi ; thread_fn
    pop rsi ; ctx
    ; Align the stack
    and rsp, -16
    call _rt_thread_setup
    ; Make return value the first arg and call thread finish routine
    mov rdi, rax
    call _rt_thread_finish
    ; Just to be sure...    
    hlt
.exit:
    ret
