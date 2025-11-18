#pragma once
enum class MissionType {
	Digging,
	Polar,
	Normal
};

MissionType CharToMType(char c) {
    switch (c) {
    case 'D': return MissionType::Digging;
    case 'P': return MissionType::Polar;
    case 'N': return MissionType::Normal;
    default:
        throw ("Invalid char for MissionType");
    }
}

char MTypeToChar(MissionType t) {
    switch (t) {
    case MissionType::Digging: return 'D';
    case MissionType::Polar:   return 'P';
    case MissionType::Normal:  return 'N';
    default:
        throw ("Invalid MissionType value");
    }
}
