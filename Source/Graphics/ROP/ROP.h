#ifndef ROP_H
#define ROP_H
#include "../Framebuffer.h"


enum ZBUFF_RES {
    LOWER,
    EQUAL,
    UPPER
};

typedef struct{
    Framebuffer* colorBuffer;
    Framebuffer* zBuffer;
    float orthoMat[16];
    float cameraMat[16];
}ROP;

typedef struct {
    int16_t A[3];
    int16_t B[3];
    int16_t C[3];
}Primitive;

ROP* CreateROP(Framebuffer* colorBuffer,Framebuffer* zbuffer);
void DestroyRop(ROP* rop);

void SetOrtho(ROP* rop,uint16_t left,uint16_t right, uint16_t bottom, uint16_t top, uint16_t near, uint16_t far);

void SetPixel(ROP *rop,uint16_t x, uint16_t y, uint16_t color);
uint16_t GetPixel(ROP *rop,uint16_t x, uint16_t y);

void DrawLine(ROP* rop, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t color);


#endif //ROP_H
