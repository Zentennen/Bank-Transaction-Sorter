#pragma once
#include <filesystem>
#include <codecvt>
#include <algorithm>
#include "Functions.h"

int main(int argc, char* argv[])
{
    if (argc < 1) return -1;
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    str path = converter.from_bytes(argv[0]);
    Book* book = xlCreateXMLBook();
    if (!book->loadSheet(path.c_str(), 0)) {
        printf("ERROR: Could not find input sheet\n");
        return -1;
    }
    Sheet* in = book->getSheet(0);
    ifstream is("Economy/Status.txt");
    if (!is.is_open()) {
        printf("ERROR: Could not open Status.txt\n");
        return -1;
    }
    string line;
    getline(is, line);
    if (line != "") {
        printf("ERROR: First line of Status.txt is not empty");
        return -1;
    }
    getline(is, line);
    int startRow = stoi(line);
    is.close();
    vector<EconomyType> economyTypes;
    economyTypes.reserve(1000);
    bool finalRow = false;
    if (!readData(economyTypes)) {
        return -1;
    }
    int stop = startRow + 50;
    if (stop > in->lastRow()) {
        stop = in->lastRow();
        finalRow = true;
    }
    for (int i = startRow; i < stop; i++) {
        auto d = in->readStr(i, 0);
        str date(d);
        int month = stoi(date.substr(5, 2)) - 1;
        int year = stoi(date.substr(0, 4));
        auto type = in->readStr(i, 1);
        auto amount = in->readNum(i, 2);
        handleEconomyRow(economyTypes, type, year, month, amount);
    }
    vector<int> years;
    years.reserve(10);
    for (auto&& type : economyTypes) {
        for (auto&& year : type.years) {
            bool found = false;
            for (auto&& i : years) {
                if (i == year.first) {
                    found = true;
                    break;
                }
            }
            if (!found) years.push_back(year.first);
        }
    }
    sort(years.begin(), years.end());
    writeData(economyTypes);
    ofstream os("Economy/Status.txt");
    if (!os.is_open()) {
        printf("ERROR: Could not open Status.txt");
        return -1;
    }
    if (finalRow) {
        for (auto&& year : years) {
            os << year << " ";
        }
    }
    os << "\n" << stop;
    os.close();
    book->release();
    return 0;
}