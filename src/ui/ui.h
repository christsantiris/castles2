#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "../map.h"
#include "../game.h"
#include "../combat.h"

class Game; // forward declare

// Colors
extern SDL_Color green;
extern SDL_Color red;
extern SDL_Color blue;
extern SDL_Color yellow;
extern SDL_Color white;

enum IconType  { TASK, UNIT, RESOURCE };
enum IconShape { CASTLE, SWORD, SHIELD, HELMET, SCROLL, ARROW, CIRCLE, TRIANGLE, SQUARE, DIAMOND, FOOD, TIMBER, IRON, GOLD };

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

void drawShape(SDL_Renderer* renderer, IconShape shape, int x, int y); // Used for creating polygons
void renderUI(SDL_Renderer* renderer, TTF_Font* font, int activeTab, const char* dateStr, Game& game); // Render main game. 
void renderLanding(SDL_Renderer* renderer, TTF_Font* font); // Render landing page. 
void renderDynastySelect(SDL_Renderer* renderer, TTF_Font* font); // Render Character select page.
void renderProvinceInfo(SDL_Renderer* renderer, TTF_Font* font, const Province& province, Game& game); //Render province info
void renderOptsPanel(SDL_Renderer* renderer, TTF_Font* font, bool musicOn); // Render options pannel
void renderStockTab(SDL_Renderer* renderer, TTF_Font* font, Game& game); // Render stock tab
void renderTopBar(SDL_Renderer* renderer, TTF_Font* font, Game& game); // Render top icon
void renderTaskRows(SDL_Renderer* renderer, TTF_Font* font, Game& game); // Render task progress
void renderTabBar(SDL_Renderer* renderer, TTF_Font* font, int activeTab); // Render tab menu options
void renderInfoPanel(SDL_Renderer* renderer, TTF_Font* font, const char* dateStr, Game& game);
void renderCombatRow(SDL_Renderer* renderer, TTF_Font* font, Game& game); // Render combat progress
void renderVictory(SDL_Renderer* renderer, TTF_Font* font, const std::string& dynasty, int score, const std::string& date); // Render victory screen
