#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

// Colors
extern SDL_Color green;
extern SDL_Color red;
extern SDL_Color blue;
extern SDL_Color yellow;
extern SDL_Color white;

enum IconType  { TASK, UNIT, RESOURCE };
enum IconShape { CASTLE, SWORD, SHIELD, HELMET, SCROLL, ARROW, CIRCLE, TRIANGLE, SQUARE, DIAMOND };

struct TopBarIcon {
    SDL_Color bgColor;
    int used;
    int total;
    IconType type;
    IconShape shape;
};

struct TaskSlot {
    std::string label;
    bool unlocked;
    int type;
    int val1, val2, val3;
    float progress;
};

extern TopBarIcon topBarIcons[];
extern TaskSlot taskSlots[];

void drawShape(SDL_Renderer* renderer, IconShape shape, int x, int y);
void renderUI(SDL_Renderer* renderer, TTF_Font* font, int activeTab);