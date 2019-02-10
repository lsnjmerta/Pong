//
// Created by Jakub Merta on 2019-02-10.
//

#ifndef GRA_GAME_H
#define GRA_GAME_H

#include "pong.hpp"
#include "util.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

bool runGame(float fps, int mode, SDL_Event& e, const Uint8 *keystates, player& p1, player& p2, ball& b, SDL_Texture *squareTex, SDL_Rect& b_rect, char buffer[], SDL_Color whiteColor, SDL_Texture *net){

    while(SDL_PollEvent(&e)) {
        if(e.type == SDL_QUIT)  return false;
        if(e.type == SDL_KEYDOWN) {
            switch(e.key.keysym.scancode) {
                case SDL_SCANCODE_ESCAPE:
                    return false;
            }
        }
    }

    if (mode == 1){
        // Player Movement
        if(keystates[SDL_SCANCODE_UP] | keystates[SDL_SCANCODE_W])
            p1.pos.y -= p1.speed;
        if(keystates[SDL_SCANCODE_DOWN] | keystates[SDL_SCANCODE_S])
            p1.pos.y += p1.speed;

        // Basic AI
        if (b.y < p2.pos.y + p2.pos.h / 2) {
            p2.pos.y -= p2.speed;
        }
        if (b.y > p2.pos.y + p2.pos.h / 2) {
            p2.pos.y += p2.speed;
        }
    } else {
        // 2 players movement setup

        if(keystates[SDL_SCANCODE_UP])
            p2.pos.y -= p1.speed;
        if(keystates[SDL_SCANCODE_DOWN])
            p2.pos.y += p1.speed;

        if (keystates[SDL_SCANCODE_W]){
            p1.pos.y -= p1.speed;
        }
        if (keystates[SDL_SCANCODE_S]) {
            p1.pos.y += p1.speed;
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

    // Update the ball rect structure
    b_rect.x = (int) b.x;
    b_rect.y = (int) b.y;

    // Collisions
    if(SDL_HasIntersection(&p1.pos, &b_rect)) {


        b.x = p1.pos.x + p1.pos.w;

        b.speed = b.speed * BALL_ACCELERATE;

        float angle = calc_angle(p1.pos.y, b.y, p1.pos.h);
        b.vx = b.speed * cos(angle);
        b.vy = ((b.vy>0)? -1 : 1) * b.speed * sin(angle);
    }

    if(SDL_HasIntersection(&p2.pos, &b_rect)) {

        b.x = p2.pos.x - BALL_WIDTH;

        b.speed = b.speed * BALL_ACCELERATE;

        float angle = calc_angle(p2.pos.y, b.y, p2.pos.h);
        b.vx = -1 * b.speed * cos(angle);
        b.vy = ((b.vy > 0) ? -1 : 1) * b.speed * sin(angle);
    }

    SDL_RenderClear(ren);

    SDL_RenderCopy(ren, squareTex, NULL, &p1.pos);
    SDL_RenderCopy(ren, squareTex, NULL, &p2.pos);

    // Render net
    renderTexture(net, ren, WIDTH/2 - CENTER/2, 0, CENTER, HEIGHT);

    // Render  Ball
    renderTexture(net, ren, b.x, b.y, BALL_WIDTH, BALL_HEIGHT);

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

    return true;
}

#endif //GRA_GAME_H