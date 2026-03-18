#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ui.h"
#include "map.h"
#include "collection.h"
#include "combat.h"

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
    int activeTab = -1;
    int score     = 0;
    int resources[4] = {8, 9, 10, 8}; // Food, Timber, Iron, Gold starting values
    int totalWorkers     = 4;
    int availableWorkers = 4;
    int pendingWorkers[4] = {1, 1, 1, 1}; // workers selected per resource before starting

    // combat related vars
    CombatTask combat;
    int playerStrength = 10;

    GameDate date = {2, 5, 1312};

    Uint32 lastTickTime = 0;
    Uint32 tickIntervalMs = 5000; // 1 real second = 1 game day

    Map map;

    CollectionTask task;

    void init();
    GameAction handleEvent(SDL_Event& event);

    int countOwnedProvinces(ResourceType res) const;

    void update();
    void render(SDL_Renderer* renderer, TTF_Font* font, bool musicOn);
    // admin tasks
    void startTask(ResourceType res, int workers);
    void cancelTask();
    // combat
    void startCombat(int provinceId);
    void resolveCombat();
    void cancelCombat();
};