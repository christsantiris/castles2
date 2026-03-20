#pragma once
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

struct Province {
    int id;
    std::string name;
    std::string owner;
    std::string resource = "Unknown";
    std::vector<SDL_Point> polygon;
    bool isSelected = false;

    bool containsPoint(int x, int y) const;
    std::vector<int> neighbors; // calculate if province is adjacent
};

class Map {
public:
    std::vector<Province> provinces;

    void load(const std::string& path);
    void handleClick(int x, int y);
    void render(SDL_Renderer* renderer, TTF_Font* font);
};
