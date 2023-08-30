
bits 64

section .text
global ld_stack_trampoline

; DESCRIPTION:
;   This function copies the current stack down from RSP to the top of the stack
;   into a memory region provided in parameter
;   After the stack has been copied, we jump into ld_stage3_entry
; PARAMS:
;   RDI (u64) - dst stack base
;   RSI (u64) - src stack base
;   RDX (u64) - dst stack size
;   RCX (u64) - src stack size
;   R8  (u64) - function to call
;   R9  (u64) - param to pass to function
; RETURNS:
;   guess what, it doesn't
ld_stack_trampoline:
    ; Calculate the amount of memory we need to move to the new stack
    add rcx, rsi
    sub rcx, rsp
    ; Calculate the base of the source memory
    mov rsi, rsp
    ; Check to see if the amount we're about to move is less than dst stack size
    cmp rcx, rdx
    jl .continue
    xor rdi, rdi
    mov rsi, r9
    mov rax, r8
    call rax
.continue:
    ; Save rsi, rdi, rcx on stack
    push rsi
    push rdi
    push rcx
    ; copy stack
    shr rcx, 3
    rep movsq
    ; get back our regs
    pop rcx
    pop rdi
    pop rsi
    ; We copied the stack at this point, lets switch
    add rsp, rdi
    sub rsp, rsi
    add rsp, rdx
    sub rsp, rcx
    ; We should have the new stack now, which is an exact copy of the old stack
    and rsp, -16
    mov rdi, 1
    mov rsi, r9
    mov rax, r8
    call rax