#include "map.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

bool Province::containsPoint(int px, int py) const {
    int n = polygon.size();
    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        int xi = polygon[i].x, yi = polygon[i].y;
        int xj = polygon[j].x, yj = polygon[j].y;
        bool intersect = ((yi > py) != (yj > py)) &&
                         (px < (xj - xi) * (py - yi) / (yj - yi) + xi);
        if (intersect) inside = !inside;
    }
    return inside;
}

void Map::load(const std::string& path) {
    std::ifstream file(path);
    json data = json::parse(file);

    for (auto& p : data["provinces"]) {
        Province province;
        province.id    = p["id"];
        province.name  = p["name"];
        province.owner = p["owner"];

        for (auto& point : p["polygon"]) {
            SDL_Point pt;
            pt.x = point["x"];
            pt.y = point["y"];
            province.polygon.push_back(pt);
        }
        provinces.push_back(province);
    }
}

void Map::handleClick(int x, int y) {
    for (auto& p : provinces) p.isSelected = false;
    for (auto& p : provinces) {
        if (p.containsPoint(x, y)) {
            p.isSelected = true;
            break;
        }
    }
}

void Map::render(SDL_Renderer* renderer, TTF_Font* font) {
    // Fill provinces
    for (auto& p : provinces) {
        if (p.name == "Aegean Sea") { // Creating ocean as province to make rendering simpler
            SDL_SetRenderDrawColor(renderer, 30, 80, 150, 255);
        } else if (p.isSelected) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        } else {
            SDL_SetRenderDrawColor(renderer, 34, 85, 34, 255);
        }
        int minX = p.polygon[0].x, minY = p.polygon[0].y;
        int maxX = p.polygon[0].x, maxY = p.polygon[0].y;
        for (auto& pt : p.polygon) {
            minX = std::min(minX, pt.x);
            minY = std::min(minY, pt.y);
            maxX = std::max(maxX, pt.x);
            maxY = std::max(maxY, pt.y);
        }
        SDL_Rect fill = {minX, minY, maxX - minX, maxY - minY};
        SDL_RenderFillRect(renderer, &fill);
    }

    // Draw province borders
    for (auto& p : provinces) {
        SDL_SetRenderDrawColor(renderer, 200, 170, 50, 255);
        int n = p.polygon.size();
        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            SDL_RenderDrawLine(renderer,
                p.polygon[i].x, p.polygon[i].y,
                p.polygon[j].x, p.polygon[j].y);
        }
    }

    // Draw province labels
    for (auto& p : provinces) {
        if (p.polygon.empty()) continue;
        int cx = 0, cy = 0;
        for (auto& pt : p.polygon) {
            cx += pt.x;
            cy += pt.y;
        }
        cx /= p.polygon.size();
        cy /= p.polygon.size();

        SDL_Color yellow = {255, 215, 0, 255};
        SDL_Surface* s = TTF_RenderText_Solid(font, p.name.c_str(), yellow);
        SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
        SDL_Rect r = {cx - s->w / 2, cy - s->h / 2, s->w, s->h};
        SDL_RenderCopy(renderer, t, NULL, &r);
        SDL_FreeSurface(s);
        SDL_DestroyTexture(t);
    }
}