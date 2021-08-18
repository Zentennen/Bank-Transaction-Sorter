#pragma once
#include <fstream>
#include "Types.h"
#include "..\..\libxl\include_cpp\libxl.h"

using namespace libxl;

Sheet* getSheetByName(Book* book, const str& name);
str toStr(const double& d);
bool shouldClean(const wchar_t& ch);
void cleanType(str& type);
vector<str> separateByToken(const str& s, const wchar_t t);
bool readData(std::vector<EconomyType>& economyTypes);
void writeData(const std::vector<EconomyType>& economyTypes);
bool sortEconomyTypes(vector<EconomyType>& economyTypes);
void handleEconomyRow(std::vector<EconomyType>& economyTypes, str type, const int& year, const int& month, const double& amount);