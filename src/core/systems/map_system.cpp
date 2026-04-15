#include "map_system.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

static const int MAP_Y = 60;

namespace MapSystem {

    void load(World& world, const std::string& path) {
        std::ifstream file(path);
        json data = json::parse(file);

        for (auto& p : data["provinces"]) {
            ProvinceComponent province;
            province.id       = p["id"];
            province.owner    = p["owner"];
            province.resource = p["resource"];
#ifdef DEBUG
            province.name = std::string(p["name"]) + " " + std::to_string((int)p["id"]);
#else
            province.name     = p["name"];
#endif

            for (auto& point : p["polygon"]) {
                Point pt;
                pt.x = point["x"];
                pt.y = point["y"];
                province.polygon.push_back(pt);
            }

            for (auto& neighbor : p["neighbors"])
                province.neighbors.push_back(neighbor);

            world.provinces.push_back(province);
        }
    }

    bool containsPoint(const ProvinceComponent& province, int px, int py) {
        int n = province.polygon.size();
        bool inside = false;
        for (int i = 0, j = n - 1; i < n; j = i++) {
            int xi = province.polygon[i].x, yi = province.polygon[i].y;
            int xj = province.polygon[j].x, yj = province.polygon[j].y;
            bool intersect = ((yi > py) != (yj > py)) &&
                             (px < (xj - xi) * (py - yi) / (yj - yi) + xi);
            if (intersect) inside = !inside;
        }
        return inside;
    }

    ProvinceComponent* handleClick(World& world, int x, int y) {
        float scaleX = 940.0f / 950.0f;
        float scaleY = 880.0f / 740.0f;
        int adjustedX = (int)(x * scaleX);
        int adjustedY = (int)((y - 60) * scaleY);

        for (auto& p : world.provinces) {
            if (!containsPoint(p, adjustedX, adjustedY)) continue;
            bool wasSelected = p.isSelected;
            deselectAll(world);
            if (!wasSelected) {
                p.isSelected = true;
                return &p;
            }
            return nullptr;
        }
        deselectAll(world);
        return nullptr;
    }

    void deselectAll(World& world) {
        for (auto& p : world.provinces)
            p.isSelected = false;
    }

}