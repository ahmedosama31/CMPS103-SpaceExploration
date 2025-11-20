#pragma once
#include "Requests.h"

class MarsStation; // forward declaration

class abortRequest : public Requests
{
public:
    abortRequest(int RDay, int ID) : Requests(RDay, ID) {}
    void Operate(MarsStation& marsStation) override; 
    void print(std::ostream& os) const override;
};

