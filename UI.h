#pragma once
#include <iostream>
using namespace std;

class MarsStation;  // forward declaration

class UI {
public:
    UI(){}

    void PrintDay(int day, MarsStation* pS);

    int ReadMode();
};
