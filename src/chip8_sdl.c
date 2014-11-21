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

#include "chip8_sdl.h"
#include "cpu.h"
#include "keyboard.h"

static void _render(SDL_Surface *screen, struct cpu *CPU)
{
    SDL_Rect tmp = { 0, 0, ASSET_WIDTH, ASSET_HEIGHT };
    int i, j;

    SDL_FillRect(screen, NULL, 0xFFFFFF);

    for (i = 0; i < WIDTH; i++) {
        for (j = 0; j < HEIGHT; j++) {
            tmp.x = i * ASSET_WIDTH;
            tmp.y = j * ASSET_HEIGHT;
            if (CPU->display[i * HEIGHT + j]) {
                SDL_FillRect(screen, &tmp, SDL_MapRGB(screen->format, 0, 0, 0));
            }
        }
    }

    SDL_Flip(screen);
}

void cpu_SDL_loop(struct cpu *CPU)
{
    int count = 0, pause = 0, sound = 1, debug = 0;
    Uint32 lasttick;
    SDL_Event event;
    SDL_Surface *screen;
    Mix_Chunk *beep = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) return;

    SDL_WM_SetCaption("CHIP-8 VM by Boro Sitnikovski", NULL);

    screen = SDL_SetVideoMode(ASSET_WIDTH * WIDTH, ASSET_HEIGHT * HEIGHT, 32, SDL_DOUBLEBUF|SDL_HWSURFACE|SDL_ANYFORMAT);

    SDL_FillRect(screen, NULL, 0xFFFFFF);
    if (SDL_Init(SDL_INIT_AUDIO) < 0) return;

    /* setup audio mode */
    if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640) != -1) {
        beep = Mix_LoadWAV("beep.wav");
    }

    lasttick = SDL_GetTicks();

    while (!CPU->halt) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                keyboardPress(CPU, event.key.keysym.sym, 1);
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == 'p') {
                    pause = !pause;
                } else if (event.key.keysym.sym == 'm') {
                    sound = !sound;
                } else if (event.key.keysym.sym == '\b') {
                    cpu_reset(CPU);
                    count = 0;
                    pause = 0;
                } else if (event.key.keysym.sym == '\r') {
                    debug = !debug;
                } else if (event.key.keysym.sym == 27) {
                    CPU->halt = 1;
                }
                else {
                    keyboardPress(CPU, event.key.keysym.sym, 0);
                }
                break;
            case SDL_QUIT:
                CPU->halt = 1;
                break;
            default:
                break;
            }
        }

        if (pause) continue;

        if (count < 12) {
            if (debug) {
                printf("0x%.4X: %.4X [I=%.4X] [SP=%.4X]\n", CPU->registers.PC, calc_opcode(CPU), CPU->registers.I, CPU->registers.SP);
            }
            cpu_cycle(CPU);
            count++;
        }

        if (SDL_GetTicks() - lasttick >= 1000/50) {
            if (CPU->delay_timer > 0) CPU->delay_timer--;
            if (CPU->sound_timer > 0) {
                if (CPU->sound_timer == 1 && sound) {
                    Mix_PlayChannel(-1, beep, 0);
                }
                CPU->sound_timer--;
            }

            lasttick = SDL_GetTicks();

            _render(screen, CPU);
            count = 0;
        }
    }

    if (beep) {
        Mix_CloseAudio();
        Mix_FreeChunk(beep);
    }
}
