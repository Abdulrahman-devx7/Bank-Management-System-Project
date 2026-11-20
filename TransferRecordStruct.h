#pragma once
#pragma once
#include <string>
#include "Date.h"
#include "Time.h"
#include "User.h"

struct TransferRecordStruct
{
	Date transferDate;
	Time transferTime;
	string senderAccountNumber;
	string receiverAccountNumber;

	float transferredAmount = 0;
	float senderBalance = 0;
	float receiverBalance = 0;
	
	string transferExcecutor;


};

