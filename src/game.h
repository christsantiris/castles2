#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ui.h"
#include "map.h"

enum GameScreen { LANDING, DYNASTY_SELECT, PLAYING };
enum GameAction { NONE, QUIT, TOGGLE_MUSIC };

struct GameDate {
    int day;
    int month;
    int year;

    static const char* monthName(int m) {
        const char* names[] = {
            "Jan","Feb","Mar","Apr","May","Jun",
            "Jul","Aug","Sep","Oct","Nov","Dec"
        };
        return names[m - 1];
    }

    int daysInMonth() const {
        int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
        return days[month - 1];
    }

    void advance() {
        day++;
        if (day > daysInMonth()) {
            day = 1;
            month++;
            if (month > 12) {
                month = 1;
                year++;
            }
        }
    }

    std::string toString() const {
        return std::string(monthName(month)) + " " + std::to_string(day) + ", " + std::to_string(year);
    }
};

class Game {
public:
    int activeTab = 0;
    int score = 1000;
    GameDate date = {2, 5, 1312};

    Uint32 lastTickTime = 0;
    Uint32 tickIntervalMs = 5000; // 1 real second = 1 game day

    Map map;

    void init();
    GameAction handleEvent(SDL_Event& event);
    void update();
    void render(SDL_Renderer* renderer, TTF_Font* font, bool musicOn);
};