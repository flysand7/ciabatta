bits 64

segment .text
global __chkstk
__chkstk:
    sub   rsp, 0x10
    mov   [rsp], r10
    mov   [rsp+0x8], r11
    xor   r11, r11
    lea   r10, [rsp+0x18]
    sub   r10, rax
    cmovb r10, r11
    mov   r11, gs:[0x10]
    cmp   r10, r11
    jnb   .end
    and   r10w, 0xf000
.loop:
    lea   r11, [r11-0x1000]
    mov   byte [r11], 0x0
    cmp   r10, r11
    jnz   .loop
.end:
    mov   r10, [rsp]
    mov   r11, [rsp+0x8]
    add   rsp, 0x10
    ret