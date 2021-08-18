#pragma once
#include "Types.h"

EconomyType::EconomyType(const str& s) 
    : type(s) 
{

}

EconomyType::EconomyType(const str& s, const vector<str>& types)
    : type(s), subtypes(types)
{
    
}

EconomyType EconomyType::operator+(const EconomyType& other)
{
    EconomyType ret(type);
    ret += other;
    return ret;
}

EconomyType& EconomyType::operator+=(const EconomyType& other) {
    for (auto&& year : other.years) {
        addYear(year.first, year.second);
    }
    return *this;
}

void EconomyType::addYear(const int year, const array<double, 12> months)
{
    auto it = years.find(year);
    if (it == years.end()) {
        years.emplace(year, months);
    }
    else {
        FOR(12) {
            it->second[i] += months[i];
        }
    }
}

void EconomyType::addMonth(const int year, const int month, const double amount)
{
    auto it = years.find(year);
    if (it == years.end()) {
        array<double, 12> months;
        months.fill(.0);
        months[month] = amount;
        years.emplace(year, months);
    }
    else {
        it->second[month] += amount;
    }
}

double EconomyType::sum() const
{
    double ret = 0.0;
    for (auto&& year : years) {
        FOR(12) {
            ret += year.second[i];
        }
    }
    return ret;
}

double EconomyType::sum(const int year) const
{
    double ret = 0.0;
    auto it = years.find(year);
    if (it != years.end()) {
        FOR(12) {
            ret += it->second[i];
        }
    }
    return ret;
}

bool EconomyType::addIfSubtype(const EconomyType& other)
{
    FOR(subtypes.size()) {
        size_t pos = other.type.find(subtypes[i]);
        if (pos < other.type.length()) {
            *this += other;
            return true;
        }
    }
    return false;
}
