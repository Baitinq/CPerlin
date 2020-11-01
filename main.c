#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>

#include "include/perlin.h"

#define WIDTH 700
#define HEIGHT 500

//TODO: ADD ZOOM
//TODO: add rgb from args, size from args

int main(int argc, char** argv);
uint32_t getargb(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

int main(int argc, char** argv)
{
    SDL_Window* window = NULL;
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Perlin Noise Visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);
    SDL_Texture* sdlTexture = SDL_CreateTexture(renderer,
    SDL_PIXELFORMAT_ARGB8888,
    SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    SDL_SetTextureBlendMode(sdlTexture, SDL_BLENDMODE_BLEND);

    srand(time(NULL));

    static int seedx = 523213;//rand();
    static int seedy = 1;//rand();

    load:
    seedx += 10000;
    seedy += 50000;
    printf("Generating perlin noise...\n");
    uint32_t* pixels = malloc(sizeof *pixels * WIDTH * HEIGHT);
    for(int i = 0, counter = 0; i < HEIGHT; i++) //no counter?
        for(int j = 0; j < WIDTH; j++)
            pixels[counter++] = getargb(214, 56, 203, (perlin_noise(i + seedx, j + seedy)-(-1))*(255-0)/(1-(-1))+ 0);

    reload:
    SDL_UpdateTexture(sdlTexture, NULL, pixels, WIDTH * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    static double zoom_value = 1;
    printf("zoom is %f\n", zoom_value);
    SDL_Rect zoom = { 0 };
    zoom.h = HEIGHT * zoom_value;
    zoom.w = WIDTH * zoom_value;
    //zoom.x = -WIDTH;
    //zoom.y = -HEIGHT; //? https://wiki.libsdl.org/SDL_Rect
    SDL_RenderCopy(renderer, sdlTexture, NULL, &zoom);
    SDL_RenderPresent(renderer);

    while(1)
    {
        SDL_Event e;
                while (SDL_PollEvent(&e))
                {
                    switch(e.type)
                    {
                        case SDL_QUIT:
                            exit(0);
                            break;
                        case SDL_KEYDOWN:
                            switch(e.key.keysym.sym)
                            {
                                case SDLK_ESCAPE:
                                case SDLK_q: exit(0); break;
                                case SDLK_F5:
                                case SDLK_r: goto load; break;
                            }
                            break;
                       case SDL_MOUSEWHEEL:
                        if(e.wheel.y > 0) // scroll up
                        {
                            printf("scroll up! (%d, %d)\n", e.motion.x, e.motion.y);
                            zoom_value += 0.1;
                            goto reload;
                        }
                        else if(e.wheel.y < 0) // scroll down
                        {
                            printf("scroll down! (%d, %d)\n", e.motion.x, e.motion.y); //https://stackoverflow.com/questions/49111054/how-to-get-mouse-position-on-mouse-wheel-event
                            zoom_value -= 0.1;
                            zoom_value = zoom_value < 1.0 ? 1.0 : zoom_value;
                            goto reload;
                        }
                        break;
                    }
                }
    }

    free(pixels);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

uint32_t getargb(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    uint32_t res = 0;
    res |= a << 24;
    res |= b << 16;
    res |= g << 8;
    res |= r;

    return res;
}
