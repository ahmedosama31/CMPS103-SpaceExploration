#pragma once
#include <iostream>
using namespace std;

class MarsStation;  // forward declaration

class UI {
public:
    UI(){}

    // Called once per day by Simulator
    void PrintDay(int day, MarsStation* pS);

    // Optional for later phases
    int ReadMode();
};
