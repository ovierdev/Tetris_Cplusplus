#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <iostream>

int main(){
    if (!SDL_Init(SDL_INIT_VIDEO)){
        std::cerr << "Error inicializando SDL: "
            << SDL_GetError()
            << '\n';
        return 1;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if(!SDL_CreateWindowAndRenderer("C++ Tetris", 800, 800, 0, &window, &renderer)){
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
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
