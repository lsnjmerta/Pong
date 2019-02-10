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

bool runGame(float fps, int mode, SDL_Event& e, const Uint8 *keystates, player& firstPlayer, player& secondPlayer, ball& ball, SDL_Texture *squareTex, SDL_Rect& b_rect, char buffer[], SDL_Color whiteColor, SDL_Texture *net){

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
            firstPlayer.pos.y -= firstPlayer.speed;
        if(keystates[SDL_SCANCODE_DOWN] | keystates[SDL_SCANCODE_S])
            firstPlayer.pos.y += firstPlayer.speed;

        // Basic AI
        if (ball.y < secondPlayer.pos.y + secondPlayer.pos.h / 2) {
            secondPlayer.pos.y -= secondPlayer.speed;
        }
        if (ball.y > secondPlayer.pos.y + secondPlayer.pos.h / 2) {
            secondPlayer.pos.y += secondPlayer.speed;
        }
    } else {
        // 2 players movement setup

        if(keystates[SDL_SCANCODE_UP])
            secondPlayer.pos.y -= firstPlayer.speed;
        if(keystates[SDL_SCANCODE_DOWN])
            secondPlayer.pos.y += firstPlayer.speed;

        if (keystates[SDL_SCANCODE_W]){
            firstPlayer.pos.y -= firstPlayer.speed;
        }
        if (keystates[SDL_SCANCODE_S]) {
            firstPlayer.pos.y += firstPlayer.speed;
        }

    }

    if(ball.vx > BALL_MAXSPEED)
        ball.vx = BALL_MAXSPEED;

    if(ball.vy > BALL_MAXSPEED)
        ball.vy = BALL_MAXSPEED;

    // Update ball position
    ball.x += ball.vx;
    ball.y += ball.vy;

    // Boundary Collision
    if(ball.y < 0) {
        ball.y = 0;
        ball.vy *= -1;
    }
    if(ball.y + BALL_HEIGHT >= HEIGHT) {
        ball.y = HEIGHT - BALL_HEIGHT - 1;
        ball.vy *= -1;
    }

    if(ball.x < 0) {

        secondPlayer.score += 1;
        ball.x = firstPlayer.pos.x + firstPlayer.pos.w;
        ball.y = firstPlayer.pos.y + firstPlayer.pos.h/2;
        ball.vx = BALL_INIT_SPEED;
        ball.speed = BALL_INIT_SPEED;
    }
    if(ball.x + BALL_WIDTH>= WIDTH) {
        firstPlayer.score += 1;
        ball.x = secondPlayer.pos.x - BALL_WIDTH;
        ball.y = secondPlayer.pos.y + secondPlayer.pos.h/2;
        ball.vx = -1 * BALL_INIT_SPEED;
        ball.speed = BALL_INIT_SPEED;
    }

    if(firstPlayer.pos.y < 0) firstPlayer.pos.y = 0;
    if(firstPlayer.pos.y + firstPlayer.pos.h > HEIGHT) firstPlayer.pos.y = HEIGHT - firstPlayer.pos.h;
    if(secondPlayer.pos.y < 0) secondPlayer.pos.y = 0;
    if(secondPlayer.pos.y + secondPlayer.pos.h > HEIGHT) secondPlayer.pos.y = HEIGHT - secondPlayer.pos.h;

    // Update the ball rect structure
    b_rect.x = (int) ball.x;
    b_rect.y = (int) ball.y;

    // Collisions
    if(SDL_HasIntersection(&firstPlayer.pos, &b_rect)) {


        ball.x = firstPlayer.pos.x + firstPlayer.pos.w;

        ball.speed = ball.speed * BALL_ACCELERATE;

        float angle = calc_angle(firstPlayer.pos.y, ball.y, firstPlayer.pos.h);
        ball.vx = ball.speed * cos(angle);
        ball.vy = ((ball.vy>0)? -1 : 1) * ball.speed * sin(angle);
    }

    if(SDL_HasIntersection(&secondPlayer.pos, &b_rect)) {

        ball.x = secondPlayer.pos.x - BALL_WIDTH;

        ball.speed = ball.speed * BALL_ACCELERATE;

        float angle = calc_angle(secondPlayer.pos.y, ball.y, secondPlayer.pos.h);
        ball.vx = -1 * ball.speed * cos(angle);
        ball.vy = ((ball.vy > 0) ? -1 : 1) * ball.speed * sin(angle);
    }

    SDL_RenderClear(ren);

    SDL_RenderCopy(ren, squareTex, NULL, &firstPlayer.pos);
    SDL_RenderCopy(ren, squareTex, NULL, &secondPlayer.pos);

    // Render net
    renderTexture(net, ren, WIDTH/2 - CENTER/2, 0, CENTER, HEIGHT);

    // Render  Ball
    renderTexture(net, ren, ball.x, ball.y, BALL_WIDTH, BALL_HEIGHT);

    // Display the score
    sprintf(buffer, "%d", firstPlayer.score);
    SDL_Texture *p1score = renderText(buffer, "fonts/Raleway-ExtraBold.ttf", whiteColor, 40, ren);
    sprintf(buffer, "%d", secondPlayer.score);
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