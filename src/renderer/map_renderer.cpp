#include "map_renderer.h"
#include <algorithm>

namespace MapRenderer {

    static const int MAP_Y = 60;
    static const int MAP_HEIGHT  = 740;
    static const float MAP_SCALE = 740.0f / 880.0f;

    static const SDL_Color WATER     = {180, 210, 230, 255};
    static const SDL_Color BORDER    = {100,  80,  40, 255};
    static const SDL_Color LABEL     = {255, 215,   0, 255};
    static const SDL_Color LABEL_SEL = {255, 255, 255, 255};

    static const float MAP_SCALE_Y = 740.0f / 880.0f;
    static const float MAP_SCALE_X = 950.0f / 940.0f;

    static std::vector<Point> offsetPolygon(const std::vector<Point>& polygon) {
        std::vector<Point> result;
        for (auto& pt : polygon)
            result.push_back({
                (int)(pt.x * MAP_SCALE_X),
                (int)(pt.y * MAP_SCALE_Y) + MAP_Y
            });
        return result;
    }

    static void fillPolygon(SDL_Renderer* renderer, const std::vector<Point>& polygon, SDL_Color color) {
        if (polygon.size() < 3) return;

        int minY = polygon[0].y, maxY = polygon[0].y;
        for (auto& pt : polygon) {
            minY = std::min(minY, pt.y);
            maxY = std::max(maxY, pt.y);
        }

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

        for (int y = minY; y <= maxY; y++) {
            std::vector<int> intersections;
            int n = polygon.size();
            for (int i = 0, j = n - 1; i < n; j = i++) {
                int yi = polygon[i].y, yj = polygon[j].y;
                int xi = polygon[i].x, xj = polygon[j].x;
                if ((yi <= y && yj > y) || (yj <= y && yi > y)) {
                    int x = xi + (y - yi) * (xj - xi) / (yj - yi);
                    intersections.push_back(x);
                }
            }
            std::sort(intersections.begin(), intersections.end());
            for (int i = 0; i + 1 < (int)intersections.size(); i += 2)
                SDL_RenderDrawLine(renderer, intersections[i], y, intersections[i+1], y);
        }
    }

    static void drawBorder(SDL_Renderer* renderer, const std::vector<Point>& polygon) {
        int n = polygon.size();
        SDL_SetRenderDrawColor(renderer, BORDER.r, BORDER.g, BORDER.b, BORDER.a);
        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            SDL_RenderDrawLine(renderer,
                polygon[i].x, polygon[i].y,
                polygon[j].x, polygon[j].y);
        }
    }

    static void drawLabel(SDL_Renderer* renderer, TTF_Font* font,
                          const std::string& text, const std::vector<Point>& polygon,
                          bool selected) {
        int cx = 0, cy = 0;
        for (auto& pt : polygon) { cx += pt.x; cy += pt.y; }
        cx /= polygon.size();
        cy /= polygon.size();

        SDL_Color color = selected ? LABEL_SEL : LABEL;
        SDL_Surface* s = TTF_RenderText_Solid(font, text.c_str(), color);
        if (!s) return;
        SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
        SDL_Rect r = {cx - s->w / 2, cy - s->h / 2, s->w, s->h};
        SDL_RenderCopy(renderer, t, nullptr, &r);
        SDL_FreeSurface(s);
        SDL_DestroyTexture(t);
    }

    void render(SDL_Renderer* renderer, TTF_Font* font, const World& world) {
        SDL_Rect mapArea = {0, MAP_Y, 950, 740};
        SDL_RenderSetClipRect(renderer, &mapArea);

        // Water background
        SDL_SetRenderDrawColor(renderer, WATER.r, WATER.g, WATER.b, WATER.a);
        SDL_RenderFillRect(renderer, &mapArea);

        // Fill provinces
        for (auto& p : world.provinces) {
            SDL_Color color = p.isSelected
                ? SDL_Color{144, 238, 144, 255}
                : SDL_Color{34, 139, 34, 255};
            fillPolygon(renderer, offsetPolygon(p.polygon), color);
        }

        // Draw borders
        for (auto& p : world.provinces)
            drawBorder(renderer, offsetPolygon(p.polygon));

        // Draw labels
        for (auto& p : world.provinces)
            drawLabel(renderer, font, p.name, offsetPolygon(p.polygon), p.isSelected);

        SDL_RenderSetClipRect(renderer, nullptr);
    }

}