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

SDL_Texture* loadTexture(const string &path, SDL_Renderer *ren){
    SDL_Texture *tex = nullptr;
    SDL_Surface *bmp = SDL_LoadBMP(path.c_str());

    if (bmp != nullptr) {
        tex = SDL_CreateTextureFromSurface(ren, bmp);
        SDL_FreeSurface(bmp);

        if (tex == nullptr) {
            logError("Failed to create textures");
        }
    } else {
        logError("Failed to load image");
    }
    return tex;
}

int main() {
    
    cout << " Starting pong ! " << endl;
    bool running = true;
    int options;

    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        logError("Failed to initialize SDL");
    }


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
