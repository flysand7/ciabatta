
bits 64

section .text
default rel
global _dlstart
extern _dlstart_c

_dlstart:
    xor rbp, rbp
    mov rdi, rsp
    lea rsi, [_dlstart_c wrt ..plt]
    and rsp, -15
