#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

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
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        // Top bar - dark gray
        SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
        SDL_Rect topBar = {0, 0, 1024, 80};
        SDL_RenderFillRect(renderer, &topBar);

        // Main map area - dark green
        SDL_SetRenderDrawColor(renderer, 34, 85, 34, 255);
        SDL_Rect mapArea = {0, 80, 740, 688};
        SDL_RenderFillRect(renderer, &mapArea);

        // Right sidebar - gray stone color
        SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
        SDL_Rect sidebar = {740, 80, 284, 688};
        SDL_RenderFillRect(renderer, &sidebar);

        // Map border - gold color
        SDL_SetRenderDrawColor(renderer, 200, 170, 50, 255);
        SDL_Rect mapBorder = {0, 80, 740, 688};
        SDL_RenderDrawRect(renderer, &mapBorder);

        // Sidebar border - dark gray
        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_Rect sidebarBorder = {740, 80, 284, 688};
        SDL_RenderDrawRect(renderer, &sidebarBorder);

        // Tab bar background - dark brown
        SDL_SetRenderDrawColor(renderer, 60, 30, 10, 255);
        SDL_Rect tabBar = {740, 80, 284, 40};
        SDL_RenderFillRect(renderer, &tabBar);

        // add rectangles for task labels
        // STOCK - green
        SDL_SetRenderDrawColor(renderer, 0, 120, 0, 255);
        SDL_Rect stockTab = {740, 80, 71, 40};
        SDL_RenderFillRect(renderer, &stockTab);

        // ARMY - red
        SDL_SetRenderDrawColor(renderer, 150, 0, 0, 255);
        SDL_Rect armyTab = {811, 80, 71, 40};
        SDL_RenderFillRect(renderer, &armyTab);

        // RELAT - blue
        SDL_SetRenderDrawColor(renderer, 0, 0, 150, 255);
        SDL_Rect relatTab = {882, 80, 71, 40};
        SDL_RenderFillRect(renderer, &relatTab);

        // OPTS - brown
        SDL_SetRenderDrawColor(renderer, 101, 67, 33, 255);
        SDL_Rect optsTab = {953, 80, 71, 40};
        SDL_RenderFillRect(renderer, &optsTab);

        // Add labels for tasks
        // stock task
        SDL_Color white = {255, 255, 255, 255};
        SDL_Surface* surface = TTF_RenderText_Solid(font, "STOCK", white);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect textRect = {750, 88, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &textRect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        // army task
        SDL_Surface* surface2 = TTF_RenderText_Solid(font, "ARMY", white);
        SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
        SDL_Rect textRect2 = {820, 88, surface2->w, surface2->h};
        SDL_RenderCopy(renderer, texture2, NULL, &textRect2);
        SDL_FreeSurface(surface2);
        SDL_DestroyTexture(texture2);

        //relations task
        SDL_Surface* surface3 = TTF_RenderText_Solid(font, "RELAT", white);
        SDL_Texture* texture3 = SDL_CreateTextureFromSurface(renderer, surface3);
        SDL_Rect textRect3 = {890, 88, surface3->w, surface3->h};
        SDL_RenderCopy(renderer, texture3, NULL, &textRect3);
        SDL_FreeSurface(surface3);
        SDL_DestroyTexture(texture3);

        // game options
        SDL_Surface* surface4 = TTF_RenderText_Solid(font, "OPTS", white);
        SDL_Texture* texture4 = SDL_CreateTextureFromSurface(renderer, surface4);
        SDL_Rect textRect4 = {960, 88, surface4->w, surface4->h};
        SDL_RenderCopy(renderer, texture4, NULL, &textRect4);
        SDL_FreeSurface(surface4);
        SDL_DestroyTexture(texture4);

        // draw ui
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);
    SDL_Quit();
    return 0;
}