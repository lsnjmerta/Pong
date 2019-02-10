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

    InitSDL(&ren, &win);

    int board_width;
    int board_height;
    SDL_Texture *squareTex = IMG_LoadTexture(ren, "images/pong_board.png");
    SDL_QueryTexture(squareTex, NULL, NULL, &board_width, &board_height);

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
    b.vx = (random() % 2 == 0)? BALL_INIT_SPEED : -1 * BALL_INIT_SPEED;
    b.vy =  -0.5f;

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
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    int mode = 1;

    while(!running) {

        // FPS Calculation
        ++frames;
        uint currTime = SDL_GetTicks();
        float elapsed = (currTime - prevTime);

        if(elapsed > 100) {
            fps = round(frames / (elapsed / 1000.0));
            frames = 0;
            prevTime = currTime;
        }

        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT)  running = true;
            if(e.type == SDL_KEYDOWN) {
                switch(e.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE:
                        running = true;
                        break;
                }
            }
        }

        // Player Movement
        if(keystates[SDL_SCANCODE_UP])
            p1.pos.y -= p1.speed;
        if(keystates[SDL_SCANCODE_DOWN])
            p1.pos.y += p1.speed;

        if ( mode == 1) {
            // Basic AI
            if (b.y < p2.pos.y + p2.pos.h / 2) {
                p2.pos.y -= p2.speed;
            }
            if (b.y > p2.pos.y + p2.pos.h / 2) {
                p2.pos.y += p2.speed;
            }
        }

        if(b.vx > BALL_MAXSPEED)
            b.vx = BALL_MAXSPEED;

        if(b.vy > BALL_MAXSPEED)
            b.vy = BALL_MAXSPEED;

        // Update ball position
        b.x += b.vx;
        b.y += b.vy;

        // Boundary Collision
        if(b.y < 0) {
            b.y = 0;
            b.vy *= -1;
        }
        if(b.y + BALL_HEIGHT >= HEIGHT) {
            b.y = HEIGHT - BALL_HEIGHT - 1;
            b.vy *= -1;
        }

        if(b.x < 0) {

            p2.score += 1;
            b.x = p1.pos.x + p1.pos.w;
            b.y = p1.pos.y + p1.pos.h/2;
            b.vx = BALL_INIT_SPEED;
            b.speed = BALL_INIT_SPEED;
        }
        if(b.x + BALL_WIDTH>= WIDTH) {


            p1.score += 1;
            b.x = p2.pos.x - BALL_WIDTH;
            b.y = p2.pos.y + p2.pos.h/2;
            b.vx = -1 * BALL_INIT_SPEED;
            b.speed = BALL_INIT_SPEED;
        }

        if(p1.pos.y < 0) p1.pos.y = 0;
        if(p1.pos.y + p1.pos.h > HEIGHT) p1.pos.y = HEIGHT - p1.pos.h;
        if(p2.pos.y < 0) p2.pos.y = 0;
        if(p2.pos.y + p2.pos.h > HEIGHT) p2.pos.y = HEIGHT - p2.pos.h;

        // Update the rect structure
        b_rect.x = (int) b.x;
        b_rect.y = (int) b.y;

        // Player Collision
        if(SDL_HasIntersection(&p1.pos, &b_rect)) {


            b.x = p1.pos.x + p1.pos.w;

            b.speed = b.speed * BALL_ACCELERATE;

            float angle = calc_angle(p1.pos.y, b.y, p1.pos.h);
            b.vx = b.speed * cos(angle);
            b.vy = ((b.vy>0)? -1 : 1) * b.speed * sin(angle);
        }

        if(mode == 1 && SDL_HasIntersection(&p2.pos, &b_rect)) {

            b.x = p2.pos.x - BALL_WIDTH;

            b.speed = b.speed * BALL_ACCELERATE;

            float angle = calc_angle(p2.pos.y, b.y, p2.pos.h);
            b.vx = -1 * b.speed * cos(angle);
            b.vy = ((b.vy>0)? -1 : 1) * b.speed * sin(angle);
        }

        SDL_RenderClear(ren);

        SDL_RenderCopy(ren, squareTex, NULL, &p1.pos);
        SDL_RenderCopy(ren, squareTex, NULL, &p2.pos);

        // Render center line
        renderTexture(squareTex, ren, WIDTH/2 - CENTER/2, 0, CENTER, HEIGHT);

        // Render  Ball
        renderTexture(squareTex, ren, b.x, b.y, BALL_WIDTH, BALL_HEIGHT);

        // Display the score
        sprintf(buffer, "%d", p1.score);
        SDL_Texture *p1score = renderText(buffer, "fonts/Raleway-ExtraBold.ttf", whiteColor, 40, ren);
        sprintf(buffer, "%d", p2.score);
        SDL_Texture *p2score = renderText(buffer, "fonts/Raleway-ExtraBold.ttf", whiteColor, 40, ren);

        int width;
        SDL_QueryTexture(p1score, NULL, NULL, &width, NULL);

        renderTexture(p1score, ren, WIDTH/2 - width - 10, 10);
        renderTexture(p2score, ren, WIDTH/2 + 10, 10);

        SDL_DestroyTexture(p1score);
        SDL_DestroyTexture(p2score);

        // Print fps
        sprintf(buffer, "%.0f", fps);
        SDL_Texture *fpsCounter = renderText(buffer, "fonts/Raleway-ExtraBold.ttf", whiteColor, 20, ren);
        renderTexture(fpsCounter, ren, WIDTH - 20, 0);
        SDL_DestroyTexture(fpsCounter);

        SDL_RenderPresent(ren);
    }

    SDL_DestroyTexture(squareTex);
    ClearAll(&ren, &win);
    return 0;
}

void InitSDL(SDL_Renderer **ren, SDL_Window **win) {
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
        logError("Failed to init SDL");

    *win = SDL_CreateWindow(
            "Pong",
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            WIDTH, HEIGHT,
            SDL_WINDOW_SHOWN
    );
    if(*win == nullptr)
        logError("Failed to create SDL Window");

    *ren = SDL_CreateRenderer(*win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(*ren == nullptr)
        logError("Failed to create SDL Renderer");

    const int flags = IMG_INIT_PNG | IMG_INIT_JPG;
    if(IMG_Init(flags) !=flags)
        logError("Failed to load the Image loading extensions");

    if(TTF_Init() != 0)
        logError("Failed to load TTF extension");
}

void ClearAll(SDL_Renderer **ren, SDL_Window **win) {
    SDL_DestroyRenderer(*ren);
    SDL_DestroyWindow(*win);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}