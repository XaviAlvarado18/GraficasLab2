#include "SDL2/SDL.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring> // Agregamos esta línea para usar memcpy
#include <windows.h>


const int FRAMEBUFFER_WIDTH = 100;
const int FRAMEBUFFER_HEIGHT = 100;
const int FRAMEBUFFER_SIZE = FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int CELL_SIZE = WINDOW_WIDTH / FRAMEBUFFER_WIDTH;

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

Color framebuffer[FRAMEBUFFER_SIZE];
Color clearColor = {0, 0, 0, 255};
Color currentColor = {255, 255, 255, 255};

void clear() {
    for (int i = 0; i < FRAMEBUFFER_SIZE; i++) {
        framebuffer[i] = clearColor;
    }
}

void point(int x, int y) {
    if (x >= 0 && x < FRAMEBUFFER_WIDTH && y >= 0 && y < FRAMEBUFFER_HEIGHT) {
        framebuffer[y * FRAMEBUFFER_WIDTH + x] = currentColor;
    }
}

// Function to check the number of live neighbors for a given cell
int countLiveNeighbors(int x, int y) {
    int liveNeighbors = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue; // Skip the cell itself
            int neighborX = x + i;
            int neighborY = y + j;
            if (neighborX >= 0 && neighborX < FRAMEBUFFER_WIDTH && neighborY >= 0 && neighborY < FRAMEBUFFER_HEIGHT) {
                if (framebuffer[neighborY * FRAMEBUFFER_WIDTH + neighborX].r == 255) {
                    liveNeighbors++;
                }
            }
        }
    }
    return liveNeighbors;
}

// Function to apply the Game of Life rules to update the framebuffer
void updateFramebuffer() {
    Color newFramebuffer[FRAMEBUFFER_SIZE];
    for (int y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
        for (int x = 0; x < FRAMEBUFFER_WIDTH; x++) {
            int liveNeighbors = countLiveNeighbors(x, y);
            if (framebuffer[y * FRAMEBUFFER_WIDTH + x].r == 255) {
                // Cell is alive
                if (liveNeighbors < 2 || liveNeighbors > 3) {
                    // Underpopulation or overpopulation, cell dies
                    newFramebuffer[y * FRAMEBUFFER_WIDTH + x] = clearColor;
                } else {
                    // Survival, cell lives on
                    newFramebuffer[y * FRAMEBUFFER_WIDTH + x] = currentColor;
                }
            } else {
                // Cell is dead
                if (liveNeighbors == 3) {
                    // Reproduction, cell becomes alive
                    newFramebuffer[y * FRAMEBUFFER_WIDTH + x] = currentColor;
                } else {
                    // Cell remains dead
                    newFramebuffer[y * FRAMEBUFFER_WIDTH + x] = clearColor;
                }
            }
        }
    }
    // Update the framebuffer with the new state
    std::memcpy(framebuffer, newFramebuffer, sizeof(framebuffer));
}

void renderBuffer(SDL_Renderer* renderer) {
    SDL_Texture* texture = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_STREAMING,
        FRAMEBUFFER_WIDTH, 
        FRAMEBUFFER_HEIGHT
    );

    SDL_UpdateTexture(
        texture, 
        NULL, 
        framebuffer, 
        FRAMEBUFFER_WIDTH * sizeof(Color)
    );

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_DestroyTexture(texture);
}

void render(SDL_Renderer* renderer) {
    // Update the framebuffer with the Game of Life rules
    updateFramebuffer();

    // Render the framebuffer to the screen
    renderBuffer(renderer);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("Game of Life", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Initialize the random seed for the initial state of the Game of Life
    std::srand(std::time(nullptr));

    // Randomly populate the initial state of the framebuffer
    for (int y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
        for (int x = 0; x < FRAMEBUFFER_WIDTH; x++) {
            if (std::rand() % 2 == 0) {
                point(x, y);
            }
        }
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Call our render function
        render(renderer);

        // Present the framebuffer to the screen
        SDL_RenderPresent(renderer);

        // Delay to limit the frame rate
        SDL_Delay(100);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
