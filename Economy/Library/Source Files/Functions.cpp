#pragma once
#include "Functions.h"

bool readData(std::vector<EconomyType>& economyTypes) {
    wifstream stream("Economy/Economy Types.txt");
    if (!stream.is_open()) {
        printf("ERROR: Could not open Economy Types.txt\n");
        return false;
    }
    str line;
    vector<str> lines;
    lines.reserve(1000);
    while (getline(stream, line)) {
        lines.push_back(line);
    }
    for (size_t i = 0; i < lines.size();) {
        economyTypes.push_back(lines[i]);
        size_t rows = stoi(lines[i+1]);
        for (size_t j = 2; j < rows + 2; j++) {
            auto elements = separateByToken(lines[i + j], L' ');
            array<double, 12> months = {
                stod(elements[1]), stod(elements[2]), stod(elements[3]),
                stod(elements[4]), stod(elements[5]), stod(elements[6]),
                stod(elements[7]), stod(elements[8]), stod(elements[9]),
                stod(elements[10]), stod(elements[11]), stod(elements[12])
            };
            economyTypes.back().addYear(stoi(elements[0]), months);
        }
        i += rows + 2;
    }
    stream.close();
    return true;
}

void writeData(const std::vector<EconomyType>& economyTypes) {
    wofstream stream("Economy/Economy Types.txt");
    FOR(economyTypes.size()) {
        stream << economyTypes[i].type << L"\n" << economyTypes[i].years.size() << L"\n";
        for (auto&& it : economyTypes[i].years) {
            stream << it.first;
            for (unsigned int j = 0; j < 12; j++) {
                stream << L" " << it.second[j];
            }
            stream << L"\n";
        }
    }
    stream.close();
}

bool sortEconomyTypes(vector<EconomyType>& economyTypes)
{
    vector<EconomyType> ret;
    ret.reserve(1000);
    wifstream subtypeData("Economy/Type Categories.txt");
    if (!subtypeData.is_open()) {
        printf("ERROR: Could not open Type Categories.txt\n");
        return false;
    }
    str line;
    bool subtype = false;
    while (getline(subtypeData, line)) {
        if (line == L"") {
            subtype = false;
        }
        else if (subtype) {
            ret.back().subtypes.push_back(line);
        }
        else {
            ret.push_back(EconomyType(line));
            subtype = true;
        }
    }
    for (auto it = economyTypes.begin(); it != economyTypes.end(); ++it) {
        FOR(ret.size()) {
            ret[i].addIfSubtype(*it);
        }
    }
    ret.insert(ret.end(), economyTypes.begin(), economyTypes.end());
    economyTypes = ret;
    return true;
}

Sheet* getSheetByName(Book* book, const str& name) {
    FOR((unsigned int)book->sheetCount()) {
        if (book->getSheetName(i) == name) return book->getSheet(i);
    }
    return nullptr;
}

bool shouldClean(const wchar_t& ch) {
    return ch == '0' || ch == '1' || ch == '2' || ch == '3' || ch == '4' || ch == '5' || ch == '6' || ch == '7' || ch == '8' || ch == '9' || ch == '-' || ch == ' ';
}

void cleanType(str& type) {
    int l = 0;
    FOR(type.length()) {
        if (shouldClean(type[i])) l++;
        else break;
    }
    type.erase(0, l);
}

vector<str> separateByToken(const str& s, const wchar_t t)
{
    vector<str> ret;
    ret.reserve(20);
    size_t pos = s.find_first_not_of(t);
    size_t pos2 = s.find_first_of(t, pos);
    size_t count = pos2 - pos;
    while (pos != str::npos) {
        ret.push_back(s.substr(pos, count));
        pos = s.find_first_not_of(t, pos2);
        pos2 = s.find_first_of(t, pos);
        count = pos2 - pos;
    }
    return ret;
}

void handleEconomyRow(std::vector<EconomyType>& economyTypes, str type, const int& year, const int& month, const double& amount) {
    cleanType(type);
    FOR(economyTypes.size()) {
        if (economyTypes[i].type == type) {
            economyTypes[i].addMonth(year, month, amount);
            return;
        }
    }
    economyTypes.push_back(EconomyType(type));
    economyTypes.back().addMonth(year, month, amount);
}

str toStr(const double& d) {
    str ret = to_wstring(d);
    bool foundDec = false;
    int off = 0;
    FOR(ret.length()) {
        if (foundDec && ret[i] != '0') off = i;
        else if (ret[i] == '.') {
            foundDec = true;
            off = i;
        }
    }
    if (foundDec) ret.erase(off);
    return ret;
}