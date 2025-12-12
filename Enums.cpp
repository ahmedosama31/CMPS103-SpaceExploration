#include "Enums.h"

MissionType CharToMissionType(char c) {
    switch (c) {
    case 'D': return MissionType::Digging;
    case 'P': return MissionType::Polar;
    case 'N': return MissionType::Normal;
    default: throw "Invalid char for MissionType";
    }
}

char MissionTypeToChar(MissionType t) {
    switch (t) {
    case MissionType::Digging: return 'D';
    case MissionType::Polar:   return 'P';
    case MissionType::Normal:  return 'N';
    default: throw "Invalid MissionType value";
    }
}

RoverType CharToRoverType(char c) {
    if (c == 'D') return RoverType::Digging;
    if (c == 'P') return RoverType::Polar;
    if (c == 'N') return RoverType::Normal;
    if (c == 'R') return RoverType::Rescue;
    return RoverType::Normal;
}

char RoverTypeToChar(RoverType t) {
    if (t == RoverType::Digging) return 'D';
    if (t == RoverType::Polar) return 'P';
    if (t == RoverType::Normal) return 'N';
    if (t == RoverType::Rescue) return 'R';
    return 'N';
}