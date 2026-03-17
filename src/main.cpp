#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

SDL_Color green = {0, 200, 0, 255};
SDL_Color red   = {220, 0, 0, 255};
SDL_Color blue  = {0, 0, 220, 255};
SDL_Color yellow = {255, 215, 0, 255};
SDL_Color white = {255, 255, 255, 255};

enum IconType { TASK, UNIT, RESOURCE };
enum IconShape { CASTLE, SWORD, SHIELD, HELMET, SCROLL, ARROW, CIRCLE, TRIANGLE, SQUARE, DIAMOND };

struct TopBarIcon {
    SDL_Color bgColor;
    int used;
    int total;
    IconType type;
    IconShape shape;
};

TopBarIcon topBarIcons[] = {
    {{0, 120, 0, 255},   4,  4, TASK,     CASTLE},   // stock 1
    {{0, 120, 0, 255},   4,  4, TASK,     CASTLE},   // stock 2
    {{150, 0, 0, 255},   4,  4, TASK,     SWORD},    // army 1
    {{150, 0, 0, 255},   4,  4, TASK,     SWORD},    // army 2
    {{0, 0, 150, 255},   4,  4, TASK,     SCROLL},   // relations 1
    {{0, 0, 150, 255},   4,  4, TASK,     SCROLL},   // relations 2
    {{150, 0, 0, 255},   0,  4, UNIT,     SWORD},    // infantry
    {{150, 0, 0, 255},   0,  4, UNIT,     ARROW},    // archers
    {{150, 0, 0, 255},   0,  4, UNIT,     SHIELD},   // knights
    {{0, 120, 0, 255},   4,  4, RESOURCE, CIRCLE},   // food
    {{0, 120, 0, 255},   4,  6, RESOURCE, TRIANGLE}, // timber
    {{0, 120, 0, 255},   6,  6, RESOURCE, DIAMOND},  // iron
    {{0, 120, 0, 255},   6,  6, RESOURCE, CIRCLE},   // gold
};

struct TaskSlot {
    std::string label;
    bool unlocked;
    int type;
    int val1, val2, val3;
    float progress; // 0.0 to 1.0
};

TaskSlot taskSlots[] = {
    {"Mine Iron",  true,  0, 4, 3, 3, 0.6f},
    {"",           false, 0, 0, 0, 0, 0.0f},
    {"Archers",    true,  1, 0, 5, 0, 0.3f},
    {"",           false, 1, 0, 0, 0, 0.0f},
    {"Merchant",   true,  2, 0, 0, 3, 0.5f},
    {"",           false, 2, 0, 0, 0, 0.0f}
};

// used for drawing placeholder icons. Can remove once real icons are implemented
void drawShape(SDL_Renderer* renderer, IconShape shape, int x, int y) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    switch (shape) {
        case CASTLE:
            SDL_RenderDrawRect(renderer, new SDL_Rect{x, y+5, 20, 20});
            SDL_RenderDrawRect(renderer, new SDL_Rect{x+3, y, 5, 7});
            SDL_RenderDrawRect(renderer, new SDL_Rect{x+12, y, 5, 7});
            break;
        case SWORD:
            SDL_RenderDrawLine(renderer, x+10, y, x+10, y+20);
            SDL_RenderDrawLine(renderer, x+5, y+8, x+15, y+8);
            break;
        case SHIELD:
            SDL_RenderDrawRect(renderer, new SDL_Rect{x+3, y, 14, 18});
            break;
        case HELMET:
            SDL_RenderDrawLine(renderer, x+3, y+10, x+17, y+10);
            SDL_RenderDrawLine(renderer, x+3, y+10, x+5, y+20);
            SDL_RenderDrawLine(renderer, x+17, y+10, x+15, y+20);
            break;
        case SCROLL:
            SDL_RenderDrawRect(renderer, new SDL_Rect{x+3, y+3, 14, 18});
            SDL_RenderDrawLine(renderer, x+3, y+3, x+3, y+21);
            break;
        case ARROW:
            SDL_RenderDrawLine(renderer, x+10, y, x+10, y+20);
            SDL_RenderDrawLine(renderer, x+10, y, x+5, y+8);
            SDL_RenderDrawLine(renderer, x+10, y, x+15, y+8);
            break;
        case CIRCLE:
            SDL_RenderDrawRect(renderer, new SDL_Rect{x+3, y+3, 14, 14});
            break;
        case TRIANGLE:
            SDL_RenderDrawLine(renderer, x+10, y, x+3, y+20);
            SDL_RenderDrawLine(renderer, x+10, y, x+17, y+20);
            SDL_RenderDrawLine(renderer, x+3, y+20, x+17, y+20);
            break;
        case SQUARE:
            SDL_RenderDrawRect(renderer, new SDL_Rect{x+3, y+3, 14, 14});
            break;
        case DIAMOND:
            SDL_RenderDrawLine(renderer, x+10, y, x+17, y+10);
            SDL_RenderDrawLine(renderer, x+17, y+10, x+10, y+20);
            SDL_RenderDrawLine(renderer, x+10, y+20, x+3, y+10);
            SDL_RenderDrawLine(renderer, x+3, y+10, x+10, y);
            break;
    }
}
int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    // Load font style
    TTF_Font* font = TTF_OpenFont("/System/Library/Fonts/NewYork.ttf", 18);
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Castles II: Siege & Conquest",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1024, 768,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event event;
    bool running = true;
    int activeTab = 0; // 0=STOCK, 1=ARMY, 2=RELAT, 3=OPTS

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                // SDL_Log("Active tab: %d", activeTab);
                int x = event.button.x;
                int y = event.button.y;

                if (y >= 424 && y <= 464) {
                    if (x >= 740 && x < 811) activeTab = 0;
                    else if (x >= 811 && x < 882) activeTab = 1;
                    else if (x >= 882 && x < 953) activeTab = 2;
                    else if (x >= 953 && x < 1024) activeTab = 3;
                }
            }
        }

        // Clear screen before rendering ui
        SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
        SDL_RenderClear(renderer);

        // Top bar - dark gray
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        SDL_Rect topBar = {5, 5, 1014, 70};
        SDL_RenderFillRect(renderer, &topBar);

        // Main map area - dark green
        SDL_SetRenderDrawColor(renderer, 34, 85, 34, 255);
        SDL_Rect mapArea = {6, 86, 728, 676};
        SDL_RenderFillRect(renderer, &mapArea);

        // Map border - gold color
        SDL_SetRenderDrawColor(renderer, 200, 170, 50, 255);
        SDL_Rect mapBorder = {5, 85, 730, 678};
        SDL_RenderDrawRect(renderer, &mapBorder);

        // add rectangles for task labels
        // STOCK - green
        SDL_SetRenderDrawColor(renderer, 0, activeTab == 0 ? 200 : 120, 0, 255);
        SDL_Rect stockTab = {740, 424, 71, 40};
        SDL_RenderFillRect(renderer, &stockTab);

        // ARMY - red
        SDL_SetRenderDrawColor(renderer, activeTab == 1 ? 220 : 150, 0, 0, 255);
        SDL_Rect armyTab =  {811, 424, 71, 40};
        SDL_RenderFillRect(renderer, &armyTab);

        // RELAT - blue
        SDL_SetRenderDrawColor(renderer, 0, 0, activeTab == 2 ? 220 : 150, 255);
        SDL_Rect relatTab = {882, 424, 71, 40};
        SDL_RenderFillRect(renderer, &relatTab);

        // OPTS - brown
        SDL_SetRenderDrawColor(renderer, activeTab == 3 ? 160 : 101, activeTab == 3 ? 100 : 67, activeTab == 3 ? 50 : 33, 255);
        SDL_Rect optsTab =  {953, 424, 71, 40};
        SDL_RenderFillRect(renderer, &optsTab);

        // Add labels for tasks
        // stock task
        SDL_Surface* surface = TTF_RenderText_Solid(font, "STOCK", yellow);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect textRect =  {750, 430, surface->w,  surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        // army task
        SDL_Surface* surface2 = TTF_RenderText_Solid(font, "ARMY", yellow);
        SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
        SDL_Rect textRect2 = {820, 430, surface2->w, surface2->h};
        SDL_RenderCopy(renderer, texture2, NULL, &textRect2);
        SDL_FreeSurface(surface2);
        SDL_DestroyTexture(texture2);

        //relations task
        SDL_Surface* surface3 = TTF_RenderText_Solid(font, "RELAT", white);
        SDL_Texture* texture3 = SDL_CreateTextureFromSurface(renderer, surface3);
        SDL_Rect textRect3 = {890, 430, surface3->w, surface3->h};
        SDL_RenderCopy(renderer, texture3, NULL, &textRect3);
        SDL_FreeSurface(surface3);
        SDL_DestroyTexture(texture3);

        // game options
        SDL_Surface* surface4 = TTF_RenderText_Solid(font, "OPTS", yellow);
        SDL_Texture* texture4 = SDL_CreateTextureFromSurface(renderer, surface4);
        SDL_Rect textRect4 = {960, 430, surface4->w, surface4->h};
        SDL_RenderCopy(renderer, texture4, NULL, &textRect4);
        SDL_FreeSurface(surface4);
        SDL_DestroyTexture(texture4);

        // First task bar - dark wood background
        SDL_SetRenderDrawColor(renderer, 80, 45, 10, 255);
        SDL_Rect taskBar1 = {745, 90, 274, 35};
        SDL_RenderFillRect(renderer, &taskBar1);

        // Game info panel - dark brown
        SDL_SetRenderDrawColor(renderer, 60, 30, 10, 255);
        SDL_Rect infoPanel = {745, 464, 274, 299};
        SDL_RenderFillRect(renderer, &infoPanel);

        // statistics pannel
        //date
        SDL_Surface* dateSurface = TTF_RenderText_Solid(font, "May 2, 1312", yellow);
        SDL_Texture* dateTexture = SDL_CreateTextureFromSurface(renderer, dateSurface);
        SDL_Rect dateRect = {750, 600, dateSurface->w, dateSurface->h};
        SDL_RenderCopy(renderer, dateTexture, NULL, &dateRect);
        SDL_FreeSurface(dateSurface);
        SDL_DestroyTexture(dateTexture);

        // score
        SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, "Score: 1000", yellow);
        SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
        SDL_Rect scoreRect = {750, 640, scoreSurface->w, scoreSurface->h};
        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
        SDL_FreeSurface(scoreSurface);
        SDL_DestroyTexture(scoreTexture);

        // title
        SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "Castles II", yellow);
        SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
        SDL_Rect titleRect = {750, 500, titleSurface->w, titleSurface->h};
        SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);
        SDL_FreeSurface(titleSurface);
        SDL_DestroyTexture(titleTexture);

        // dynamic task progress bars
        for (int i = 0; i < 6; i++) {
            if (taskSlots[i].unlocked) {
                SDL_SetRenderDrawColor(renderer, 80, 45, 10, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
            }
            SDL_Rect taskRow = {745, 90 + (i * 55), 274, 45};
            SDL_RenderFillRect(renderer, &taskRow);
        }

        // task slot loop
        for (int i = 0; i < 6; i++) {
            if (taskSlots[i].unlocked) {
                SDL_Surface* v1 = TTF_RenderText_Solid(font, std::to_string(taskSlots[i].val1).c_str(), green);
                SDL_Texture* t1 = SDL_CreateTextureFromSurface(renderer, v1);
                SDL_Rect r1 = {747, 97 + (i * 55), v1->w, v1->h};
                SDL_RenderCopy(renderer, t1, NULL, &r1);
                SDL_FreeSurface(v1);
                SDL_DestroyTexture(t1);

                SDL_Surface* v2 = TTF_RenderText_Solid(font, std::to_string(taskSlots[i].val2).c_str(), red);
                SDL_Texture* t2 = SDL_CreateTextureFromSurface(renderer, v2);
                SDL_Rect r2 = {770, 97 + (i * 55), v2->w, v2->h};
                SDL_RenderCopy(renderer, t2, NULL, &r2);
                SDL_FreeSurface(v2);
                SDL_DestroyTexture(t2);

                SDL_Surface* v3 = TTF_RenderText_Solid(font, std::to_string(taskSlots[i].val3).c_str(), blue);
                SDL_Texture* t3 = SDL_CreateTextureFromSurface(renderer, v3);
                SDL_Rect r3 = {793, 97 + (i * 55), v3->w, v3->h};
                SDL_RenderCopy(renderer, t3, NULL, &r3);
                SDL_FreeSurface(v3);
                SDL_DestroyTexture(t3);

                // Progress bar background
                SDL_SetRenderDrawColor(renderer, 30, 15, 5, 255);
                SDL_Rect progressBg = {745, 118 + (i * 55), 274, 8};
                SDL_RenderFillRect(renderer, &progressBg);

                // Progress bar fill
                SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
                SDL_Rect progressFill = {745, 118 + (i * 55), (int)(274 * taskSlots[i].progress), 8};
                SDL_RenderFillRect(renderer, &progressFill);

                SDL_Surface* labelSurface = TTF_RenderText_Solid(font, taskSlots[i].label.c_str(), yellow);
                SDL_Texture* labelTexture = SDL_CreateTextureFromSurface(renderer, labelSurface);
                SDL_Rect labelRect = {820, 97 + (i * 55), labelSurface->w, labelSurface->h};
                SDL_RenderCopy(renderer, labelTexture, NULL, &labelRect);
                SDL_FreeSurface(labelSurface);
                SDL_DestroyTexture(labelTexture);
            }
        }
        
        // draw top nav boxes
        for (int i = 0; i < 13; i++) {
            if (topBarIcons[i].type == TASK) {
                // Icon portion - colored background
                SDL_SetRenderDrawColor(renderer, 
                    topBarIcons[i].bgColor.r, 
                    topBarIcons[i].bgColor.g, 
                    topBarIcons[i].bgColor.b, 255);
                SDL_Rect iconBg = {5 + (i * 60), 5, 35, 65};
                SDL_RenderFillRect(renderer, &iconBg);

                // Number portion - darker background
                SDL_SetRenderDrawColor(renderer, 
                    topBarIcons[i].bgColor.r * 0.6, 
                    topBarIcons[i].bgColor.g * 0.6, 
                    topBarIcons[i].bgColor.b * 0.6, 255);
                SDL_Rect numberBg = {40 + (i * 60), 5, 20, 65};
                SDL_RenderFillRect(renderer, &numberBg);

                // Draw shape in icon portion
                drawShape(renderer, topBarIcons[i].shape, 5 + (i * 60), 10);

                // Used number - top right
                SDL_Surface* usedSurface = TTF_RenderText_Solid(font, std::to_string(topBarIcons[i].used).c_str(), yellow);
                SDL_Texture* usedTexture = SDL_CreateTextureFromSurface(renderer, usedSurface);
                SDL_Rect usedRect = {40 + (i * 60), 10, usedSurface->w, usedSurface->h};
                SDL_RenderCopy(renderer, usedTexture, NULL, &usedRect);
                SDL_FreeSurface(usedSurface);
                SDL_DestroyTexture(usedTexture);

                // Total number - bottom right
                SDL_Surface* totalSurface = TTF_RenderText_Solid(font, std::to_string(topBarIcons[i].total).c_str(), yellow);
                SDL_Texture* totalTexture = SDL_CreateTextureFromSurface(renderer, totalSurface);
                SDL_Rect totalRect = {40 + (i * 60), 40, totalSurface->w, totalSurface->h};
                SDL_RenderCopy(renderer, totalTexture, NULL, &totalRect);
                SDL_FreeSurface(totalSurface);
                SDL_DestroyTexture(totalTexture);
            } else {
                // Single colored background
                SDL_SetRenderDrawColor(renderer, 
                    topBarIcons[i].bgColor.r, 
                    topBarIcons[i].bgColor.g, 
                    topBarIcons[i].bgColor.b, 255);
                SDL_Rect iconBg = {5 + (i * 60), 5, 55, 65};
                SDL_RenderFillRect(renderer, &iconBg);

                // Draw shape
                drawShape(renderer, topBarIcons[i].shape, 5 + (i * 60), 10);

                // Single number - bottom right
                SDL_Surface* totalSurface = TTF_RenderText_Solid(font, std::to_string(topBarIcons[i].total).c_str(), yellow);
                SDL_Texture* totalTexture = SDL_CreateTextureFromSurface(renderer, totalSurface);
                SDL_Rect totalRect = {40 + (i * 60), 40, totalSurface->w, totalSurface->h};
                SDL_RenderCopy(renderer, totalTexture, NULL, &totalRect);
                SDL_FreeSurface(totalSurface);
                SDL_DestroyTexture(totalTexture);
            }
        }

        // draw ui
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    SDL_Quit();
    
    return 0;
}