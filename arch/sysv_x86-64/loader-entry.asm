
bits 64

section .text
default rel
global _dlstart
extern _DYNAMIC:weak

_dlstart:
    xor rbp, rbp
    mov rdi, rsp
    lea rsi, _DYNAMIC
    and rsp, -16
    ; `call` pushes 8-byte value onto the stack
    ; by pushing an 8-bit value ourselves we can make
    ; sure the stack is aligned after rbp push in prologue
    push 0
     
