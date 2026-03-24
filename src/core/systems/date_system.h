#pragma once
#include "../components.h"
#include <string>

namespace DateSystem {
    int  daysInMonth(int month, int year);
    void advance(DateComponent& date);
    std::string toString(const DateComponent& date);
}