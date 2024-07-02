
#include "ROP.h"

#include <stdlib.h>


static void swapInt(uint16_t* a, uint16_t*b) {
    uint16_t temp = *a;
    *a = *b;
    *b = temp;
}
ROP * CreateROP(Framebuffer *colorBuffer, Framebuffer *zbuffer) {
    ROP* rop = malloc(sizeof(ROP));
    rop->colorBuffer = colorBuffer;
    rop->zBuffer = zbuffer;
    return rop;
}

void DestroyRop(ROP *rop) {
    free(rop);
}

void SetOrtho(ROP *rop, uint16_t left, uint16_t right, uint16_t bottom, uint16_t top, uint16_t near, uint16_t far) {
    rop->orthoMat[0] = 2 / (right - left);
    rop->orthoMat[3] = -(right+left)/(right-left);
    rop->orthoMat[5] = 2 / (top - bottom);
    rop->orthoMat[7] = -(top+bottom)/(top-bottom);
    rop->orthoMat[10] = -2/(far-near);
    rop->orthoMat[11] = -(far+near)/(far-near);
    rop->orthoMat[15] = 1;
}

void SetPixel(ROP* rop,uint16_t x, uint16_t y, uint16_t color) {
    if(x <= rop->colorBuffer->width && y <= rop->colorBuffer->height) {
        rop->colorBuffer->data[x + rop->colorBuffer->width*y] = color;
    }
}

uint16_t GetPixel(ROP* rop, uint16_t x, uint16_t y) {
    return rop->colorBuffer->data[x + rop->colorBuffer->width*y];
}

void DrawLine(ROP *rop, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color) {
    uint8_t steep =0;
    if (abs(x0-x1)<abs(y0-y1)) {
        swapInt(&x0, &y0);
        swapInt(&x1, &y1);
        steep = 1;
    }
    if (x0>x1) {
        swapInt(&x0, &x1);
        swapInt(&y0, &y1);
    }
    int16_t dx = x1-x0;
    int16_t dy = y1-y0;
    int16_t derror2 = abs(dy)*2;
    int16_t error2 = 0;
    uint16_t y = y0;
    for (int x=x0; x<=x1; x++) {
        if (steep) {
            SetPixel(rop,y, x, color);
        } else {
            SetPixel(rop,x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1>y0?1:-1);
            error2 -= dx*2;
        }
    }
}
