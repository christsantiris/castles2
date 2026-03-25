#pragma once
#include "../world.h"
#include "../components.h"
#include <string>

namespace HallOfFameSystem {
    HallOfFame load(const std::string& path);
    void save(const HallOfFame& hof, const std::string& path);
    void addEntry(HallOfFame& hof, const std::string& dynasty, int score, const std::string& date);
}