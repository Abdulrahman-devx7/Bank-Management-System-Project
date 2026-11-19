#pragma once
#pragma once
#include <string>
#include "Date.h"
#include "Time.h"


class TransferRecordStruct
{
	Date transferDate;
	Time transferTime;
	string senderAccountNumber;
	string receiverAccountNumber;

	float transferredAmount = 0;



};

