#pragma once
enum class MissionType {
    Digging,
    Polar,
    Normal
};

enum class RoverType {
    Digging,
    Polar,
    Normal
};

MissionType CharToMissionType(char c);
char MissionTypeToChar(MissionType t);
RoverType CharToRoverType(char c);
char RoverTypeToChar(RoverType t);

