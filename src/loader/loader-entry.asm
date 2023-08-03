
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
