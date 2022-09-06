#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<inttypes.h>
#include<assert.h>
#include<string.h>
#include<sys/mman.h>
#include<unistd.h>

#define CODE_SIZE 4096

FILE *in, *out;
uint8_t code[128] = {0};
uint8_t codex86[CODE_SIZE] = {0};
uint32_t n = 0;
uint32_t p = 0;

void compile(uint8_t *code, int n);
void pi(uint8_t byte); // push into x86 code
void show_code();
void execute();
uint8_t b(int8_t r);

int main(int argc, char **argv){
    if(!(argc == 3))
        return 1;
    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");
    if(in == NULL || out == NULL)
        return 12;
    uint32_t byte = 0;
    while(fscanf(in, "%X", &byte) == 1)
        code[n++] = byte;
    compile(code, n);
    show_code();
    execute();
    fclose(in);
    fclose(out);
    return 0;
}

void pi(uint8_t byte){
    assert(p < CODE_SIZE);
    codex86[p] = byte;
    p++;
}

void compile(uint8_t *code, int n){
    uint32_t index = 0;
    uint8_t prefix = 0;
    int8_t rx = 0, ry = 0;
    uint8_t operand = 0;
    int8_t byte1 = 0, byte2 = 0;
    int8_t source = 0, destination = 0;
    uint32_t length = 0;

    pi(0x55);
    pi(0x48);
    pi(0x89);
    pi(0xe5);
    pi(0x48);
    pi(0x81);
    pi(0xec);
    pi(0x80);
    pi(0x00);
    pi(0x00);
    pi(0x00);
    while(index < n){
        switch(code[index]){
            case 0x00:
                rx = (0xF0 & code[index+1]) >> 4;

                // mov rax, <arg>
                pi(0x48);
                pi(0xc7);
                pi(0xc0);
                pi(code[index+2]);
                pi(code[index+3]);
                pi(0x00);
                pi(0x00);
                // mov [rbp-?], rax
                pi(0x48);
                pi(0x89);
                pi(0x45);
                pi(256 - ((rx + 1) * 8));

                index += 4;
                break;
            case 0x01:
                rx = (0xF0 & code[index+1]) >> 4;
                ry = 0xF & code[index+1];

                // mov rax, [rbp-?]
                pi(0x48);
                pi(0x8b);
                pi(0x45);
                pi(256 - ((ry + 1) * 8));

                // mov [rbp-??], rax
                pi(0x48);
                pi(0x89);
                pi(0x45);
                pi(256 - ((rx + 1) * 8));

                index += 4;
                break;
            case 0x02:
                index += 4;
                break;
            case 0x03:
                index += 4;
                break;
            case 0x04:
                rx = (0xF0 & code[index+1]) >> 4;
                ry = 0xF & code[index+1];

                // mov rax, [rbp-?]
                pi(0x48);
                pi(0x8b);
                pi(0x45);
                pi(256 - ((rx + 1) * 8));

                // mov rbx, [rbp-??]
                pi(0x48);
                pi(0x8b);
                pi(0x5d);
                pi(256 - ((ry + 1) * 8));

                // cmp rax, rbx
                pi(0x48);
                pi(0x39);
                pi(0xd8);

                index += 4;
                break;
            case 0x05:
                // complicado
                // jmp offset
                // e9 offset
                pi(0xe9);
                pi(code[index+2]);
                pi(code[index+3]);
                pi(0x00);
                pi(0x00);

                index += 4;
                break;
            case 0x06:
                // jg offset
                // 0f 8f offset
                pi(0x0f);
                pi(0x8f);
                pi(code[index+2]);
                pi(code[index+3]);
                pi(0x00);
                pi(0x00);

                index += 4;
                break;
            case 0x07:
                // jl offset
                pi(0x0f);
                pi(0x8c);
                pi(code[index+2]);
                pi(code[index+3]);
                pi(0x00);
                pi(0x00);

                index += 4;
                break;
            case 0x08:
                // je offset
                pi(0x0f);
                pi(0x84);
                pi(code[index+2]);
                pi(code[index+3]);
                pi(0x00);
                pi(0x00);

                index += 4;
                break;
            case 0x09:
                rx = (0xF0 & code[index+1]) >> 4;
                ry = 0xF & code[index+1];

                // mov rax, [rbp-?]
                pi(0x48);
                pi(0x8b);
                pi(0x45);
                pi(b(rx));
                
                // mov rbx, [rbp-??]
                pi(0x48);
                pi(0x8b);
                pi(0x5d);
                pi(b(ry));

                // add rax, rbx
                pi(0x48);
                pi(0x01);
                pi(0xd8);

                // mov [rbp-?], rax
                pi(0x48);
                pi(0x89);
                pi(0x45);
                pi(b(rx));

                index += 4;
                break;
            case 0x0A:
                rx = (0xF0 & code[index+1]) >> 4;
                ry = 0xF & code[index+1];

                // mov rax, [rbp-?]
                pi(0x48);
                pi(0x8b);
                pi(0x45);
                pi(b(rx));
                
                // mov rbx, [rbp-??]
                pi(0x48);
                pi(0x8b);
                pi(0x5d);
                pi(b(ry));

                // sub rax, rbx
                pi(0x48);
                pi(0x29);
                pi(0xd8);

                // mov [rbp-?], rax
                pi(0x48);
                pi(0x89);
                pi(0x45);
                pi(b(rx));

                index += 4;
                break;
            case 0x0B:
                rx = (0xF0 & code[index+1]) >> 4;
                ry = 0xF & code[index+1];

                // mov rax, [rbp-?]
                pi(0x48);
                pi(0x8b);
                pi(0x45);
                pi(b(rx));
                
                // mov rbx, [rbp-??]
                pi(0x48);
                pi(0x8b);
                pi(0x5d);
                pi(b(ry));

                // and rax, rbx
                pi(0x48);
                pi(0x21);
                pi(0xd8);

                // mov [rbp-?], rax
                pi(0x48);
                pi(0x89);
                pi(0x45);
                pi(b(rx));

                index += 4;
                break;
            case 0x0C:
                rx = (0xF0 & code[index+1]) >> 4;
                ry = 0xF & code[index+1];

                // mov rax, [rbp-?]
                pi(0x48);
                pi(0x8b);
                pi(0x45);
                pi(b(rx));
                
                // mov rbx, [rbp-??]
                pi(0x48);
                pi(0x8b);
                pi(0x5d);
                pi(b(ry));

                // or rax, rbx
                pi(0x48);
                pi(0x09);
                pi(0xd8);

                // mov [rbp-?], rax
                pi(0x48);
                pi(0x89);
                pi(0x45);
                pi(b(rx));

                index += 4;
                break;
            case 0x0D:
                rx = (0xF0 & code[index+1]) >> 4;
                ry = 0xF & code[index+1];

                // mov rax, [rbp-?]
                pi(0x48);
                pi(0x8b);
                pi(0x45);
                pi(b(rx));
                
                // mov rbx, [rbp-??]
                pi(0x48);
                pi(0x8b);
                pi(0x5d);
                pi(b(ry));

                // xor rax, rbx
                pi(0x48);
                pi(0x31);
                pi(0xd8);

                // mov [rbp-?], rax
                pi(0x48);
                pi(0x89);
                pi(0x45);
                pi(b(rx));

                index += 4;
                break;
            case 0x0E:
                rx = (0xF0 & code[index+1]) >> 4;
                operand = (0x1F & code[index+3]);

                // mov rax, [rbp-?]
                pi(0x48);
                pi(0x8b);
                pi(0x45);
                pi(b(rx));

                // shl rax, ??
                pi(0x48);
                pi(0xc1);
                pi(0xe0);
                pi(operand);
                
                // mov [rbp-?], rax
                pi(0x48);
                pi(0x89);
                pi(0x45);
                pi(b(rx));

                index += 4;
                break;
            case 0x0F:
                rx = (0xF0 & code[index+1]) >> 4;
                operand = (0x1F & code[index+3]);

                // mov rax, [rbp-?]
                pi(0x48);
                pi(0x8b);
                pi(0x45);
                pi(b(rx));

                // shr rax, ??
                pi(0x48);
                pi(0xc1);
                pi(0xe8);
                pi(operand);
                
                // mov [rbp-?], rax
                pi(0x48);
                pi(0x89);
                pi(0x45);
                pi(b(rx));

                index += 4;
                break;
            default:
                printf("I have seen shit worst than this.\n");
                break;
        }
    }
    pi(0x48);
    pi(0x31);
    pi(0xc0);
    pi(0x48);
    pi(0x89);
    pi(0xec);
    pi(0x5d);
    pi(0xc3);
}

void show_code(){
    FILE *dump = fopen("dump", "w");
    assert(dump != NULL);
    int i = 0;
    while(i < CODE_SIZE) fprintf(dump, "%02x", codex86[i]), i++;
    fclose(dump);
}

void execute(){
    uint32_t length = sysconf(_SC_PAGE_SIZE);
    void* memory = mmap(0, length, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    mprotect(memory, length, PROT_WRITE);
    codex86[0xff7] = 0xff;
    memcpy(memory, (void*)(codex86), sizeof(codex86));
    mprotect(memory, length, PROT_EXEC);
    void (*jit)(void) = (void (*)(void))(memory);
    (*jit)();
    munmap(memory, length);
}

uint8_t b(int8_t r){
    return 256 - ((r + 1) * 8);
}
