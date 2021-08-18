#pragma once
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <chrono>
#include <thread>
#include <filesystem>
#include "Functions.h"

using namespace std;
using namespace std::filesystem;

void read() {
    for (auto& file : directory_iterator("Input")) {
        LPCSTR exe = "Economy/Reader.exe";
        string s = file.path().generic_string();
        LPSTR path = const_cast<LPSTR>(s.c_str());
        ofstream reset("Economy/Status.txt");
        reset << "\n4";
        reset.close();
        while (true) {
            wifstream stream("Economy/Status.txt");
            wstring years;
            getline(stream, years);
            stream.close();
            if (years != L"") {
                break;
            }
            STARTUPINFOA si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(si));
            si.cb = sizeof(si);
            ZeroMemory(&pi, sizeof(pi));
            CreateProcessA(exe, path, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }
}

void write(const vector<int>& years) {
    for (auto&& year : years) {
        LPCSTR p = "Economy/Writer.exe";
        string s = to_string(year);
        LPSTR y = const_cast<LPSTR>(s.c_str());
        STARTUPINFOA si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        CreateProcessA(p, y, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
}

vector<int> getYears() {
    vector<int> years;
    years.reserve(10);
    wifstream stream("Economy/Status.txt");
    if (!stream.is_open()) {
        return years;
    }
    str line;
    getline(stream, line);
    stream.close();
    auto yearStrings = separateByToken(line, L' ');
    for (auto&& s : yearStrings) {
        years.push_back(stoi(s));
    }
    return years;
}

int main()
{
    {// empty Economy Types.txt
        ofstream stream("Economy/Economy Types.txt");
        stream.close();
    }
    read();
    vector<int> years = getYears();
    vector<EconomyType> economyTypes;
    economyTypes.reserve(1000);
    if (!readData(economyTypes)) {
        return -1;
    }
    if (!sortEconomyTypes(economyTypes)) {
        return -1;
    }
    writeData(economyTypes);
    auto book = xlCreateXMLBook();
    Sheet* sheet = book->addSheet(L"Total");
    FOR(economyTypes.size()) {
        sheet->writeStr(i + 1, 0, economyTypes[i].type.c_str());
        sheet->writeStr(i + 1, 1, toStr(economyTypes[i].sum()).c_str());
    }
    book->save(L"Economy.xlsx");
    book->release();
    write(years);
    return 0;
}