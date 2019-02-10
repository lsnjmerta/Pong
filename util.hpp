//
// Created by Jakub Merta on 2019-02-10.
//

#ifndef GRA_UTIL_H
#define GRA_UTIL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>

using namespace std;

void logError(const string &msg, ostream &os = cerr) {
    os << msg << " error: " << SDL_GetError() << endl;
    exit(-1);
}

float calc_angle(float y1, float y2, int height) {
    float rely = y1 + height/2 - y2;
    rely /= height/2.0;   // Normalise
    return rely * MAX_ANGLE;
}

SDL_Texture *renderText(const std::string &msg, const std::string &fontPath, SDL_Color color, int fontSize, SDL_Renderer *ren) {
    TTF_Font *font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if(font == nullptr) {
        logError("Unable to open font");
        return nullptr;
    }

    SDL_Surface *surface = TTF_RenderText_Blended(font, msg.c_str(), color);
    if(surface == nullptr) {
        TTF_CloseFont(font);
        logError("Unable to render text to a surface");
        return nullptr;
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surface);
    if(tex == nullptr) {
        logError("Unable to render surface to texture");
    }

    SDL_FreeSurface(surface);
    TTF_CloseFont(font);

    return tex;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w=-1, int h=-1) {
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;

    // jak nie jest określona wysokość i szerokość korzysta z tych podanych w teksturze
    if(w == -1 || h == -1)
        SDL_QueryTexture(tex, nullptr, nullptr, &dest.w, &dest.h);

    SDL_RenderCopy(ren, tex, nullptr, &dest);
}



#endif //GRA_UTIL_H
