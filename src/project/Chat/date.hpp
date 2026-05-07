#pragma once
#include <ctime>

struct Date {
    int min_;
    int hour_;
    int day_;
    int month_;
    int year_;
    Date() : min_(0), hour_(0), day_(0), month_(0), year_(0) {}
    Date(int min, int hour, int day, int month, int year);
    static Date now();
};