.intel_syntax noprefix
.section .rodata
rfn: .string "sort.input"
r_mode: .string "r"
wfn: .string "sort.output"
w_mode: .string "w"
tests_format: .string "%ld "
newline: .string "\n"
indexation: .string "[%d] "

.section .bss
input_f: .zero 8
output_f: .zero 8
arr: .zero 8
tests: .zero 8
trick: .zero 8
size: .zero 8
s: .zero 8
i: .zero 8
condition: .zero 8

# sort
s_m: .zero 8
s_mm: .zero 8
min: .zero 8
modified: .zero 8
j: .zero 8
add_min: .zero 8

aux1: .zero 8
aux2: .zero 8


# reproduzir problema com o arr
    
.section .text
.global main
main:
    push rbp
    mov rbp, rsp

    # abrindo arquivos de entrada e saida
    lea rsi, [rip+r_mode]
    lea rdi, [rip+rfn]
    call fopen@plt

    # guardando ponteiro para o arquivo aberto
    mov [rip+input_f], rax

    lea rsi, [rip+w_mode]
    lea rdi, [rip+wfn]
    call fopen@plt

    # guardando ponteiro para o arquivo aberto
    mov [rip+output_f], rax
    
    # pegando numero de testes
    lea rdx, [rip+tests]
    lea rsi, [rip+tests_format]
    mov rdi, [rip+input_f]
    call fscanf@plt

    mov edx, [rip+tests]
    mov [rip+trick], edx

.LOOP:
    cmp edx, 0
    je .OUT
    mov [rip+condition], edx
    # dentro do loop, lembrar de decrementar a cond
    lea rdx, [rip+size]
    lea rsi, [rip+tests_format]
    mov rdi, [rip+input_f]
    call fscanf@plt
    mov rax, 8
    #mov rax, 4
    mov rdx, [rip+size]
    mul rdx

    mov rdi, rax
    call malloc@plt
    mov [rip+arr], rax
    # espero q arr contenha um endereco mesmo
    mov rdx, [rip+size]
    mov [rip+s], rdx
    mov ebx, 0
    #mov ecx, [rip+arr]
    mov [rip+i], ebx
    .LOOP2:
        cmp ebx, [rip+size]
        je .OUT2
        mov [rip+i], ebx
        mov rax, 8
        mov rdx, [rip+i]
        mul rdx
        mov rdx, [rip+arr]
        add rdx, rax
        lea rsi, [rip+tests_format]
        mov rdi, [rip+input_f]
        call fscanf@plt

        add ebx, 1
        jmp .LOOP2
    .OUT2:
     mov rdi, [rip+s]
     call sort

    # trick - (tests + 1)
    mov rax, [rip+tests]
    sub rax, 1
    mov [rip+tests], rax

    inc rax
    mov rbx, rax
    mov rax, [rip+trick]
    sub rax, rbx

    mov rdx, rax
    lea rsi, [rip+indexation]
    mov rdi, [rip+output_f]
    call fprintf@plt

    mov ebx, 0
    .LOOP3:
        cmp ebx, [rip+s]
        je .OUT3
        mov [rip+i], ebx
        mov rax, 8
        mov rdx, [rip+i]
        mul rdx
        mov rdx, [rip+arr]
        add rdx, rax
        mov rdx, [rdx]
        lea rsi, [rip+tests_format]
        mov rdi, [rip+output_f]
        call fprintf@plt

        add ebx, 1
        jmp .LOOP3
    .OUT3:
    lea rsi, [rip+newline]
    mov rdi, [rip+output_f]
    call fprintf@plt

    mov rdi, [rip+arr]
    call free@plt

    mov edx, [rip+condition]
    sub edx, 1
    mov [rip+condition], edx

    jmp .LOOP
.OUT:
    # fechando arquivos
    mov rdi, [rip+input_f]
    call fclose@plt
    mov rdi, [rip+output_f]
    call fclose@plt

    xor eax, eax
    pop rbp
    ret

sort:
    push rbp
    mov rbp, rsp
    sub rdi, 1
    mov [rip+s_m], rdi # s_m == size-1

    mov ebx, 0
    # loop
.LOOP_FORA:
    cmp ebx, [rip+s_m]
    jge .OUT_LOOP_FORA

    mov [rip+i], ebx
    mov [rip+min], ebx

    mov ecx, ebx
    inc ecx
.LOOP_DENTRO:
    cmp ecx, [rip+size]
    jge .OUT_LOOP_DENTRO

    mov [rip+j], ecx

    mov rax, 8
    mov rdx, [rip+j]
    mul rdx
    mov rsi, [rip+arr]
    add rsi, rax
    mov rsi, [rsi]
    
    mov rax, 8
    mov rdx, [rip+min]
    mul rdx
    mov rdi, [rip+arr]
    add rdi, rax
    mov rdi, [rdi]

    inc ecx
    
    cmp rsi, rdi
    jge .LOOP_DENTRO

    mov rax, [rip+j]
    mov [rip+min], rax

    jmp .LOOP_DENTRO

.OUT_LOOP_DENTRO:
    #if(min != i)
    inc ebx
    mov rsi, [rip+min]
    mov rdi, [rip+i]

    cmp rsi, rdi
    je .LOOP_FORA

    call swap
    
    jmp .LOOP_FORA

.OUT_LOOP_FORA:
    pop rbp
    ret

swap:
    push rbp
    mov rbp, rsp

    mov rax, 8
    mov rdx, rsi
    mul rdx
    add rax, [rip+arr]

    push [rax]

    mov rax, 8
    mov rdx, rdi
    mul rdx
    add rax, [rip+arr]

    push [rax]

    mov rax, 8
    mov rdx, rsi
    mul rdx
    add rax, [rip+arr]

    pop [rax]

    mov rax, 8
    mov rdx, rdi
    mul rdx
    add rax, [rip+arr]

    pop [rax]

    pop rbp
    ret
