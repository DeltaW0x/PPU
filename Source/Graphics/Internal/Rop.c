#include "Rop.h"

#include <stdlib.h>

#define ORTHO_H(x) x = ((x)+1 *128/2)
#define ORTHO_V(y) y = ((y)+1 *128/2)

static void SwapInt(int32_t *x, int32_t *y) {
    int32_t temp = *x;
    *x = *y;
    *y = temp;
}

static void SwapPrimitive(Primitive *prim1, Primitive *prim2) {
    Primitive temp = *prim1;
    *prim1 = *prim2;
    *prim2 = temp;
}

static void SwapSVector(SVector *v1, SVector *v2) {
    SVector temp = *v1;
    *v1 = *v2;
    *v2 = temp;
}


Rop *CreateRop(Renderbuffer *colorBuffer) {
    Rop *rop = malloc(sizeof(Rop));
    rop->colorBuffer = colorBuffer;
    rop->Depthbuffer = malloc(sizeof(Depthbuffer));
    rop->Depthbuffer->data = calloc(rop->colorBuffer->width * rop->colorBuffer->height, sizeof(uint8_t));
    return rop;
}

void DestroyRop(Rop *rop) {
    free(rop->Depthbuffer->data);
    free(rop->Depthbuffer);
    free(rop);
}

static void WritePixel(Rop *rop, int16_t x, int16_t y, uint32_t color) {
    if (x <= rop->colorBuffer->width && y <= rop->colorBuffer->height) {
        rop->colorBuffer->data[x + rop->colorBuffer->width * y] = color;
    }
}




void DrawLine(Rop *rop, SVector p1, SVector p2, uint16_t color) {
    uint8_t steep = 0;

    ORTHO_H(p1.x);
    ORTHO_H(p2.x);

    ORTHO_V(p1.y);
    ORTHO_V(p2.y);

    if (abs(p1.x - p2.x) < abs(p1.y - p2.y)) {
        SwapInt(&p1.x, &p1.y);
        SwapInt(&p2.x, &p2.y);
        steep = 1;
    }
    if (p1.x > p2.x) {
        SwapInt(&p1.x, &p2.x);
        SwapInt(&p1.y, &p2.y);
    }
    int32_t dx = p2.x - p1.x;
    int32_t dy = p2.y - p1.y;
    int32_t derror2 = abs(dy) * 2;
    int32_t error2 = 0;
    int32_t y = p1.y;
    for (int x = p1.x; x <= p2.x; x++) {
        if (steep) {
            WritePixel(rop, y, x, color);
        } else {
            WritePixel(rop, x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (p2.y > p1.y ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void DrawPrimitive(Rop *rop, Primitive primitive, uint16_t color) {

    ORTHO_H(primitive.Pos1.x);
    ORTHO_H(primitive.Pos2.x);
    ORTHO_H(primitive.Pos3.x);

    ORTHO_V(primitive.Pos1.y);
    ORTHO_V(primitive.Pos2.y);
    ORTHO_V(primitive.Pos3.y);

    if (primitive.Pos1.y > primitive.Pos2.y) SwapSVector(&primitive.Pos1, &primitive.Pos2);
    if (primitive.Pos1.y > primitive.Pos3.y) SwapSVector(&primitive.Pos1, &primitive.Pos3);
    if (primitive.Pos2.y > primitive.Pos3.y) SwapSVector(&primitive.Pos2, &primitive.Pos3);
    int total_height = primitive.Pos3.y - primitive.Pos1.y;
    for (int y = primitive.Pos1.y; y <= primitive.Pos2.y; y++) {
        int segment_height = primitive.Pos2.y - primitive.Pos1.y + 1;
        float alpha = (float) (y - primitive.Pos1.y) / total_height;
        float beta = (float) (y - primitive.Pos1.y) / segment_height; // be careful with divisions by zero
        SVector A;
        A.x = primitive.Pos1.x + (primitive.Pos3.x - primitive.Pos1.x) * alpha;
        A.y = primitive.Pos1.y + (primitive.Pos3.y - primitive.Pos1.y) * alpha;
        A.z = 0;

        SVector B;
        B.x = primitive.Pos1.x + (primitive.Pos2.x - primitive.Pos1.x) * beta;
        B.y = primitive.Pos1.y + (primitive.Pos2.y - primitive.Pos1.y) * beta;
        B.z = 0;
        if (A.x > B.x) SwapSVector(&A, &B);
        for (int j = A.x; j <= B.x; j++) {
            WritePixel(rop, j, y, color); // attention, due to int casts primitive.Pos1.y+i != A.y
        }
    }
    for (int y = primitive.Pos2.y; y <= primitive.Pos3.y; y++) {
        int segment_height = primitive.Pos3.y - primitive.Pos2.y + 1;
        float alpha = (float) (y - primitive.Pos1.y) / total_height;
        float beta = (float) (y - primitive.Pos2.y) / segment_height; // be careful with divisions by zero
        SVector A;
        A.x = primitive.Pos1.x + (primitive.Pos3.x - primitive.Pos1.x) * alpha;
        A.y = primitive.Pos1.y + (primitive.Pos3.y - primitive.Pos1.y) * alpha;
        A.z = 0;

        SVector B;
        B.x = primitive.Pos2.x + (primitive.Pos3.x - primitive.Pos2.x) * beta;
        B.y = primitive.Pos2.y + (primitive.Pos3.y - primitive.Pos2.y) * beta;
        B.z = 0;
        if (A.x > B.x) SwapSVector(&A, &B);
        for (int j = A.x; j <= B.x; j++) {
            WritePixel(rop, j, y, color); // attention, due to int casts primitive.Pos1.y+i != A.y
        }
    }
}
