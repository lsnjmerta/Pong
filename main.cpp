#include <iostream>
#include <SDL2/SDL.h>
using namespace std;


const int WIDTH = 640;
const int HEIGHT = 480;

void logError(const string &msg) {
    cerr << msg << SDL_GetError() << endl;
    exit(-1);
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
