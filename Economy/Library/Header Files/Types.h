#pragma once
#include <string>
#include <array>
#include <map>
#include <vector>

using namespace std;
typedef wstring str;
#define FOR(x) for(unsigned int i = 0; i < x; i++)

struct EconomyType {
    str type;
    vector<str> subtypes;
    map<int, array<double, 12>> years;
    EconomyType(const str& s);
    EconomyType(const str& s, const vector<str>& types);
    EconomyType operator+(const EconomyType& other);
    EconomyType& operator+=(const EconomyType& other);
    void addYear(const int year, const array<double, 12> months);
    void addMonth(const int year, const int month, const double amount);
    double sum() const;
    double sum(const int year) const;
    bool addIfSubtype(const EconomyType& other);
private:
    EconomyType() = delete;
};