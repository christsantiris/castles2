#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "game.h"
#include "input.h"

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Music* music = Mix_LoadMUS("music/theme/medieval.ogg");
    if (!music) {
        SDL_Log("Failed to load music: %s", Mix_GetError());
    } else {
        Mix_PlayMusic(music, -1); // -1 means loop forever
    }

    bool musicOn = true;

    TTF_Font* font = TTF_OpenFont("fonts/MedievalSharp-Regular.ttf", 18);
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Castles III: Siege & Conquest",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1024, 768,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Game game;
    game.init();

    SDL_Event event;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            GameAction action = handleInput(event, game);
            if (action == QUIT) running = false;
            if (action == TOGGLE_MUSIC) {
                musicOn = !musicOn;
                if (musicOn) Mix_ResumeMusic();
                else Mix_PauseMusic();
            }
        }

        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
        SDL_RenderClear(renderer);

        game.update(); // increment date
        game.render(renderer, font, musicOn);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    TTF_Quit();
    Mix_FreeMusic(music);
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}