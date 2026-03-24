#include <catch2/catch_test_macros.hpp>
#include "core/systems/date_system.h"

TEST_CASE("date advances by one day", "[date]") {
    DateComponent d{1, 1, 1312};
    DateSystem::advance(d);
    REQUIRE(d.day == 2);
    REQUIRE(d.month == 1);
    REQUIRE(d.year == 1312);
}

TEST_CASE("date rolls over to next month", "[date]") {
    DateComponent d{31, 1, 1312};
    DateSystem::advance(d);
    REQUIRE(d.day == 1);
    REQUIRE(d.month == 2);
    REQUIRE(d.year == 1312);
}

TEST_CASE("date rolls over to next year", "[date]") {
    DateComponent d{31, 12, 1312};
    DateSystem::advance(d);
    REQUIRE(d.day == 1);
    REQUIRE(d.month == 1);
    REQUIRE(d.year == 1313);
}

TEST_CASE("date toString formats correctly", "[date]") {
    DateComponent d{2, 5, 1312};
    REQUIRE(DateSystem::toString(d) == "May 2, 1312");
}