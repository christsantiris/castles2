#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ui.h"
#include "map.h"

enum GameScreen { LANDING, DYNASTY_SELECT, PLAYING };
enum GameAction { NONE, QUIT, TOGGLE_MUSIC };

class Game {
public:
    int activeTab = 0;
    int score = 1000;
    std::string date = "May 2, 1312";

    Map map;

    void init();
    GameAction handleEvent(SDL_Event& event);
    void update();
    void render(SDL_Renderer* renderer, TTF_Font* font, bool musicOn);
};