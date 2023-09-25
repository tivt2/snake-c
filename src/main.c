#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <SDL2/SDL.h>

typedef struct Snake {
    
} Snake;

struct Game {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int target_fps;
    int width;
    int height;

    int is_game_running;
    Snake *snake;
    int points;
} Game;

void initialize_game(int width, int height, int fps) {
    Game.window = NULL;
    Game.renderer = NULL;
    Game.target_fps = fps;
    Game.width = width;
    Game.height = height;

    Game.is_game_running = 0;
    Game.snake = NULL;
    Game.points = 0;
}

int initialize_window(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error while initializing SDL\n");
        return 0;
    }

    Game.window = SDL_CreateWindow(
        "Snake",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        Game.width,
        Game.height,
        SDL_WINDOW_BORDERLESS
    );
    if (!Game.window) {
        fprintf(stderr, "Error while creating window\n");
        return 0;
    }

    Game.renderer = SDL_CreateRenderer(Game.window, -1, 0);
    if (!Game.renderer) {
        fprintf(stderr, "Error while creating renderer\n");
        return 0;
    }

    return 1;
}

void destroy_window() {
    SDL_DestroyRenderer(Game.renderer);
    SDL_DestroyWindow(Game.window);
    SDL_Quit();
}

void read_input() {
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:
            Game.is_game_running = 0;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                Game.is_game_running = 0;
            }
            break;
    }
}

void update() {
}

void draw() {
}

int main() {
    initialize_game(800, 600, 60);
    Game.is_game_running = initialize_window();

    while (Game.is_game_running) {
        read_input();
        update();
        draw();
    }

    destroy_window();

    return 0;
}

