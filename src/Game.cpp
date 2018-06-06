#include "Game.h"
#include <algorithm>
#include <iostream>
#include <string>
#include "Camera.h"
#include "Common.h"

Game* Game::_instance = nullptr;

Game::~Game() {
    IMG_Quit();
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    delete storedState;
}

Game* Game::GetInstance(const std::string& title, int w, int h) {
    if (!_instance) _instance = new Game(title, w, h);
    return _instance;
}

void Game::UpdateBeatTime(int timeRhythm) {
    // std::cout << tickCounter << " ; " << timeRhythm << std::endl;
    tickCounter = tickCounter * 0.9 + timeRhythm * 0.1;
}

void Game::CalculateDeltaTime() {
    int ticksTotal = static_cast<int>(SDL_GetTicks());
    dt = ticksTotal - frameStart;
    tickCounter += dt;
    dt /= 1000;
    frameStart = ticksTotal;

    if (input.KeyPress(SDL_SCANCODE_EQUALS)) {
        adjust += 10;
        std::cout << "Adjust = " << adjust << "ms\n";
    }
    if (input.KeyPress(SDL_SCANCODE_MINUS)) {
        adjust -= 10;
        std::cout << "Adjust = " << adjust << "ms\n";
    }

    int fixedTicks = tickCounter + adjust;
    int newHalfBeatCounter = fixedTicks / halfBeatTime;
    if (newHalfBeatCounter > halfBeatCounter) {
        halfBeatCounter = newHalfBeatCounter;
        shouldRhythmUpdate = true;
        offBeat = halfBeatCounter % 2;

    } else {
        shouldRhythmUpdate = false;
    }

    int a = fixedTicks - (halfBeatCounter / 2) * beatTime;
    int b = fixedTicks - ((halfBeatCounter / 2) + 1) * beatTime;

    int deltaRhythmMs = (a < -b) ? a : b;
    deltaRhythm = deltaRhythmMs / (beatTime / 2.0);
}

void Game::Run() {
    if (storedState) {
        stateStack.emplace(storedState);
        stateStack.top()->Start();
        storedState = nullptr;
    }

    while (!stateStack.empty()) {
        CalculateDeltaTime();
        input.Update(deltaRhythm);

        if (shouldRhythmUpdate) {
            shouldRhythmUpdate = false;
            if (!offBeat)
                stateStack.top()->RhythmUpdate();
            else
                stateStack.top()->RhythmReset();
            // std::cout << "." << offBeat << "." << std::endl;
        }

        stateStack.top()->Update(dt);
        stateStack.top()->Render();
        SDL_RenderPresent(renderer);

        if (stateStack.top()->PopRequested() ||
            stateStack.top()->QuitRequested()) {
            stateStack.pop();
            Resources::ClearAll();
            if (!stateStack.empty()) stateStack.top()->Resume();
        }

        if (storedState) {
            stateStack.emplace(storedState);
            stateStack.top()->Start();
            storedState = nullptr;
            tickCounter = 0;
        }
        SDL_Delay(10);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }
}

/* Private */
Game::Game(const std::string& title, int width, int height)
    : input(InputManager::GetInstance()) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
        std::cerr << "SDL_Init: " << SDL_GetError() << std::endl;
        exit(EXIT_SUCCESS);
    }

    int init_flags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (IMG_Init(init_flags) != init_flags) {
        std::cerr << "IMG_Init: " << SDL_GetError() << std::endl;
        exit(EXIT_SUCCESS);
    }

    if (Mix_OpenAudio(bitRate, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        std::cerr << "Mix_OpenAudio: " << Mix_GetError() << std::endl;
        exit(EXIT_SUCCESS);
    }
    init_flags = MIX_INIT_OGG;
    if (Mix_Init(init_flags) != init_flags) {
        std::cerr << "Mix_Init: " << Mix_GetError() << std::endl;
        exit(EXIT_SUCCESS);
    }

    int nchannels = 32;
    if (Mix_AllocateChannels(nchannels) != nchannels) {
        std::cerr << "Mix_AllocateChannels: " << Mix_GetError() << std::endl;
        exit(EXIT_SUCCESS);
    }

    if (TTF_Init()) {
        std::cerr << "TTF_Init: " << TTF_GetError() << std::endl;
        exit(EXIT_SUCCESS);
    }

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (!window) {
        std::cerr << "SDL_CreateWindow: " << SDL_GetError() << std::endl;
        exit(EXIT_SUCCESS);
    }
    Camera::screenSize.Set(width, height);

    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer: " << SDL_GetError() << std::endl;
        exit(EXIT_SUCCESS);
    }
}
