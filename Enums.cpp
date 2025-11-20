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
    switch (c) {
    case 'D': return RoverType::Digging;
    case 'P': return RoverType::Polar;
    case 'N': return RoverType::Normal;
    default: throw "Invalid char for RoverType";
    }
}

char RoverTypeToChar(RoverType t) {
    switch (t) {
    case RoverType::Digging: return 'D';
    case RoverType::Polar:   return 'P';
    case RoverType::Normal:  return 'N';
    default: throw "Invalid RoverType value";
    }
}