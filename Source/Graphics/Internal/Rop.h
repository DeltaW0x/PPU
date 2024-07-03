#pragma once
#include "Renderbuffer.h"
#include "Math.h"

typedef struct {
    uint8_t* data;
}Depthbuffer;


typedef struct{
    Renderbuffer* colorBuffer;
    Depthbuffer* Depthbuffer;
    float projectionMat [16];
}Rop;

typedef struct {
    SVector Pos1,Pos2,Pos3;
}Primitive;

Rop* CreateRop(Renderbuffer* colorBuffer);
void DestroyRop(Rop* rop);

void DrawLine(Rop* rop, SVector p1, SVector p2,uint16_t color);
void DrawPrimitive(Rop* rop, Primitive primitive,uint16_t color);