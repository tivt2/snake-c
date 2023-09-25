#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>

typedef struct Cell {
    int x;
    int y;
} Cell;

struct Game {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int target_fps;
    int width;
    int height;

    int is_game_running;
    int last_tick;

    Cell **snake;
    int length;
    int dir;
    int snake_size;
} Game;

Cell *create_cell(int x, int y){
    Cell *cell = (Cell *)malloc(sizeof(Cell));
    cell->x = x;
    cell->y = y;
    return cell;
}

void initialize_game(int width, int height, int fps) {
    Game.window = NULL;
    Game.renderer = NULL;
    Game.target_fps = 1000/fps;
    Game.width = width;
    Game.height = height;

    Game.is_game_running = 0;
    Game.last_tick = 0;
    Game.length = 4;
    Game.snake = (Cell **)malloc(40 * sizeof(Cell *));
    Game.snake[0] = create_cell(100, 100);
    Game.snake[1] = create_cell(80, 100);
    Game.snake[2] = create_cell(60, 100);
    Game.snake[3] = create_cell(40, 100);
    Game.dir = 1;
    Game.snake_size = 20;
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
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    Game.is_game_running = 0;
                    break;
                case SDLK_UP:
                    Game.dir = 0;
                    break;
                case SDLK_RIGHT:
                    Game.dir = 1;
                    break;
                case SDLK_DOWN:
                    Game.dir = 2;
                    break;
                case SDLK_LEFT:
                    Game.dir = 3;
                    break;
            }
            break;
    }
}

void check_colision() {
    for (int i = 1; i < Game.length; i++) {
        if (Game.snake[0]->x == Game.snake[i]->x &&
            Game.snake[0]->y == Game.snake[i]->y) {
            Game.is_game_running = 0;
        }
    }
}

void update() {
    check_colision();
    int delay = Game.target_fps - (SDL_GetTicks() - Game.last_tick);
    if (delay > 0) {
        SDL_Delay(delay);
    }
    Game.last_tick = SDL_GetTicks();

    int headX = Game.snake[0]->x;
    int headY = Game.snake[0]->y;
    switch (Game.dir) {
        case 0:
            Game.snake[0]->y -= Game.snake_size;
            if (Game.snake[0]->y < 0) {
                Game.snake[0]->y += Game.height;
            }
        break;
        case 1:
            Game.snake[0]->x += Game.snake_size;
            Game.snake[0]->x %= Game.width;
        break;
        case 2:
            Game.snake[0]->y += Game.snake_size;
            Game.snake[0]->y %= Game.height;
        break;
        case 3:
            Game.snake[0]->x -= Game.snake_size;
            if (Game.snake[0]->x < 0) {
                Game.snake[0]->x += Game.width;
            }
        break;
    }

    if (Game.length > 1) {
        Game.snake[Game.length-1]->x = headX;
        Game.snake[Game.length-1]->y = headY;

        Cell *lastCell = Game.snake[Game.length-1];
        for (int i = Game.length - 1; i > 1; i--) {
            Game.snake[i] = Game.snake[i - 1];
        }
        Game.snake[1] = lastCell;
    }
}

void draw_snake() {
    SDL_SetRenderDrawColor(Game.renderer, 0, 255, 0, 255);
    for (int i = 0; i < Game.length; i++) {
        SDL_RenderFillRect(
            Game.renderer,
            &(SDL_Rect){
            .x = Game.snake[i]->x,
            .y = Game.snake[i]->y,
            .w = Game.snake_size,
            .h = Game.snake_size,
            });
    }
}

void draw_food() {
}

void draw() {
    SDL_SetRenderDrawColor(Game.renderer, 0, 0, 0, 255);
    SDL_RenderClear(Game.renderer);
    
    draw_snake();
    draw_food();

    SDL_RenderPresent(Game.renderer);
}

int main() {
    initialize_game(800, 600, 24);
    Game.is_game_running = initialize_window();

    printf("Game is running...\n");
    while (Game.is_game_running) {
        read_input();
        update();
        draw();
    }

    destroy_window();

    return 0;
}

