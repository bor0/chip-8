/*
This file is part of CHIP-8 VM.

CHIP-8 VM is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

CHIP-8 VM is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with CHIP-8 VM. If not, see <http://www.gnu.org/licenses/>.

*/
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include <math.h>

#include "chip8_sdl.h"
#include "cpu.h"
#include "keyboard.h"

#define ASSET_WIDTH 16
#define ASSET_HEIGHT 16
#define WIDTH 64
#define HEIGHT 32

static SDL_Surface *_img_loader(char *file) {
    SDL_Surface *pic;

    pic = IMG_Load(file);

    if (pic == NULL) {
        return NULL;
    }

    pic = SDL_DisplayFormat(pic);

    return pic;
}

static void _render(SDL_Surface *screen, SDL_Surface **assets, struct cpu *CPU) {
    SDL_Rect tmp = { 0, 0, ASSET_WIDTH, ASSET_HEIGHT };
    int i, j;

    SDL_FillRect(screen, NULL, 0);

    for (i=0;i<WIDTH;i++) {
        for (j=0;j<HEIGHT;j++) {
            tmp.x = i * ASSET_WIDTH;
            tmp.y = j * ASSET_HEIGHT;
            SDL_BlitSurface(assets[(int)CPU->display[i * HEIGHT + j]], NULL, screen, &tmp);
        }
    }

    SDL_Flip(screen);
}

void cpu_SDL_loop(struct cpu *CPU) {
    int ret;
    SDL_Event event;
    SDL_Surface *screen, *assets[2];
    /*Mix_Music *mus = NULL;*/

    if (SDL_Init(SDL_INIT_VIDEO) < 0) return;

    SDL_WM_SetCaption("CHIP8 VM by Boro Sitnikovski", NULL);

    screen = SDL_SetVideoMode(ASSET_WIDTH * WIDTH, ASSET_HEIGHT * HEIGHT, 32, SDL_DOUBLEBUF|SDL_HWSURFACE|SDL_ANYFORMAT);

    assets[0] = _img_loader("square.png");
    assets[1] = _img_loader("square2.png");

    if (SDL_Init(SDL_INIT_AUDIO) < 0) return;

    /* setup audio mode */
    /*if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640) != -1) {
        mus = Mix_LoadMUS("music/untzuntz.ogg");
        Mix_PlayMusic(mus, -1);
    }*/

    while (!CPU->halt) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                keyboardDown(event.key.keysym.sym, CPU);
                break;
            case SDL_KEYUP:
                keyboardUp(event.key.keysym.sym, CPU);
                break;
            case SDL_QUIT:
                keyboardDown(27, CPU);
                break;
            default:
                break;
            }
        }

        ret = cpu_cycle(CPU);

        if (ret & 1) {
            _render(screen, assets, CPU);
        }
        if (ret & 2) {
            /* _audio(); ... */
        }

    }

    /*if (mus) {
        Mix_CloseAudio();
        Mix_FreeMusic(mus);
    }*/
}
