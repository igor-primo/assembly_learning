#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<inttypes.h>
#include<assert.h>

FILE *in, *out;
int32_t v64[16] = {0};
uint32_t ic[16] = {0}; // instruction counter
uint8_t code[128] = {0};

int interpret(uint8_t *code, uint32_t n);
void see_v64();

int main(int argc, char **argv){
    if(!(argc == 3))
        return 1;
    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");
    if(in == NULL)
        return 12;
    uint32_t byte = 0, n = 0;
    while(fscanf(in, "%X", &byte) == 1)
        code[n++] = byte;
    interpret(code, n);
    see_v64();
    fclose(in);
    fclose(out);
    return 0;
}

void pi_x86(uint8_t byte){
    
}

void compile(uint8_t *code, int n){
    uint32_t index = 0;
    while(index < n){
        switch(code[index]){
            case 0x00:
                break;
            case 0x01:
                break;
            case 0x02:
                break;
            case 0x03:
                break;
            case 0x04:
                break;
            case 0x05:
                break;
            case 0x06:
                break;
            case 0x07:
                break;
            case 0x08:
                break;
            case 0x09:
                break;
            case 0x0A:
                break;
            case 0x0B:
                break;
            case 0x0C:
                break;
            case 0x0D:
                break;
            case 0x0E:
                break;
            case 0x0F:
                break;
            default:
                printf("I have seen shit worst than this.\n");
                break;
        }
    }
}

int interpret(uint8_t *ccode, uint32_t n){
	uint8_t eq = 0, gt = 0, lt = 0;
    int8_t i = 0;
    int8_t rx = 0, ry = 0;
	uint32_t index = 0;
    int16_t byte1 = 0, byte2 = 0;
    uint32_t offset = 0;
    while(index < n){
        switch(code[index]){
            case 0x00:
                index++;
                i = code[index];
                rx = (0xF0 & i) >> 4;
                index++;
                byte1 = (uint16_t) code[index+1] << 8;
                byte2 = (uint16_t) code[index];
                v64[rx] = byte1 | byte2;
                index += 2;
                ic[0x00]++;
                break;
            case 0x01:
                index++;
                i = code[index];
                rx = (0xF0 & i) >> 4;
                ry = 0xF & i;
                v64[rx] = v64[ry];
                index += 3;
                ic[0x01]++;
                break;
            case 0x02:
                index++;
                i = code[index];
                rx = (0xF0 & i) >> 4;
                ry = 0xF & i;
                v64[rx] = code[v64[ry]+3];
                v64[rx] <<= 8;
                v64[rx] += code[v64[ry]+2];
                v64[rx] <<= 8;
                v64[rx] += code[v64[ry]+1];
                v64[rx] <<= 8;
                v64[rx] += code[v64[ry]];
                index += 3;
                ic[0x02]++;
                break;
            case 0x03:
                index++;
                i = code[index];
                rx = (0xF0 & i) >> 4;
                ry = 0xF & i;
                assert(v64[rx]+3 < index);
                code[v64[rx]+3] = (uint8_t) (v64[ry] & 0xFF);
                code[v64[rx]+2] |= (uint8_t) (v64[ry] & 0xFF00);
                code[v64[rx]+1] |= (uint8_t) (v64[ry] & 0xFF0000);
                code[v64[rx]] |= (uint8_t) (v64[ry] & 0xFF000000);
                index += 3;
                ic[0x03]++;
                break;
            case 0x04:
                index++;
                i = code[index];
                rx = (0xF0 & i) >> 4;
                ry = 0xF & i;

                eq = v64[rx] == v64[ry];
                gt = v64[rx] > v64[ry];
                lt = v64[rx] < v64[ry];
                index += 3;
                ic[0x04]++;
                break;
            case 0x05:
                byte1 = (int16_t) code[index+3] << 8;
                byte2 = (int16_t) code[index+2];
                offset = byte1 | byte2;
                index += 4 + offset;
                ic[0x05]++;
                break;
            case 0x06:
                byte1 = (int16_t) code[index+3] << 8;
                byte2 = (int16_t) code[index+2];
                offset = byte1 | byte2;
                if(gt) index += 4 + offset;
                ic[0x06]++;
                break;
            case 0x07:
                byte1 = (int16_t) code[index+3] << 8;
                byte2 = (int16_t) code[index+2];
                offset = byte1 | byte2;
                if(lt) index += 4 + offset;
                ic[0x07]++;
                break;
            case 0x08:
                byte1 = (int16_t) code[index+3] << 8;
                byte2 = (int16_t) code[index+2];
                offset = byte1 | byte2;
                if(eq) index += 4 + offset;
                ic[0x08]++;
                break;
            case 0x09:
                index++;
                i = code[index];
                rx = (0xF0 & i) >> 4;
                ry = 0xF & i;
                v64[rx] += v64[ry];
                index += 3;
                ic[0x09]++;
                break;
            case 0x0A:
                index++;
                i = code[index];
                rx = (0xF0 & i) >> 4;
                ry = 0xF & i;
                v64[rx] -= v64[ry];
                index += 3;
                ic[0x0A]++;
                break;
            case 0x0B:
                index++;
                i = code[index];
                rx = (0xF0 & i) >> 4;
                ry = 0xF & i;
                v64[rx] &= v64[ry];
                index += 3;
                ic[0x0B]++;
                break;
            case 0x0C:
                index++;
                i = code[index];
                rx = (0xF0 & i) >> 4;
                ry = 0xF & i;
                v64[rx] |= v64[ry];
                index += 3;
                ic[0x0C]++;
                break;
            case 0x0D:
                index++;
                i = code[index];
                rx = (0xF0 & i) >> 4;
                ry = 0xF & i;
                v64[rx] ^= v64[ry];
                index += 3;
                ic[0x0D]++;
                break;
            case 0x0E:
                index++;
                i = code[index];
                rx = (0xF0 & i) >> 4;
                index += 2;
                v64[rx] <<= (0xF & code[index]);
                index++;
                ic[0x0E]++;
                break;
            case 0x0F:
                index++;
                i = code[index];
                rx = (0xF0 & i) >> 4;
                index += 2;
                v64[rx] >>= (0xF & code[index]);
                index++;
                ic[0x0F]++;
                break;
            default:
                printf("I have seen worst than this.\n");
                return -1;
                break;
        }
    }
    return 0;
}

void see_v64(){
    for(int i=0;i<16;i++)
        fprintf(out, "0%x:%"SCNu32"\n", i, ic[i]);
    for(int i=0;i<16;i++)
        fprintf(out, "R[%d]=0x%08x\n", i, v64[i]);
}
