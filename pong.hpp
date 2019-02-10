//
// Created by Jakub Merta on 2019-02-10.
//

#ifndef GRA_PONG_H
#define GRA_PONG_H

#include <SDL2/SDL.h>

const int WIDTH = 640;
const int HEIGHT = 480;

const int CENTER = 5;
const float MAX_ANGLE = 3.142f /5.0f;

const float BALL_MAXSPEED = 8.0f;
const float BALL_ACCELERATE = 1.05f;
const float BALL_INIT_SPEED = 4.0f;
const int BALL_WIDTH = 10;
const int BALL_HEIGHT = 10;

void InitSDL(SDL_Renderer **ren, SDL_Window **win);
void ClearAll(SDL_Renderer **ren, SDL_Window **win);


#endif //GRA_PONG_H
