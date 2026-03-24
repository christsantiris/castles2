#include "world.h"

ProvinceComponent* World::findProvince(int id) {
    for (auto& p : provinces)
        if (p.id == id) return &p;
    return nullptr;
}

int World::countOwnedProvinces(const std::string& dynasty, ResourceType res) const {
    const char* resNames[] = {"Food", "Timber", "Iron", "Gold", "None"};
    std::string resName = resNames[(int)res];
    int count = 0;
    for (auto& p : provinces)
        if (p.owner == dynasty && p.resource == resName)
            count++;
    return count;
}

int World::countOwnedProvinces(const std::string& dynasty) const {
    int count = 0;
    for (auto& p : provinces)
        if (p.owner == dynasty)
            count++;
    return count;
}

bool World::isDefeated(const std::string& dynasty) const {
    return countOwnedProvinces(dynasty) == 0;
}