#pragma once
#define INCLUDE_SDL 
#define INCLUDE_SDL_IMAGE 
#define INCLUDE_SDL_MIXER
#include "SDL_include.h"
#include "State.h"

class Game
{
    public:
        ~Game(void);
        void run(void);
        SDL_Renderer* getRenderer(void);
        State* getState(void);
        static Game* getInstance(void);
        static Game* getInstance(std::string title, int w, int h);

    private:
        static Game *_instance;
        SDL_Window *window;
        SDL_Renderer *renderer;
        State *state;

        Game(std::string title, int width, int height);
};

