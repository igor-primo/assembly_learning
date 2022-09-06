.intel_syntax noprefix

.section .bss
help: .zero 8
help2: .zero 8

.section .text
.global main
main:
    push   rbp
    mov    rbp, rsp
    sub    rsp,0x80
    mov    rax,0x1
    mov    QWORD PTR [rbp-0x8],rax
    mov    rax,0x2
    mov    QWORD PTR [rbp-0x10],rax
    mov    rax,0x3
    mov    QWORD PTR [rbp-0x18],rax
    mov    rax,0x8000
    mov    QWORD PTR [rbp-0x20],rax
    mov    rax,QWORD PTR [rbp-0x8]
    mov    QWORD PTR [rbp-0x28],rax
    mov    rax,QWORD PTR [rbp-0x10]
    mov    QWORD PTR [rbp-0x30],rax
    mov    rax,QWORD PTR [rbp-0x18]
    mov    QWORD PTR [rbp-0x38],rax
    mov    rax,QWORD PTR [rbp-0x20]
    mov    QWORD PTR [rbp-0x40],rax
    mov    rax,QWORD PTR [rbp-0x8]
    mov    rbx,QWORD PTR [rbp-0x10]
    cmp    rax,rbx
    jmp    0x70
    jg     0x6a
    jl     0x70
    je     0x76
    mov    rax,QWORD PTR [rbp-0x58]
    mov    rbx,QWORD PTR [rbp-0x8]
    add    rax,rbx
    mov    QWORD PTR [rbp-0x58],rax
    mov    rax,QWORD PTR [rbp-0x60]
    mov    rbx,QWORD PTR [rbp-0x20]
    sub    rax,rbx
    mov    QWORD PTR [rbp-0x60],rax
    mov    rax,QWORD PTR [rbp-0x18]
    mov    rbx,QWORD PTR [rbp-0x10]
    and    rax,rbx
    mov    QWORD PTR [rbp-0x18],rax
    mov    rax,QWORD PTR [rbp-0x10]
    mov    rbx,QWORD PTR [rbp-0x18]
    or     rax,rbx
    mov    QWORD PTR [rbp-0x10],rax
    mov    rax,QWORD PTR [rbp-0x8]
    mov    rbx,QWORD PTR [rbp-0x8]
    xor    rax,rbx
    mov    QWORD PTR [rbp-0x8],rax
    mov    rax,QWORD PTR [rbp-0x20]
    shl    rax,0xc
    mov    QWORD PTR [rbp-0x20],rax
    mov    rax,QWORD PTR [rbp-0x8]
    shr    rax,0x0
    mov    QWORD PTR [rbp-0x8],rax
    jmp    0x182
    mov    rax,0x0
    mov    QWORD PTR [rbp-0x8],rax
    mov    rax,QWORD PTR [rbp-0x8]
    mov    QWORD PTR [rbp-0x8],rax
    xor    rax,rax
    mov    rsp,rbp
    pop    rbp
    ret
