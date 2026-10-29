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

	void InvalidTimeFallBack()
	{
		if (!IsValidTime(*this))
		{
			cout << "INVALID DATE WARNING\nDATE HAS BEEN SET TO TODAY\'S DATE\n";
			*this = GetSystemTime();
		}
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
			InvalidTimeFallBack();
	}

	static bool IsValidTime(short hour, short minute, short second)
	{
		return ((hour >= 0) && (hour < 24)) && ((minute >= 0) && (minute < 60)) && ((second >= 0) && (second < 60));
	}

	static bool IsValidTime(const Time& time)
	{
		return IsValidTime(time.Hour(), time.Minute(), time.Second());
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

	static Time StringToTime(const string& line, const string delimiter = ":")
	{
		vector<string> timeElements = RichString::Split(line, delimiter);

		short hour = stoi(timeElements[0]);
		short minute = stoi(timeElements[1]);
		short second = stoi(timeElements[2]);

		return Time(hour, minute, second);
	}

	static string TimeToString(const Time &time, const string &delimiter = ":")
	{
		return to_string(time.Hour()) + delimiter +
			to_string(time.Minute()) + delimiter +
			to_string(time.Second());
	}

	string TimeToString(const string& delimiter = ":") const
	{
		return TimeToString(*this, delimiter);
	}

	void SetToSystemTime()
	{
		*this = GetSystemTime();
	}

};

