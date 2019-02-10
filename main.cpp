//
// Created by Jakub Merta on 2019-02-10.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "setupFunctions.hpp"

#include "pong.hpp"
#include "util.hpp"
#include "game.hpp"

void defineFields(int mode, int &board_width, int &board_height, SDL_Texture *&table, SDL_Texture *&net,
                  SDL_Texture *&racket, SDL_Color &whiteColor, SDL_Rect &b_rect, player &p1, player &p2, ball &b,
                  uint &prevTime, bool &running, int &frames, float &fps, const Uint8 *&keystates);

using namespace std;


int main(int argc, char *argv[]) {
    int board_width;
    int board_height;
    SDL_Texture *table;
    SDL_Texture *net;
    SDL_Texture *racket;
    SDL_Color whiteColor;
    SDL_Rect b_rect;
    player p1;
    player p2;
    ball b;
    uint prevTime;
    bool running;
    int frames;
    float fps;
    char buffer[512];
    const Uint8 *keystates;
    SDL_Event e;

    InitSDL(&ren, &win);
    int mode = getChoosenOption();
    SDL_RenderClear(ren);
    defineFields(mode, board_width, board_height, table, net, racket, whiteColor,
                 b_rect, p1, p2, b, prevTime, running,
                 frames,
                 fps, keystates);


    while (running && mode != 3) {
        // FPS Calculation
        ++frames;
        uint currTime = SDL_GetTicks();
        float elapsed = (currTime - prevTime);

        if (elapsed > 100) {
            fps = round(frames / (elapsed / 1000.0));
            frames = 0;
            prevTime = currTime;
        }

        running = runGame(fps, mode, e, keystates, p1, p2, b, racket, b_rect, buffer, whiteColor, net);
    }

    SDL_DestroyTexture(racket);
    SDL_DestroyTexture(net);
    SDL_DestroyTexture(table);
    ClearAll(&ren, &win);
    return 0;
}


void InitSDL(SDL_Renderer **ren, SDL_Window **win) {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
        logError("Failed to init SDL");

    *win = SDL_CreateWindow(
            "Pong",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            WIDTH, HEIGHT,
            SDL_WINDOW_SHOWN
    );
    if (*win == nullptr)
        logError("Failed to create SDL Window");

    *ren = SDL_CreateRenderer(*win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (*ren == nullptr)
        logError("Failed to create SDL Renderer");

    const int flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (IMG_Init(flags) != flags)
        logError("Failed to load the Image loading extensions");

    if (TTF_Init() != 0)
        logError("Failed to load TTF extension");
}

void ClearAll(SDL_Renderer **ren, SDL_Window **win) {
    SDL_DestroyRenderer(*ren);
    SDL_DestroyWindow(*win);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}


void defineFields(int mode, int &board_width, int &board_height, SDL_Texture *&table, SDL_Texture *&net,
                  SDL_Texture *&racket, SDL_Color &whiteColor, SDL_Rect &b_rect, player &p1, player &p2, ball &b,
                  uint &prevTime, bool &running, int &frames, float &fps, const Uint8 *&keystates) {
    table = IMG_LoadTexture(ren, "images/table.png");
    net = IMG_LoadTexture(ren, "images/net.png");
    racket = IMG_LoadTexture(ren, "images/racket.png");
    whiteColor = {255, 255, 255};
    prevTime = SDL_GetTicks();
    running = true;
    frames = 0;
    fps = 0;
    keystates = SDL_GetKeyboardState(NULL);
    SDL_QueryTexture(racket, NULL, NULL, &board_width, &board_height);


    // Define Ball
    b.x = WIDTH / 2;
    b.y = HEIGHT / 2;
    b.speed = BALL_INIT_SPEED;
    b.vx = (random() % 2 == 0) ? BALL_INIT_SPEED : -1 * BALL_INIT_SPEED;
    b.vy = -0.5f;
    b_rect.w = BALL_HEIGHT;
    b_rect.h = BALL_HEIGHT;

    // Define Players
    p1.score = p2.score = 0;
    p1.pos.w = p2.pos.w = board_width;
    p1.pos.h = p2.pos.h = 150;
    p1.speed = 10;
    if (mode == 1) {
        p2.speed = 3.5f;
    } else {
        p2.speed = 10;
    }
    p1.pos.x = board_width / 2 + 10;
    p2.pos.x = WIDTH - p2.pos.w - 10 - p2.pos.w / 2;

    p1.pos.y = HEIGHT / 2 - p1.pos.h / 2;
    p2.pos.y = HEIGHT / 2 - p2.pos.h / 2;
    char buffer[512];
}
