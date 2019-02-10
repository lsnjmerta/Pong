//
// Created by Jakub Merta on 2019-02-10.
//

#ifndef GRA_SETUP_FUNCTIONS_H
#define GRA_SETUP_FUNCTIONS_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "pong.hpp"
#include "util.hpp"

using namespace std;

int getChoosenOption() {
    int mode = 1;
    int x, y;
    int menuWidth;
    int menuHeight;
    bool active = true;

    SDL_Texture *menu = IMG_LoadTexture(ren, "images/menu.png");


    while (active) {
        SDL_QueryTexture(menu, NULL, NULL, &menuWidth, &menuHeight);
        renderTexture(menu, ren, 0, 0, menuWidth, menuHeight);
        SDL_RenderPresent(ren);

        while (SDL_PollEvent(&mouse_event)) {
            if (mouse_event.type == SDL_MOUSEBUTTONDOWN) {
                x = mouse_event.motion.x;
                y = mouse_event.motion.y;

                cout << "click caught";

                if ((x > 195) && (x < 410) && (y < 196) && (y > 93)) {
                    mode = 1;
                    active = false;
                    cout << "selected solo";
                }

                if ((x > 195) && (x < 410) && (y < 312) && (y > 205)) {
                    mode = 2;
                    active = false;
                    cout << "selected 2 players";
                }

                if ((x > 195) && (x < 410) && (y < 430) && (y > 336)) {
                    mode = 3;
                    active = false;
                    cout << "quit";
                }
            }

            switch (mouse_event.type) {
                case SDL_KEYDOWN:
                    if (mouse_event.key.keysym.sym == SDLK_ESCAPE) {
                        mode = 3;
                        active = false;
                    }
                    break;
                case SDL_QUIT: {
                    mode = 3;
                    active = false;
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }
    SDL_DestroyTexture(menu);

    return mode;
}


#endif //GRA_SETUP_FUNCTIONS_H
