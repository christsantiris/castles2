#include "date_system.h"

namespace DateSystem {

    int daysInMonth(int month, int year) {
        int days[] = {31,28,31,30,31,30,31,31,30,31,30,31};
        return days[month - 1];
    }

    void advance(DateComponent& date) {
        date.day++;
        if (date.day > daysInMonth(date.month, date.year)) {
            date.day = 1;
            date.month++;
            if (date.month > 12) {
                date.month = 1;
                date.year++;
            }
        }
    }

    std::string toString(const DateComponent& date) {
        const char* months[] = {
            "Jan","Feb","Mar","Apr","May","Jun",
            "Jul","Aug","Sep","Oct","Nov","Dec"
        };
        return std::string(months[date.month - 1]) + " " +
               std::to_string(date.day) + ", " +
               std::to_string(date.year);
    }

}