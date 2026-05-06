#include "date.hpp"

Date::Date(int min, int hour, int day, int month, int year) :
min_{min}, hour_{hour}, day_{day}, month_{month}, year_{year} {}

Date Date::now() {
    time_t t = time(nullptr);
    tm *n = localtime(&t);
    return Date(n->tm_min, n->tm_hour, n->tm_mday, n->tm_mon + 1, n->tm_year - 100);
}