#include "Renderbuffer.h"

#include <stdlib.h>

Renderbuffer* CreateRenderbuffer(uint32_t width,uint32_t height){
    Renderbuffer* rb = malloc(sizeof(Renderbuffer));
    rb->data = calloc(width*height,sizeof(uint16_t));
    rb->width = width;
    rb->height = height;
    return rb;
}

void DestroyRenderBuffer(Renderbuffer *renderbuffer) {
    free(renderbuffer->data);
}
