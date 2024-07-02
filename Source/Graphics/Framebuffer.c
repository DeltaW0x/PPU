

#include "Framebuffer.h"
#include <stdlib.h>

Framebuffer* CreateFramebuffer(uint16_t width, uint16_t height) {
    Framebuffer* fb = malloc(sizeof(Framebuffer));
    fb->data = calloc(width*height,sizeof(int16_t));
    fb->width = width;
    fb->height = height;
    return fb;
}

void DestroyFramebuffer(Framebuffer *framebuffer) {
    free(framebuffer->data);
    free(framebuffer);
}



