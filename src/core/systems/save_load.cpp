#include "save_load.h"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

static json serialize_task_base(const TaskBase& t) {
    return {
        {"active", t.active},
        {"workersAssigned", t.workersAssigned},
        {"daysRequired", t.daysRequired},
        {"daysAccumulated", t.daysAccumulated}
    };
}

static void deserialize_task_base(const json& j, TaskBase& t) {
    t.active = j.at("active");
    t.workersAssigned = j.at("workersAssigned");
    t.daysRequired = j.at("daysRequired");
    t.daysAccumulated = j.at("daysAccumulated");
}

static json serialize_battle_units(const std::vector<BattleUnit>& units) {
    json arr = json::array();
    for (const auto& u : units) {
        arr.push_back({{"x", u.x}, {"y", u.y}, {"health", u.health}, {"alive", u.alive}, {"type", u.type}});
    }
    return arr;
}

static std::vector<BattleUnit> deserialize_battle_units(const json& j) {
    std::vector<BattleUnit> units;
    for (const auto& u : j) {
        BattleUnit bu;
        bu.x = u.at("x");
        bu.y = u.at("y");
        bu.health = u.at("health");
        bu.alive = u.at("alive");
        bu.type = u.at("type");
        units.push_back(bu);
    }
    return units;
}

namespace SaveLoad {

void save_game(const World& world, const std::string& path) {
    json j;

    j["scoutedProvinces"] = std::vector<int>(world.scoutedProvinces.begin(), world.scoutedProvinces.end());

    json aiStatesJ = json::object();
    for (const auto& [dynasty, state] : world.aiStates) {
        aiStatesJ[dynasty] = {{"dayCounter", state.dayCounter}, {"recruitCounter", state.recruitCounter}};
    }
    j["aiStates"] = aiStatesJ;

    j["aiConfig"] = {
        {"tickInterval", world.aiConfig.tickInterval},
        {"recruitInterval", world.aiConfig.recruitInterval},
        {"stockWorkers", world.aiConfig.stockWorkers},
        {"militaryWorkers", world.aiConfig.militaryWorkers},
        {"dipWorkers", world.aiConfig.dipWorkers},
        {"startInfantry", world.aiConfig.startInfantry},
        {"startArchers", world.aiConfig.startArchers},
        {"startKnights", world.aiConfig.startKnights}
    };

    j["pendingMilitaryWorkers"] = world.pendingMilitaryWorkers;
    j["pendingDiplomaticWorkers"] = world.pendingDiplomaticWorkers;
    j["pendingBribeGold"] = world.pendingBribeGold;
    j["pendingTradeDynasty"] = world.pendingTradeDynasty;
    j["pendingTradeOfferRes"] = world.pendingTradeOfferRes;
    j["pendingTradeRequestRes"] = world.pendingTradeRequestRes;
    j["pendingTradeQty"] = world.pendingTradeQty;

    json provincesJ = json::array();
    for (const auto& p : world.provinces) {
        provincesJ.push_back({{"id", p.id}, {"owner", p.owner}, {"isRevealed", p.isRevealed}});
    }
    j["provinces"] = provincesJ;

    json armiesJ = json::object();
    for (const auto& [dynasty, army] : world.armies) {
        armiesJ[dynasty] = {{"infantry", army.infantry}, {"archers", army.archers}, {"knights", army.knights}};
    }
    j["armies"] = armiesJ;

    j["resources"] = {
        {"food", world.resources.food},
        {"timber", world.resources.timber},
        {"iron", world.resources.iron},
        {"gold", world.resources.gold}
    };

    j["workerPool"] = {
        {"totalStockWorkers", world.workerPool.totalStockWorkers},
        {"availableStockWorkers", world.workerPool.availableStockWorkers},
        {"totalMilitaryWorkers", world.workerPool.totalMilitaryWorkers},
        {"availableMilitaryWorkers", world.workerPool.availableMilitaryWorkers},
        {"totalDiplomaticWorkers", world.workerPool.totalDiplomaticWorkers},
        {"availableDiplomaticWorkers", world.workerPool.availableDiplomaticWorkers}
    };

    json collectionJ = json::array();
    for (int i = 0; i < 2; i++) {
        const auto& s = world.collectionTasks.slots[i];
        json sj = serialize_task_base(s);
        sj["resource"] = static_cast<int>(s.resource);
        collectionJ.push_back(sj);
    }
    j["collectionTasks"] = collectionJ;

    json combatJ = json::array();
    for (int i = 0; i < 2; i++) {
        const auto& s = world.combatTasks.slots[i];
        json sj = serialize_task_base(s);
        sj["targetProvinceId"] = s.targetProvinceId;
        combatJ.push_back(sj);
    }
    j["combatTasks"] = combatJ;

    json recruitJ = json::array();
    for (int i = 0; i < 2; i++) {
        const auto& s = world.recruitTasks.slots[i];
        json sj = serialize_task_base(s);
        sj["unitType"] = static_cast<int>(s.unitType);
        recruitJ.push_back(sj);
    }
    j["recruitTasks"] = recruitJ;

    json bribeJ = json::array();
    for (int i = 0; i < 2; i++) {
        const auto& s = world.bribeTasks.slots[i];
        json sj = serialize_task_base(s);
        sj["targetProvinceId"] = s.targetProvinceId;
        sj["goldAmount"] = s.goldAmount;
        bribeJ.push_back(sj);
    }
    j["bribeTasks"] = bribeJ;

    json scoutJ = json::array();
    for (int i = 0; i < 2; i++) {
        const auto& s = world.scoutTasks.slots[i];
        json sj = serialize_task_base(s);
        sj["targetProvinceId"] = s.targetProvinceId;
        scoutJ.push_back(sj);
    }
    j["scoutTasks"] = scoutJ;

    json tradeJ = json::array();
    {
        const auto& s = world.tradeTasks.slots[0];
        json sj = serialize_task_base(s);
        sj["offerRes"] = s.offerRes;
        sj["requestRes"] = s.requestRes;
        sj["qty"] = s.qty;
        sj["dynasty"] = s.dynasty;
        tradeJ.push_back(sj);
    }
    j["tradeTasks"] = tradeJ;

    j["date"] = {{"day", world.date.day}, {"month", world.date.month}, {"year", world.date.year}};

    j["ctx"] = {
        {"screen", static_cast<int>(world.ctx.screen)},
        {"playerDynasty", world.ctx.playerDynasty},
        {"score", world.ctx.score},
        {"dayCounter", world.ctx.dayCounter},
        {"activeTab", world.ctx.activeTab},
        {"battleMessage", world.ctx.battleMessage},
        {"battleMessageTimer", world.ctx.battleMessageTimer},
        {"victoryRecorded", world.ctx.victoryRecorded},
        {"shouldQuit", world.ctx.shouldQuit},
        {"upkeepInterval", world.ctx.upkeepInterval},
        {"upkeepCounter", world.ctx.upkeepCounter},
        {"upkeepDue", world.ctx.upkeepDue},
        {"paused", world.ctx.paused},
        {"marchFoodFlat", world.ctx.marchFoodFlat}
    };

    j["battle"] = {
        {"phase", static_cast<int>(world.battle.phase)},
        {"targetProvinceId", world.battle.targetProvinceId},
        {"targetProvinceName", world.battle.targetProvinceName},
        {"defenderDynasty", world.battle.defenderDynasty},
        {"playerUnits", serialize_battle_units(world.battle.playerUnits)},
        {"aiUnits", serialize_battle_units(world.battle.aiUnits)},
        {"playerHealth", world.battle.playerHealth},
        {"aiHealth", world.battle.aiHealth},
        {"playerMaxHealth", world.battle.playerMaxHealth},
        {"aiMaxHealth", world.battle.aiMaxHealth},
        {"roundTimer", world.battle.roundTimer},
        {"roundInterval", world.battle.roundInterval},
        {"transitionTimer", world.battle.transitionTimer},
        {"playerWon", world.battle.playerWon},
        {"statusText", world.battle.statusText}
    };

    json marchesJ = json::array();
    for (const auto& m : world.aiMarches) {
        marchesJ.push_back({
            {"active", m.active},
            {"dynasty", m.dynasty},
            {"targetProvinceId", m.targetProvinceId},
            {"daysAccumulated", m.daysAccumulated},
            {"daysRequired", m.daysRequired}
        });
    }
    j["aiMarches"] = marchesJ;

    j["pendingWorkers"] = {world.pendingWorkers[0], world.pendingWorkers[1], world.pendingWorkers[2], world.pendingWorkers[3]};

    std::ofstream file(path);
    file << j.dump(2);
}

bool load_game(World& world, const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return false;
    }

    json j;
    try {
        j = json::parse(file);

    world.scoutedProvinces.clear();
    for (int id : j.at("scoutedProvinces")) {
        world.scoutedProvinces.insert(id);
    }

    world.aiStates.clear();
    for (const auto& [dynasty, stateJ] : j.at("aiStates").items()) {
        AIState s;
        s.dayCounter = stateJ.at("dayCounter");
        s.recruitCounter = stateJ.at("recruitCounter");
        world.aiStates[dynasty] = s;
    }

    const auto& ac = j.at("aiConfig");
    world.aiConfig.tickInterval = ac.at("tickInterval");
    world.aiConfig.recruitInterval = ac.at("recruitInterval");
    world.aiConfig.stockWorkers = ac.at("stockWorkers");
    world.aiConfig.militaryWorkers = ac.at("militaryWorkers");
    world.aiConfig.dipWorkers = ac.at("dipWorkers");
    world.aiConfig.startInfantry = ac.at("startInfantry");
    world.aiConfig.startArchers = ac.at("startArchers");
    world.aiConfig.startKnights = ac.at("startKnights");

    world.pendingMilitaryWorkers = j.at("pendingMilitaryWorkers");
    world.pendingDiplomaticWorkers = j.at("pendingDiplomaticWorkers");
    world.pendingBribeGold = j.at("pendingBribeGold");
    world.pendingTradeDynasty = j.at("pendingTradeDynasty");
    world.pendingTradeOfferRes = j.at("pendingTradeOfferRes");
    world.pendingTradeRequestRes = j.at("pendingTradeRequestRes");
    world.pendingTradeQty = j.at("pendingTradeQty");

    // Provinces: patch mutable fields into the already-loaded map data
    for (const auto& pj : j.at("provinces")) {
        int id = pj.at("id");
        for (auto& p : world.provinces) {
            if (p.id == id) {
                p.owner = pj.at("owner");
                p.isRevealed = pj.at("isRevealed");
                break;
            }
        }
    }

    world.armies.clear();
    for (const auto& [dynasty, armyJ] : j.at("armies").items()) {
        ArmyComponent a;
        a.owner = dynasty;
        a.infantry = armyJ.at("infantry");
        a.archers = armyJ.at("archers");
        a.knights = armyJ.at("knights");
        world.armies[dynasty] = a;
    }

    const auto& rj = j.at("resources");
    world.resources.food = rj.at("food");
    world.resources.timber = rj.at("timber");
    world.resources.iron = rj.at("iron");
    world.resources.gold = rj.at("gold");

    const auto& wpj = j.at("workerPool");
    world.workerPool.totalStockWorkers = wpj.at("totalStockWorkers");
    world.workerPool.availableStockWorkers = wpj.at("availableStockWorkers");
    world.workerPool.totalMilitaryWorkers = wpj.at("totalMilitaryWorkers");
    world.workerPool.availableMilitaryWorkers = wpj.at("availableMilitaryWorkers");
    world.workerPool.totalDiplomaticWorkers = wpj.at("totalDiplomaticWorkers");
    world.workerPool.availableDiplomaticWorkers = wpj.at("availableDiplomaticWorkers");

    const auto& colJ = j.at("collectionTasks");
    for (int i = 0; i < 2; i++) {
        deserialize_task_base(colJ[i], world.collectionTasks.slots[i]);
        world.collectionTasks.slots[i].resource = static_cast<ResourceType>(colJ[i].at("resource").get<int>());
    }

    const auto& comJ = j.at("combatTasks");
    for (int i = 0; i < 2; i++) {
        deserialize_task_base(comJ[i], world.combatTasks.slots[i]);
        world.combatTasks.slots[i].targetProvinceId = comJ[i].at("targetProvinceId");
    }

    const auto& recJ = j.at("recruitTasks");
    for (int i = 0; i < 2; i++) {
        deserialize_task_base(recJ[i], world.recruitTasks.slots[i]);
        world.recruitTasks.slots[i].unitType = static_cast<UnitType>(recJ[i].at("unitType").get<int>());
    }

    const auto& briJ = j.at("bribeTasks");
    for (int i = 0; i < 2; i++) {
        deserialize_task_base(briJ[i], world.bribeTasks.slots[i]);
        world.bribeTasks.slots[i].targetProvinceId = briJ[i].at("targetProvinceId");
        world.bribeTasks.slots[i].goldAmount = briJ[i].at("goldAmount");
    }

    const auto& scJ = j.at("scoutTasks");
    for (int i = 0; i < 2; i++) {
        deserialize_task_base(scJ[i], world.scoutTasks.slots[i]);
        world.scoutTasks.slots[i].targetProvinceId = scJ[i].at("targetProvinceId");
    }

    const auto& trJ = j.at("tradeTasks");
    deserialize_task_base(trJ[0], world.tradeTasks.slots[0]);
    world.tradeTasks.slots[0].offerRes = trJ[0].at("offerRes");
    world.tradeTasks.slots[0].requestRes = trJ[0].at("requestRes");
    world.tradeTasks.slots[0].qty = trJ[0].at("qty");
    world.tradeTasks.slots[0].dynasty = trJ[0].at("dynasty");

    const auto& dj = j.at("date");
    world.date.day = dj.at("day");
    world.date.month = dj.at("month");
    world.date.year = dj.at("year");

    const auto& cxj = j.at("ctx");
    world.ctx.screen = static_cast<GameScreen>(cxj.at("screen").get<int>());
    world.ctx.playerDynasty = cxj.at("playerDynasty");
    world.ctx.score = cxj.at("score");
    world.ctx.dayCounter = cxj.at("dayCounter");
    world.ctx.activeTab = cxj.at("activeTab");
    world.ctx.battleMessage = cxj.at("battleMessage");
    world.ctx.battleMessageTimer = cxj.at("battleMessageTimer");
    world.ctx.victoryRecorded = cxj.at("victoryRecorded");
    world.ctx.shouldQuit = cxj.at("shouldQuit");
    world.ctx.upkeepInterval = cxj.at("upkeepInterval");
    world.ctx.upkeepCounter = cxj.at("upkeepCounter");
    world.ctx.upkeepDue = cxj.at("upkeepDue");
    world.ctx.paused = cxj.at("paused");
    world.ctx.marchFoodFlat = cxj.at("marchFoodFlat");
    world.ctx.shouldQuit = false;

    const auto& bj = j.at("battle");
    world.battle.phase = static_cast<BattlePhase>(bj.at("phase").get<int>());
    world.battle.targetProvinceId = bj.at("targetProvinceId");
    world.battle.targetProvinceName = bj.at("targetProvinceName");
    world.battle.defenderDynasty = bj.at("defenderDynasty");
    world.battle.playerUnits = deserialize_battle_units(bj.at("playerUnits"));
    world.battle.aiUnits = deserialize_battle_units(bj.at("aiUnits"));
    world.battle.playerHealth = bj.at("playerHealth");
    world.battle.aiHealth = bj.at("aiHealth");
    world.battle.playerMaxHealth = bj.at("playerMaxHealth");
    world.battle.aiMaxHealth = bj.at("aiMaxHealth");
    world.battle.roundTimer = bj.at("roundTimer");
    world.battle.roundInterval = bj.at("roundInterval");
    world.battle.transitionTimer = bj.at("transitionTimer");
    world.battle.playerWon = bj.at("playerWon");
    world.battle.statusText = bj.at("statusText");

    world.aiMarches.clear();
    for (const auto& mj : j.at("aiMarches")) {
        AIMarchTask m;
        m.active = mj.at("active");
        m.dynasty = mj.at("dynasty");
        m.targetProvinceId = mj.at("targetProvinceId");
        m.daysAccumulated = mj.at("daysAccumulated");
        m.daysRequired = mj.at("daysRequired");
        world.aiMarches.push_back(m);
    }

    const auto& pwj = j.at("pendingWorkers");
    for (int i = 0; i < 4; i++) {
        world.pendingWorkers[i] = pwj[i];
    }

    } catch (...) {
        return false;
    }

    return true;
}

} // namespace SaveLoad
