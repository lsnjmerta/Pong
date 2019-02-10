//
// Created by Jakub Merta on 2019-02-10.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

#include <SDL2/SDL.h>
using namespace std;
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "pong.hpp"
#include "util.hpp"

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

    Init(&ren,&win);

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
        }
    }

    return 0;
}
