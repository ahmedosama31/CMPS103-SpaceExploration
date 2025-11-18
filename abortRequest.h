#pragma once
#include "Requests.h"
class abortRequest :public Requests
{
	public:
		abortRequest(int RDay, int ID) : Requests(RDay, ID) {}
	
};

