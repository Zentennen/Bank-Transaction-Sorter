#pragma once
#include "Functions.h"

int main(int argc, char* argv[]) {
    if (argc < 1) return -1;
    int year = stoi(argv[0]);
    vector<EconomyType> economyTypes;
    economyTypes.reserve(1000);
    if (!readData(economyTypes)) {
        return -1;
    }

    Book* book = xlCreateXMLBook();
    book->load(L"Economy.xlsx");
    Sheet* sheet = book->addSheet(to_wstring(year).c_str());
    sheet->writeStr(1, 2, L"Total");
    sheet->writeStr(1, 3, L"Jan");
    sheet->writeStr(1, 4, L"Feb");
    sheet->writeStr(1, 5, L"Mar");
    sheet->writeStr(1, 6, L"Apr");
    sheet->writeStr(1, 7, L"Maj");
    sheet->writeStr(1, 8, L"Jun");
    sheet->writeStr(1, 9, L"Jul");
    sheet->writeStr(1, 10, L"Aug");
    sheet->writeStr(1, 11, L"Sep");
    sheet->writeStr(1, 12, L"Okt");
    sheet->writeStr(1, 13, L"Nov");
    sheet->writeStr(1, 14, L"Dec");
    int count = 0;
    for (auto&& et : economyTypes) {
        double sum = et.sum(year);
        if (sum == .0) continue;
        ++count;
        sheet->writeStr(count + 2, 0, et.type.c_str());
        sheet->writeStr(count + 2, 2, toStr(sum).c_str());
        for (int j = 0; j < 12; j++) {
            sheet->writeStr(count + 2, j + 3, toStr(et.years[year][j]).c_str());
        }
    }
    book->save(L"Economy.xlsx");
    book->release();
}