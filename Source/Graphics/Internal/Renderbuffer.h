#pragma once
#include <stdint.h>

typedef struct {
    uint16_t* data;
    uint32_t width,height;
} Renderbuffer;

Renderbuffer* CreateRenderbuffer(uint32_t width,uint32_t height);
void DestroyRenderBuffer(Renderbuffer* renderbuffer);