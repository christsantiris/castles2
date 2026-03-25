#include "hall_of_fame_system.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <algorithm>

using json = nlohmann::json;

namespace HallOfFameSystem {

    HallOfFame load(const std::string& path) {
        HallOfFame hof;
        std::ifstream file(path);
        if (!file.is_open()) return hof;

        json data = json::parse(file);
        for (auto& e : data["entries"]) {
            HallOfFameEntry entry;
            entry.dynasty = e["dynasty"];
            entry.score   = e["score"];
            entry.date    = e["date"];
            hof.entries.push_back(entry);
        }
        return hof;
    }

    void save(const HallOfFame& hof, const std::string& path) {
        json data;
        data["entries"] = json::array();
        for (auto& e : hof.entries) {
            json entry;
            entry["dynasty"] = e.dynasty;
            entry["score"]   = e.score;
            entry["date"]    = e.date;
            data["entries"].push_back(entry);
        }
        std::ofstream file(path);
        file << data.dump(4);
    }

    void addEntry(HallOfFame& hof, const std::string& dynasty, int score, const std::string& date) {
        HallOfFameEntry entry;
        entry.dynasty = dynasty;
        entry.score   = score;
        entry.date    = date;
        hof.entries.push_back(entry);

        // Sort by score descending
        std::sort(hof.entries.begin(), hof.entries.end(),
            [](const HallOfFameEntry& a, const HallOfFameEntry& b) {
                return a.score > b.score;
            });

        // Keep top 5
        if (hof.entries.size() > 5)
            hof.entries.resize(5);
    }

} 