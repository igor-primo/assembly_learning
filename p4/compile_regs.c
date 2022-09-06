#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<inttypes.h>
#include<assert.h>
#include<string.h>
#include<sys/mman.h>
#include<unistd.h>

// TODO: salvar rsp antes de modificalo

#define CODE_SIZE 4096

FILE *in, *out;
uint8_t code[128] = {0};
uint8_t codex86[CODE_SIZE] = {0};
uint32_t n = 0;
uint32_t p = 0;

void compile(uint8_t *code, int n);
void pi(uint8_t byte); // push into x86 code
void show_code(int size);
void execute();

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
    show_code(n);
    execute();
    fclose(in);
    fclose(out);
    return 0;
}

void pi(uint8_t byte){
    assert(p < n);
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
    pi(0x48); // mov rsp to safe localtion
    pi(0x89);
    pi(0x25);
    pi(0xf7);
    pi(0x0f);
    pi(0x00);
    pi(0x00);
    while(index < n){
        switch(code[index]){
            case 0x00:
                rx = (0xF0 & code[index+1]) >> 4;
                byte1 = code[index+2];
                byte2 = code[index+3];
                prefix = rx > 7 ? 0x49: 0x48;
                rx = rx > 7 ? rx % 8 : rx;
                operand = 0xc0;
                operand |= rx;
                pi(prefix);
                pi(0xc7);
                pi(operand);
                pi(byte1);
                pi(byte2);
                pi(0x00);
                pi(0x00);

                index += 4;
                break;
            case 0x01:
                rx = (0xF0 & code[index+1]) >> 4;
                ry = 0xF & code[index+1];
                operand = 0xc0;
                source = ry > 7 ? ry % 8 : ry; // 000 <= source <= 111
                destination = rx > 7 ? rx % 8 : rx;
                assert(source < 8);
                assert(destination < 8);
                source <<= 3;
                operand |= source | destination;
                if(7 > rx && 7 > ry)
                    prefix = 0x48;
                else if(rx > 7 && 7 > ry)
                    prefix = 0x49;
                else if(7 > rx && ry > 7)
                    prefix = 0x4c;
                else if(rx > 7 && ry > 7)
                    prefix = 0x4d;
                pi(prefix);
                pi(0x89);
                pi(operand);

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
                operand = 0xc0;
                source = ry > 7 ? ry % 8 : ry; // 000 <= source <= 111
                destination = rx > 7 ? rx % 8 : rx;
                assert(source < 8);
                assert(destination < 8);
                source <<= 3;
                operand |= source | destination;
                if(7 > rx && 7 > ry)
                    prefix = 0x48;
                else if(rx > 7 && 7 > ry)
                    prefix = 0x49;
                else if(7 > rx && ry > 7)
                    prefix = 0x4c;
                else if(rx > 7 && ry > 7)
                    prefix = 0x4d;
                pi(prefix);
                pi(0x39);
                pi(operand);

                index += 4;
                break;
            case 0x05:
                /*
                byte1 = code[index+2];
                byte2 = code[index+3];
                pi(0xff);
                pi(0x25);
                pi(byte1);
                pi(byte2);
                pi(0x00);
                pi(0x00);
                */

                index += 4;
                break;
            case 0x06:
                index += 4;
                break;
            case 0x07:
                index += 4;
                break;
            case 0x08:
                index += 4;
                break;
            case 0x09:
                rx = (0xF0 & code[index+1]) >> 4;
                ry = 0xF & code[index+1];
                operand = 0xc0;
                source = ry > 7 ? ry % 8 : ry; // 000 <= source <= 111
                destination = rx > 7 ? rx % 8 : rx;
                assert(source < 8);
                assert(destination < 8);
                source <<= 3;
                operand |= source | destination;
                if(7 > rx && 7 > ry)
                    prefix = 0x48;
                else if(rx > 7 && 7 > ry)
                    prefix = 0x49;
                else if(7 > rx && ry > 7)
                    prefix = 0x4c;
                else if(rx > 7 && ry > 7)
                    prefix = 0x4d;
                pi(prefix);
                pi(0x01);
                pi(operand);

                index += 4;
                break;
            case 0x0A:
                rx = (0xF0 & code[index+1]) >> 4;
                ry = 0xF & code[index+1];
                operand = 0xc0;
                source = ry > 7 ? ry % 8 : ry; // 000 <= source <= 111
                destination = rx > 7 ? rx % 8 : rx;
                assert(source < 8);
                assert(destination < 8);
                source <<= 3;
                operand |= source | destination;
                if(7 > rx && 7 > ry)
                    prefix = 0x48;
                else if(rx > 7 && 7 > ry)
                    prefix = 0x49;
                else if(7 > rx && ry > 7)
                    prefix = 0x4c;
                else if(rx > 7 && ry > 7)
                    prefix = 0x4d;
                pi(prefix);
                pi(0x29);
                pi(operand);

                index += 4;
                break;
            case 0x0B:
                rx = (0xF0 & code[index+1]) >> 4;
                ry = 0xF & code[index+1];
                operand = 0xc0;
                source = ry > 7 ? ry % 8 : ry; // 000 <= source <= 111
                destination = rx > 7 ? rx % 8 : rx;
                assert(source < 8);
                assert(destination < 8);
                source <<= 3;
                operand |= source | destination;
                if(7 > rx && 7 > ry)
                    prefix = 0x48;
                else if(rx > 7 && 7 > ry)
                    prefix = 0x49;
                else if(7 > rx && ry > 7)
                    prefix = 0x4c;
                else if(rx > 7 && ry > 7)
                    prefix = 0x4d;
                pi(prefix);
                pi(0x21);
                pi(operand);

                index += 4;
                break;
            case 0x0C:
                rx = (0xF0 & code[index+1]) >> 4;
                ry = 0xF & code[index+1];
                operand = 0xc0;
                source = ry > 7 ? ry % 8 : ry; // 000 <= source <= 111
                destination = rx > 7 ? rx % 8 : rx;
                assert(source < 8);
                assert(destination < 8);
                source <<= 3;
                operand |= source | destination;
                if(7 > rx && 7 > ry)
                    prefix = 0x48;
                else if(rx > 7 && 7 > ry)
                    prefix = 0x49;
                else if(7 > rx && ry > 7)
                    prefix = 0x4c;
                else if(rx > 7 && ry > 7)
                    prefix = 0x4d;
                pi(prefix);
                pi(0x09);
                pi(operand);

                index += 4;
                break;
            case 0x0D:
                rx = (0xF0 & code[index+1]) >> 4;
                ry = 0xF & code[index+1];
                operand = 0xc0;
                source = ry > 7 ? ry % 8 : ry; // 000 <= source <= 111
                destination = rx > 7 ? rx % 8 : rx;
                assert(source < 8);
                assert(destination < 8);
                source <<= 3;
                operand |= source | destination;
                if(7 > rx && 7 > ry)
                    prefix = 0x48;
                else if(rx > 7 && 7 > ry)
                    prefix = 0x49;
                else if(7 > rx && ry > 7)
                    prefix = 0x4c;
                else if(rx > 7 && ry > 7)
                    prefix = 0x4d;
                pi(prefix);
                pi(0x31);
                pi(operand);

                index += 4;
                break;
            case 0x0E:
                index += 4;
                break;
            case 0x0F:
                index += 4;
                break;
            default:
                printf("I have seen shit worst than this.\n");
                break;
        }
    }
    // get rsp from safe location
    pi(0x48);
    pi(0x8b);
    pi(0x25);
    pi(0xf7);
    pi(0x0f);
    pi(0x00);
    pi(0x00);
    pi(0x5d);
    pi(0xc3);
}

void show_code(int size){
    FILE *dump = fopen("dump", "w");
    assert(dump != NULL);
    int i = 0;
    while(i < size) fprintf(dump, "%02x", codex86[i]), i++;
    fclose(dump);
}

void execute(){
    int i = 0;
    while(codex86[i] != 0xc3) i++;
    printf("execution attempt %d\n", i);
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
