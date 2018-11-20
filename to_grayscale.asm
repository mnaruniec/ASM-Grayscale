global set_rgb
global to_grayscale

section .data
    RED dd 77
    GREEN dd 151
    BLUE dd 28

section .text

set_rgb:
    mov [RED], dil
    mov [GREEN], sil
    mov [BLUE], dl
    ret

to_grayscale:
    push rbp
    mov rbp, rsp
    imul rdx, rsi ; length = width * height
    mov rsi, rdi ; source_ptr = dest_ptr

    mov ecx, [RED] ; load color weights to free registers
    mov r9d, [GREEN]
    mov r11d, [BLUE]

    test rdx, rdx
    jmp cond
loop:
    xor eax, eax
    lodsb
    imul eax, ecx ; calculate red
    mov r8d, eax

    xor eax, eax
    lodsb
    imul eax, r9d ; calculate green
    add r8d, eax

    xor eax, eax
    lodsb
    imul eax, r11d ; calculate blue
    add eax, r8d

    shr eax, 8 ; divide by 256

    stosb

    dec rdx
cond:
    jnz loop ; if rcx != 0
    leave
    ret

