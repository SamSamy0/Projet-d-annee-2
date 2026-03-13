#pragma once

struct Date {
    int day_;
    int month_;
    int year_;
    Date() : day_(0), month_(0), year_(0) {}
    Date(int d, int m, int y);
};