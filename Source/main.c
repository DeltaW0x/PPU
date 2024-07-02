#include <stdio.h>
#include <SDL2/SDL.h>
#include "Graphics/ROP/ROP.h"
#define MATRIX_WIDTH 128
#define MATRIX_HEIGHT 32
#define SCALER 8
int main(void) {
    Framebuffer* colorBuffer = CreateFramebuffer(MATRIX_WIDTH,MATRIX_HEIGHT);
    Framebuffer* zbuffer = CreateFramebuffer(MATRIX_WIDTH,MATRIX_HEIGHT);

    ROP* rop = CreateROP(colorBuffer,zbuffer);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow(
        "PPU Debugger",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        MATRIX_WIDTH * SCALER,
        MATRIX_HEIGHT * SCALER,
        0);
    SDL_Surface* displaySr = SDL_CreateRGBSurfaceWithFormatFrom(colorBuffer->data,
        colorBuffer->width,
        colorBuffer->height,
        sizeof(uint16_t),
        colorBuffer->width * sizeof(uint16_t),
        SDL_PIXELFORMAT_RGB565);
    SDL_Surface* windowSurface = SDL_GetWindowSurface(window);

    SDL_Rect windowRect;
    windowRect.x = 0;
    windowRect.y = 0;
    windowRect.w = MATRIX_WIDTH * SCALER;
    windowRect.h = MATRIX_HEIGHT * SCALER;

    SDL_Rect framebufferRect;
    framebufferRect.x = 0;
    framebufferRect.y = 0;
    framebufferRect.w = MATRIX_WIDTH;
    framebufferRect.h = MATRIX_HEIGHT;

    while(1) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    goto quit_proc;
                default:
                    break;
            }
        }
        DrawLine(rop,0,0,127,31,0xFFFF);
        SDL_BlitScaled(displaySr,&framebufferRect,windowSurface,&windowRect);
        SDL_UpdateWindowSurface(window);
    }

    quit_proc:
        DestroyRop(rop);
        DestroyFramebuffer(colorBuffer);
        DestroyFramebuffer(zbuffer);
        SDL_FreeSurface(displaySr);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
}
