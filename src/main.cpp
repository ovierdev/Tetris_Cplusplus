
/*
 * TODO:
 * v0.0.5: impedir que la pieza atraviese las paredes y el suelo mediante canMove().
 *
 */
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_timer.h>
#include <iostream>
#include <array>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 800;

constexpr int BOARD_COLUMNS = 10;
constexpr int BOARD_ROWS = 20;
constexpr float CELL_SIZE = 30.0f;

constexpr float BOARD_WIDTH = BOARD_COLUMNS * CELL_SIZE;
constexpr float BOARD_HEIGHT = BOARD_ROWS * CELL_SIZE;
constexpr float BOARD_X = (WINDOW_WIDTH - BOARD_WIDTH) / 2.0f;
constexpr float BOARD_Y = (WINDOW_HEIGHT - BOARD_HEIGHT) /2.0f;

constexpr Uint64 DROP_INTERVAL_MS = 500;

struct Block {
    int x;
    int y;
};

struct Piece{
    int x;
    int y;
    int rotation;
    std::array<Block, 4> blocks;
};

constexpr std::array<std::array<Block, 4>, 4> T_ROTATIONS{{
    {{
        {1, 0},
        {0, 1},
        {1, 1},
        {2, 1}
    }},
    {{
        {1, 0},
        {1, 1},
        {2, 1},
        {1, 2}
    }},
    {{
        {0, 1},
        {1, 1},
        {2, 1},
        {1, 2}
    }},
    {{
        {1, 0},
        {0, 1},
        {1, 1},
        {1, 2}
    }}
}};
Piece rotatedPiece(const Piece& piece){
    Piece rotated = piece;

    rotated.rotation =
        (piece.rotation + 1) % 4;

    rotated.blocks =
        T_ROTATIONS[rotated.rotation];

    return rotated;
}


using Board = std::array<std::array<bool, BOARD_COLUMNS>, BOARD_ROWS>;

constexpr std::array<Block, 4> T_PIECE{{
    {1, 0},
    {0, 1},
    {1, 1},
    {2, 1}
}};

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

void drawPiece(
    SDL_Renderer* renderer,
    const Piece& piece
)
{
    for (const Block& block : piece.blocks) {

        const int boardX = piece.x + block.x;
        const int boardY = piece.y + block.y;

        SDL_FRect rect{
            BOARD_X + boardX * CELL_SIZE,
            BOARD_Y + boardY * CELL_SIZE,
            CELL_SIZE,
            CELL_SIZE
        };

        SDL_RenderFillRect(renderer, &rect);
    }
}

bool canPlace(const Board& board, const Piece& piece){
    for (const Block& block : piece.blocks){
        const int x = piece.x + block.x;
        const int y = piece.y + block.y;

        if (x < 0 || x >= BOARD_COLUMNS || y >= BOARD_ROWS){
            return false;
        }

        if (y>= 0 && board[y][x]){
            return false;
        }
    }
    return true;
}

bool canMove( const Board& board, const Piece& piece, int offsetX, int offsetY){
    Piece moved = piece;
    moved.x = piece.x + offsetX;
    moved.y = piece.y + offsetY;

    return canPlace(board, moved);
}


void lockPiece(Board& board, const Piece& piece){
    for (const Block& block: piece.blocks){
        const int boardX = piece.x + block.x;
        const int boardY= piece.y + block.y;

        if (boardX >= 0 && boardX < BOARD_COLUMNS && boardY >= 0){
            board[boardY][boardX] = true;
        }
    }
}

void drawLockedBlocks(SDL_Renderer* renderer, const Board& board){
    for (int row = 0; row < BOARD_ROWS; ++row){
        for (int column =0; column < BOARD_COLUMNS; ++column){
            if (!board[row][column]){
                continue;
            }
            SDL_FRect rect{
                BOARD_X + column * CELL_SIZE,
                BOARD_Y + row * CELL_SIZE,
                CELL_SIZE,
                CELL_SIZE
            };
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

int main(){

    Board board{};

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

    Piece piece{
        3,
        0,
        0,
        T_ROTATIONS[0]
    };

    Uint64 lastDropTime = SDL_GetTicks();

    while(running){
        SDL_Event event;

        while (SDL_PollEvent(&event)){
            if (event.type == SDL_EVENT_QUIT){
                running = false;
            }
            if ( event.type == SDL_EVENT_KEY_DOWN){
                if (event.key.key == SDLK_LEFT){
                    if (canMove(board, piece, -1, 0)){
                        piece.x--;
                    }
                }
                if (event.key.key == SDLK_RIGHT){
                    if(canMove(board, piece, 1, 0)){
                        piece.x++;
                    }
                }
                if (event.key.key == SDLK_DOWN){
                    if(canMove(board, piece, 0, 1)){
                        piece.y++;
                    }
                }
                if (event.key.key == SDLK_UP){
                    Piece rotated = rotatedPiece(piece);

                    if (canPlace(board, rotated)){
                        piece =rotated;
                    }
                }
            }
        }

        const Uint64 currentTime = SDL_GetTicks();

        if (currentTime - lastDropTime >= DROP_INTERVAL_MS) {
            if (canMove(board, piece, 0, 1)) {
                piece.y++;
            } else {
                lockPiece(board, piece);
                piece = Piece{
                    3,
                    0,
                    0,
                    T_ROTATIONS[0]
                };
            }
            lastDropTime = currentTime;
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        drawBoard(renderer);
        SDL_SetRenderDrawColor(
            renderer,
            70, 150, 210, 255
        );

        drawLockedBlocks(renderer, board);

        SDL_SetRenderDrawColor(
            renderer,
            180, 80, 180, 255
        );

        drawPiece(renderer, piece);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
