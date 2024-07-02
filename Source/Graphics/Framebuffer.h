

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <stdint.h>
typedef struct{
    uint16_t* data;
    uint16_t width, height;
} Framebuffer;

Framebuffer* CreateFramebuffer(uint16_t width, uint16_t height);
void DestroyFramebuffer(Framebuffer* framebuffer);

#endif //FRAMEBUFFER_H
