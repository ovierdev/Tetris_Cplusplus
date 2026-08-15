#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <iostream>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 800;

constexpr int BOARD_COLUMNS = 10;
constexpr int BOARD_ROWS = 20;
constexpr float CELL_SIZE = 30.0f;

constexpr float BOARD_WIDTH = BOARD_COLUMNS * CELL_SIZE;
constexpr float BOARD_HEIGHT = BOARD_ROWS * CELL_SIZE;
constexpr float BOARD_X = (WINDOW_WIDTH - BOARD_WIDTH) / 2.0f;
constexpr float BOARD_Y = (WINDOW_HEIGHT - BOARD_HEIGHT) /2.0f;

void drawBoard(SDL_Renderer* renderer){
    for (int row = 0; row < BOARD_ROWS; ++row){
        for(int column = 0; column < BOARD_COLUMNS; ++column){
            SDL_FRect cell{
                BOARD_X + column * CELL_SIZE,
                BOARD_Y + row * CELL_SIZE,
                CELL_SIZE,
                CELL_SIZE
            };
            SDL_RenderRect(renderer, &cell);
        }
    }
}

int main(){
    if (!SDL_Init(SDL_INIT_VIDEO)){
        std::cerr << "Error inicializando SDL: "
            << SDL_GetError()
            << '\n';
        return 1;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if(!SDL_CreateWindowAndRenderer("C++ Tetris", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)){
        std::cerr << "Error creando la ventana: "
            << SDL_GetError()
            << '\n';
        SDL_Quit();
        return 1;
    }

    bool running = true;

    while(running){
        SDL_Event event;

        while (SDL_PollEvent(&event)){
            if (event.type == SDL_EVENT_QUIT){
                running = false;
            }
        }
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        drawBoard(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
