#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../include/display.h"

bool is_running = false;

void setup(void) {
    // Allocate the memory in bytes to holde the entire color buffer
    color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);

    if (!color_buffer) {
        fprintf(stderr, "Error allocating memory for the color buffer\n");
        is_running = false;
    }

    // Create a SDL texture that is used to display the color buffer
    color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING, window_width, window_height);

    if (!color_buffer_texture) {
        fprintf(stderr, "Error creating the SDL Texture\n");
        is_running = false;
    }
}

void process_input(void) {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        // 'x' button of the window
        case SDL_QUIT:
            is_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) 
                is_running = false;
            break;
    }
}

void update(void) {

}

void render(void) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);

    draw_grid_lines(100, 100);
    draw_rect(1500, 500, 300, 170, 0xFFFF00FF);

    render_color_buffer();
    clear_color_buffer(0xFF000000);
    
    SDL_RenderPresent(renderer);
}

int main(int argc, char *argv[]) {
    
    is_running = initialize_window();

    setup();

    while(is_running) {
        process_input();
        update();
        render();
    }

    destroy_window();

    return 0;
}
