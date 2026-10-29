#pragma once
#include <ctime>
class Time
{
private:

	short _hour = 0;
	short _minute = 0;
	short _second = 0;

	void SetHour(short hour)
	{
		_hour = hour;
	}

	void SetMinute(short minute)
	{
		_minute = minute;
	}

	void SetSecond(short second)
	{
		_second = second;
	}

public:

	short Hour() const
	{
		return _hour;
	}

	short Minute() const
	{
		return _minute;
	}

	short Second() const
	{
		return _second;
	}

	Time()
	{
		*this = GetSystemTime();
	}

	Time(short hour, short minute, short second)
	{
		if (IsValidTime(hour, minute, second))
		{
			SetHour(hour);
			SetMinute(minute);
			SetSecond(second);
		}
		else
			*this = GetSystemTime();
	}

	static bool IsValidTime(short hour, short minute, short second)
	{
		return ((hour >= 0) && (hour < 24)) && ((minute >= 0) && (minute < 60)) && ((second >= 0) && (second < 60));
	}

	bool IsValidTime()
	{
		return IsValidTime(_hour, _minute, _second);
	}

	static Time GetSystemTime()
	{
		time_t t = time(nullptr);
		tm* timePtr = localtime(&t);

		int hour = timePtr->tm_hour;
		int minute = timePtr->tm_min;
		int second = timePtr->tm_sec;

		return Time(hour, minute, second);

	}

	void SetToSystemTime()
	{
		*this = GetSystemTime();
	}
};

