// Date.cpp
#include "Date.h"

Date::Date() : year(0), month(0), day(0) {}

void Date::setDate(int y, int m, int d) {
    year = y;
    month = m;
    day = d;
}

std::string Date::getDate() const {
    std::stringstream ss;
    ss << year << "-" << (month < 10 ? "0" : "") << month << "-" << (day < 10 ? "0" : "") << day;
    return ss.str();
}
