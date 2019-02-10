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

#include "pong.hpp"
#include "util.hpp"

using namespace std;

typedef struct {
    float x;
    float y;
    float vx;
    float vy;
    float speed;
} ball;

typedef struct {
    SDL_Rect pos;
    int score;
    int speed;
} player;

float calc_angle(float y1, float y2, int height) {
    float rely = y1 + height/2 - y2;
    rely /= height/2.0;   // Normalise
    return rely * MAX_ANGLE;
}

int main(int argc, char* argv[]) {


    SDL_Event e;
    SDL_Renderer *ren = nullptr;
    SDL_Window *win = nullptr;
    SDL_GameController *controller = nullptr;

    SDL_Haptic *haptic = nullptr;

    InitSDL(&ren, &win);

    // Check for controller support
    if (SDL_NumJoysticks() == 1 && SDL_IsGameController(0)) {
        controller = SDL_GameControllerOpen(0);
        cout << "Found a controller: " << SDL_GameControllerName(controller) << endl;

        haptic = SDL_HapticOpen(0);

        SDL_HapticRumbleInit(haptic);
    }

    int board_width;
    int board_height;
    SDL_Texture *squareTex = IMG_LoadTexture(ren, "../images/pong_board.png");
    SDL_QueryTexture(squareTex, nullptr, nullptr, &board_width, &board_height);

    while (running){
        switch(options){
            case 1:
                // solo
                break;
            case 2:
                // 2 players
                break;

            case 3:
                // quit
                break;

                defaut:
                    break;
    SDL_Color whiteColor = {255, 255, 255};
    SDL_Surface *fpsCounter;

    SDL_Rect b_rect;
    b_rect.w = BALL_HEIGHT;
    b_rect.h = BALL_HEIGHT;

    // Define Players
    player p1;
    player p2;

    // Define Ball
    ball b;
    b.x = WIDTH / 2;
    b.y = HEIGHT / 2;
    b.speed = BALL_INIT_SPEED;
    b.vx = (rand() % 2 == 0)? BALL_INIT_SPEED : -1 * BALL_INIT_SPEED;
    b.vy = -0.5f;

    p1.score = p2.score = 0;
    p1.pos.w = p2.pos.w = board_width;
    p1.pos.h = p2.pos.h = 150;
    p1.speed = 10;
    p2.speed = 3.5;

    p1.pos.x = board_width/2 + 10;
    p2.pos.x = WIDTH - p2.pos.w- 10 - p2.pos.w/2;

    p1.pos.y = HEIGHT/2 - p1.pos.h/2;
    p2.pos.y = HEIGHT/2 - p2.pos.h/2;

    uint prevTime = SDL_GetTicks();
    bool running = false;
    int frames = 0;
    float fps;
    char buffer[512];
    const Uint8 *keystates = SDL_GetKeyboardState(nullptr);

    while(!running) {


        }
    }

    return 0;
}
