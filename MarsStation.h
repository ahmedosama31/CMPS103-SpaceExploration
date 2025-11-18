#pragma once

#include <iostream>
#include <fstream>
#include "LinkedQueue.h"
#include "rover.h"
#include "mission.h"

template <typename T>
class MarsStation {
private:

    MarsStation() {
        ifstream inputfile("input.txt");
        if (!inputfile.is_open()) {
            return;
        }

        int DR, PR, NR;
        inputfile >> DR >> PR >> NR;
        getline(inputfile);   // move to the next line

        int DS, PS, NS;
        inputfile >> DS >> PS >> NS;
        getline(inputfile);

        int MD, MP, MN, Duration;
        inputfile >> MD >> MP >> MN >> Duration;
        // Rover(int id, RoverType t, double spd, int missionsBC, int checkDur)

        // --- DIGGING ROVERS ---
        for (int i = 0; i < DR; i++) {
            Rover* diggingRover = new Rover(i + 1, DIGGING, DS, MD, Duration);
            DIG_Rovers.enqueue(diggingRover);
        }

        // --- POLAR ROVERS ---
        for (int i = 0; i < PR; i++) {
            Rover* polarRover = new Rover(i + 1 + DR, POLAR, PS, MP, Duration);
            POL_Rovers.enqueue(polarRover);
        }

        // --- NORMAL ROVERS ---
        for (int i = 0; i < NR; i++) {
            Rover* normalRover = new Rover(i + 1 + DR + PR, NORMAL, NS, MN, Duration);
            NM_Rovers.enqueue(normalRover);
        }
    }
};
