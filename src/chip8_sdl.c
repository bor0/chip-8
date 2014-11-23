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
    int count = 0, pause = 0, sound = 1, debug = 0, step = 0, i, print_memory = 0;
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
                switch (event.key.keysym.sym) {
                case SDLK_p:
                    pause = !pause;
                    break;
                case SDLK_m:
                    sound = !sound;
                    break;
                case SDLK_BACKSPACE:
                    cpu_reset(CPU);
                    count = 0;
                    pause = 0;
                    break;
                case SDLK_F8:
                    step = 1;
                    debug = 1;
                    break;
                case SDLK_F9:
                    debug = 0;
                    break;
                case SDLK_F10:
                    if (debug) step = 1;
                    break;
                case SDLK_BACKQUOTE:
                    if (debug) print_memory = 1;
                    break;
                case SDLK_ESCAPE:
                    CPU->halt = 1;
                    break;
                default:
                    keyboardPress(CPU, event.key.keysym.sym, 0);
                    break;
                }
                break;
            case SDL_QUIT:
                CPU->halt = 1;
                break;
            default:
                break;
            }
        }

        if (pause) {
            continue;
        }

        if (debug) {
            if (print_memory) {
                printf("-- Printing memory contents --");
                for (i = 0; i < sizeof(CPU->memory); i+=2) {
                    if (i % 32 == 0) {
                        printf("\n%.4X: ", i);
                    }
                    printf("%.4X ", (CPU->memory[i] << 8) | CPU->memory[i+1]);
                }
                printf("\n-- Printing memory contents --\n");
                print_memory = 0;
                continue;
            }
            if (!step) {
                continue;
            }
            printf("%.4X: %.4X [I=%.4X] [SP=%.4X] [V0..V16=", CPU->registers.PC, calc_opcode(CPU), CPU->registers.I, CPU->registers.SP);
            for (i = 0; i < 16; i++) {
                printf("%.2X ", CPU->registers.v[i]);
            }
            printf("\b]\n");
            step = 0;
        }

        if (count < 12) {
            cpu_cycle(CPU);
            count++;
        }

        if (SDL_GetTicks() - lasttick >= 1000/50) { /* every 20 ms */
            if (CPU->delay_timer > 0) {
                CPU->delay_timer--;
            }

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
