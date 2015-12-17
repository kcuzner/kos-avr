/**
 * Simulator for KOS using sdl
 */

#include <stdio.h>
#include <libgen.h>

#include <pthread.h>

#include <SDL.h>

#include "sim_avr.h"
#include "sim_elf.h"
#include "sim_gdb.h"

#define GDB_PORT 1234
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

static avr_t *avr;

static void *avr_run_thread(void * ig)
{
    while (1)
    {
        avr_run(avr);
    }

    return NULL;
}

static SDL_Window *init_window(void)
{
    SDL_Window *win = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Unable to initialize SDL: %s", SDL_GetError());
        exit(1);
    }

    win = SDL_CreateWindow("KOS Simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!win)
    {
        fprintf(stderr, "Unable to create SDL Window: %s", SDL_GetError());
        exit(1);
    }

    return win;
}

static char handle_event(SDL_Event *event)
{
    switch (event->type)
    {
        case SDL_QUIT:
            return 0;
    }

    return 1; //continue
}

int main(int argc, char** argv)
{
    elf_firmware_t f;
    SDL_Window *win;
    const char *fname = "kos.elf";
    SDL_Event event;

    if (elf_read_firmware(fname, &f) < 0)
    {
        exit(1);
    }

    avr = avr_make_mcu_by_name(f.mmcu);
    avr_init(avr);
    avr_load_firmware(avr, &f);

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Unable to initialize SDL: %s", SDL_GetError());
        exit(1);
    }

    win = SDL_CreateWindow("KOS Simulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!win)
    {
        fprintf(stderr, "Unable to create SDL Window: %s", SDL_GetError());
        exit(1);
    }

    while (SDL_WaitEvent(&event) >= 0)
    {
        if (!handle_event(&event))
            break;
    }

    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}
