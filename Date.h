#pragma warning(disable : 4996)
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <iomanip>
#include "RichString.h"

using namespace std;

class Date
{
private:
    enum enDayOfWeek {
        Sunday = 0,
        Monday = 1,
        Tuesday = 2,
        Wednesday = 3,
        Thursday = 4,
        Friday = 5,
        Saturday = 6
    };

    enum enDateCompare { Before = -1, Equal = 0, After = 1 };

    int _dayNumber = 0;
    int _month = 0;
    int _year = 0;

    static void PrintCalendarHeader(short monthOrder)
    {
        cout << string(17, '_') << MonthShortName(monthOrder) << string(17, '_') << "\n\n";

        for (int i = 0; i < 7; i++)
        {
            cout << setw(5) << DayShortName(i);
        }
        cout << "\n";
    }

    static int CalculateDayDiffLogic(const Date& lowerDate, const Date& higherDate)
    {
        int DaysSummation = 0;

        if (lowerDate.Year() == higherDate.Year())
            return NumberOfDaysFromYearBeginning(higherDate) - NumberOfDaysFromYearBeginning(lowerDate);
        else
        {
            int numDaysFromBeginning = NumberOfDaysFromYearBeginning(lowerDate);

            DaysSummation += (IsLeapYear(lowerDate.Year()) ? 366 - numDaysFromBeginning : 365 - numDaysFromBeginning);

            for (int i = lowerDate.Year() + 1; i <= higherDate.Year(); i++)
            {
                if (i != higherDate.Year())
                    DaysSummation += (IsLeapYear(i) ? 366 : 365);
                else
                    DaysSummation += NumberOfDaysFromYearBeginning(higherDate);
            }
        }

        return DaysSummation;
    }

    void SetYear(short newYear) {
        _year = newYear;
    }

    void SetDay(short day) {
        _dayNumber = day;
    }

    void SetMonth(short month) {
        _month = month;
    }

    void InvalidDateFallBack()
    {
        if (!IsValidDate(*this))
        {
            cout << "INVALID DATE WARNING\nDATE HAS BEEN SET TO TODAY\'S DATE\n";
            *this = GetSystemDate();
        }
    }

public:

    Date()
    {
        *this = GetSystemDate();
    }

    Date(short day, short month, short year)
    {
        SetDay(day);
        SetMonth(month);
        SetYear(year);
        InvalidDateFallBack();
    }

    Date(string rawDateString, string delimiter = "/")
    {
        *this = StringToDate(rawDateString, delimiter);
        InvalidDateFallBack();
    }

    Date(int DateDayOrder, int year)
    {
        *this = DateFromDayOfYear(DateDayOrder, year);
        InvalidDateFallBack();
    }

    short Day() const {
        return _dayNumber;
    }

    short Month() const {
        return _month;
    }

    int Year() const {
        return _year;
    }

    static void Print(const Date& date)
    {
        cout << DateToString(date);
    }

    void Print()
    {
        Print(*this);
    }

    static Date GetSystemDate()
    {
        time_t t = time(nullptr);

        tm* gmtm = gmtime(&t);

        return Date::Date(gmtm->tm_mday, gmtm->tm_mon + 1, gmtm->tm_year + 1900);
    }

    static bool IsLeapYear(short Year)
    {
        return ((Year % 400 == 0) || ((Year % 4 == 0) && (Year % 100 != 0)));
    }

    bool IsLeapYear() const
    {
        return IsLeapYear(Year());
    }

    static short NumberOfDaysInYear(short Year)
    {
        return IsLeapYear(Year) ? 366 : 365;
    }

    short NumberOfDaysInYear() const
    {
        return NumberOfDaysInYear(Year());
    }

    static short NumberOfHoursInYear(short Year)
    {
        return NumberOfDaysInYear(Year) * 24;
    }

    short NumberOfHoursInYear() const
    {
        return NumberOfHoursInYear(Year());
    }

    static short NumberOfMinutesInYear(short Year)
    {
        return NumberOfHoursInYear(Year) * 60;
    }

    short NumberOfMinutesInYear() const
    {
        return NumberOfMinutesInYear(Year());
    }

    static short NumberOfSecondsInYear(short Year)
    {
        return NumberOfMinutesInYear(Year) * 60;
    }

    short NumberOfSecondsInYear() const
    {
        return NumberOfSecondsInYear(Year());
    }

    // This should probably be replaced by an exception handling
    static const short NumberOfDaysInMonth(short month, short year)
    {
        if (month < 1 || month>12)
            return 0;

        short monthDays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

        return (month == 2) ? (IsLeapYear(year) ? 29 : 28)
            : monthDays[month - 1];
    }

    short NumberOfDaysInMonth() const
    {
        return NumberOfDaysInMonth(Month(), Year());
    }

    static const enDayOfWeek CalculateDayOrder(short day, short month, short year)
    {
        int a = (14 - month) / 12;
        int y = year - a;
        int m = month + 12 * a - 2;

        return static_cast<enDayOfWeek>((day + y + (y / 4) - (y / 100) + (y / 400) + ((31 * m) / 12)) % 7);
    }

    static enDayOfWeek CalculateDayOrder(const Date& date)
    {
        return CalculateDayOrder(date.Day(), date.Month(), date.Day());
    }

    enDayOfWeek CalculateDayOrder() const
    {
        return CalculateDayOrder(Day(), Month(), Year());
    }

    static string DayShortName(short dayOrder)
    {
        static const array<string, 7> NamesOfDays{ "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
        return NamesOfDays.at(dayOrder);
    }

    string DayShortName() const
    {
        return DayShortName(CalculateDayOrder());
    }

    static string MonthShortName(short MonthOrder)
    {
        static const array<string, 12> NamesOfMonths
        {
            "Jan", "Feb", "Mar", "Apr", "May", "Jun",
            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
        };
        return NamesOfMonths.at(MonthOrder - 1);
    }

    string MonthShortName() const
    {
        return MonthShortName(Month());
    }

    static void PrintDayData(const Date& date)
    {
        int dayOrder = CalculateDayOrder(date.Day(), date.Month(), date.Year());

        cout << "\nDate      :" << date.Day() << "/" << date.Month() << "/" << date.Year();
        cout << "\nDay Order : " << dayOrder;
        cout << "\nDay Name  : " << DayShortName(dayOrder) << "\n";
    }

    void PrintDayData() const
    {
        PrintDayData(*this);
    }

    static void PrintMonthCalendar(short month, short year)
    {
        short numDays = NumberOfDaysInMonth(month, year);
        short dayOrder = CalculateDayOrder(1, month, year);

        PrintCalendarHeader(month);

        int i = 0;

        for (i = 0; i < dayOrder; i++)
        {
            cout << "     ";
        }

        for (int dayNumber = 1; dayNumber <= numDays; dayNumber++)
        {
            cout << setw(5) << dayNumber;

            if (++i == 7)
            {
                i = 0;
                cout << "\n";
            }
        }

        cout << "\n" << string(37, '_') << "\n";
    }

    void PrintMonthCalendar() const
    {
        PrintMonthCalendar(Month(), Year());
    }

    static void PrintYearCalendar(short year)
    {
        cout << "\n " << string(33, '_') << "\n\n";
        cout << string(10, ' ') << "" << "Calendar - " << year << "\n";
        cout << " " << string(33, '_') << "\n";

        for (int month = 1; month <= 12; month++)
        {
            PrintMonthCalendar(month, year);
        }
    }

    void PrintYearCalendar() const
    {
        PrintYearCalendar(Year());
    }

    static int NumberOfDaysFromYearBeginning(const Date& date)
    {
        int numDays = 0;

        for (short i = 0; i < date.Month(); i++)
            numDays += NumberOfDaysInMonth(i, date.Year());

        numDays += date.Day();

        return numDays;
    }

    int NumberOfDaysFromYearBeginning() const
    {
        return NumberOfDaysFromYearBeginning(*this);
    }

    static Date DateFromDayOfYear(short dayOfYear, short year)
    {
        short month = 1;
        short remainingDays = dayOfYear;

        for (int i = 1; i <= 12; i++)
        {
            short daysInCurrentMonth = NumberOfDaysInMonth(i, year);

            if (remainingDays > daysInCurrentMonth)
            {
                month++;
                remainingDays -= daysInCurrentMonth;
            }
            else
                break;
        }

        return Date(remainingDays, month, year);
    }

    Date DateFromDayOfYear(short dayOfYear) const
    {
        return DateFromDayOfYear(dayOfYear, Year());
    }

    static Date DateAfterNumberOfDays(int numDays, Date date)
    {
        int RemainingDays = numDays + NumberOfDaysFromYearBeginning(date);

        short MonthDays = 0;
        date.SetMonth(1);

        while (true)
        {
            MonthDays = NumberOfDaysInMonth(date.Month(), date.Year());

            if (RemainingDays > MonthDays)
            {
                RemainingDays -= MonthDays;
                date.SetMonth(date.Month() + 1);

                if (date.Month() > 12)
                {
                    date.SetMonth(1);
                    date.SetYear(date.Year() + 1);
                }
            }
            else
            {
                date.SetDay(RemainingDays);
                break;
            }
        }
        return date;
    }

    Date DateAfterNumberOfDays(int numDays) const
    {
        return DateAfterNumberOfDays(numDays, *this);
    }

    static enDateCompare CompareDates(const Date& date1, const Date& date2)
    {
        if (date1.Year() != date2.Year())
            return (date1.Year() < date2.Year()) ? Before : After;

        if (date1.Month() != date2.Month())
            return (date1.Month() < date2.Month()) ? Before : After;

        if (date1.Day() != date2.Day())
            return (date1.Day() < date2.Day()) ? Before : After;

        return Equal;
    }

    enDateCompare CompareDates(const Date& date2) const
    {
        return CompareDates(*this, date2);
    }

    static bool IsDate1BeforeDate2(const Date& date1, const Date& date2)
    {
        return CompareDates(date1, date2) == Before;
    }

    bool IsBefore(const Date& date2) const
    {
        return IsDate1BeforeDate2(*this, date2);
    }

    static bool IsDate1EqualDate2(const Date& date1, const Date& date2)
    {
        return CompareDates(date1, date2) == Equal;
    }

    bool IsEqual(const Date& date2) const
    {
        return IsDate1EqualDate2(*this, date2);
    }

    static bool IsDate1AfterDate2(const Date& date1, const Date& date2)
    {
        return CompareDates(date1, date2) == After;
    }

    bool IsAfter(const Date& date2) const
    {
        return IsDate1AfterDate2(*this, date2);
    }

    static bool IsLastDayInMonth(const Date& date)
    {
        return date.Day() == NumberOfDaysInMonth(date.Month(), date.Year());
    }

    bool IsLastDayInMonth() const
    {
        return IsLastDayInMonth(*this);
    }

    static bool IsLastMonthInYear(short monthNumber)
    {
        return monthNumber == 12;
    }

    bool IsLastMonthInYear() const
    {
        return IsLastMonthInYear(Month());
    }

    static Date AddOneDay(const Date& inputtedDate)
    {
        Date newData = inputtedDate;

        short lastDayInMonth = NumberOfDaysInMonth(inputtedDate.Month(), inputtedDate.Year());

        if (IsLastDayInMonth(inputtedDate))
        {
            if (IsLastMonthInYear(inputtedDate.Month()))
            {
                newData.SetYear(inputtedDate.Year() + 1);
                newData.SetDay(1);
                newData.SetMonth(1);
            }
            else
            {
                newData.SetDay(1);
                newData.SetMonth(1);
            }
        }
        else
            newData.SetDay(inputtedDate.Day() + 1);

        return newData;
    }

    Date AddOneDay() const
    {
        return AddOneDay(*this);
    }

    static Date IncreaseDateByXDays(int numDays, Date date)
    {
        return DateAfterNumberOfDays(numDays, date);
    }

    void IncreaseDateByXDays(int numDays)
    {
        *this = IncreaseDateByXDays(numDays, *this);
    }

    static Date IncreaseDateByOneWeek(Date date)
    {
        return DateAfterNumberOfDays(7, date);
    }

    void IncreaseDateByOneWeek()
    {
        *this = IncreaseDateByOneWeek(*this);
    }

    static Date IncreaseDateByXWeeks(int numWeeks, Date date)
    {
        return DateAfterNumberOfDays(7 * numWeeks, date);
    }

    void IncreaseDateByXWeeks(int numWeeks)
    {
        *this = IncreaseDateByXWeeks(numWeeks, *this);
    }

    static Date IncreaseDateByOneMonth(const Date& date)
    {
        short currentMonthDays = NumberOfDaysInMonth(date.Month(), date.Year());
        return DateAfterNumberOfDays(currentMonthDays, date);
    }

    void IncreaseDateByOneMonth()
    {
        *this = IncreaseDateByOneMonth(*this);
    }

    static Date IncreaseDateByXMonths(int numMonths, Date date)
    {
        // Note: No "Faster" variation existed for Months in the original code,
        // so this remains a standard iteration.
        for (int i = 0; i < numMonths; i++)
        {
            date = IncreaseDateByOneMonth(date);
        }
        return date;
    }

    void IncreaseDateByXMonths(int numMonths)
    {
        *this = IncreaseDateByXMonths(numMonths, *this);
    }

    static Date IncreaseDateByOneYear(Date date)
    {
        if (date.Month() == 2 && date.Day() == 29)
            date.SetDay(28);

        date.SetYear(date.Year() + 1);
        return date;
    }

    void IncreaseDateByOneYear()
    {
        *this = IncreaseDateByOneYear(*this);
    }

    static Date IncreaseDateByXYears(int numYears, Date date)
    {
        if (date.Month() == 2 && date.Day() == 29)
            date.SetDay(28);

        date.SetYear(date.Year() + numYears);
        return date;
    }

    void IncreaseDateByXYears(int numYears)
    {
        *this = IncreaseDateByXYears(numYears, *this);
    }

    static Date IncreaseDateByOneDecade(Date date)
    {
        if (date.Month() == 2 && date.Day() == 29)
            date.SetDay(28);

        date.SetYear(date.Year() + 10);
        return date;
    }

    void IncreaseDateByOneDecade()
    {
        *this = IncreaseDateByOneDecade(*this);
    }

    static Date IncreaseDateByXDecades(int numDecades, Date date)
    {
        if (date.Month() == 2 && date.Day() == 29)
            date.SetDay(28);

        date.SetYear(date.Year() + (10 * numDecades));
        return date;
    }

    void IncreaseDateByXDecades(int numDecades)
    {
        *this = IncreaseDateByXDecades(numDecades, *this);
    }

    static Date IncreaseDateByOneCentury(Date date)
    {
        if (date.Month() == 2 && date.Day() == 29)
            date.SetDay(28);

        date.SetYear(date.Year() + 100);
        return date;
    }

    void IncreaseDateByOneCentury()
    {
        *this = IncreaseDateByOneCentury(*this);
    }

    static Date IncreaseDateByOneMillennium(Date date)
    {
        if (date.Month() == 2 && date.Day() == 29)
            date.SetDay(28);

        date.SetYear(date.Year() + 1000);
        return date;
    }

    void IncreaseDateByOneMillennium()
    {
        *this = IncreaseDateByOneMillennium(*this);
    }

    static Date DecreaseDateByOneDay(Date date)
    {
        if (date.Day() == 1)
        {
            if (date.Month() == 1)
            {
                date.SetYear(date.Year() - 1);
                date.SetMonth(12);
                date.SetDay(31);
            }
            else
            {
                date.SetMonth(date.Month() - 1);
                date.SetDay(NumberOfDaysInMonth(date.Month(), date.Year()));
            }
        }
        else
        {
            date.SetDay(date.Day() - 1);
        }
        return date;
    }

    void DecreaseDateByOneDay()
    {
        *this = DecreaseDateByOneDay(*this);
    }

    static Date DecreaseDateByXDays(int numDays, Date date)
    {
        return DateBeforeNumberOfDays(numDays, date);
    }

    void DecreaseDateByXDays(int numDays)
    {
        *this = DecreaseDateByXDays(numDays, *this);
    }

    static Date DecreaseDateByOneWeek(Date date)
    {
        return DateBeforeNumberOfDays(7, date);
    }

    void DecreaseDateByOneWeek()
    {
        *this = DecreaseDateByOneWeek(*this);
    }

    static Date DecreaseDateByXWeeks(int numWeeks, Date date)
    {
        return DateBeforeNumberOfDays(7 * numWeeks, date);
    }

    void DecreaseDateByXWeeks(int numWeeks)
    {
        *this = DecreaseDateByXWeeks(numWeeks, *this);
    }

    static Date DecreaseDateByOneMonth(Date date)
    {
        short currentMonthDays = 0;

        if (date.Month() == 1)
            currentMonthDays = 31;
        else
            currentMonthDays = NumberOfDaysInMonth(date.Month(), date.Year());

        return DateBeforeNumberOfDays(currentMonthDays, date);
    }

    void DecreaseDateByOneMonth()
    {
        *this = DecreaseDateByOneMonth(*this);
    }

    static Date DecreaseDateByXMonths(int numMonths, Date date)
    {
        for (int i = 0; i < numMonths; i++)
            date = DecreaseDateByOneMonth(date);
        return date;
    }

    void DecreaseDateByXMonths(int numMonths)
    {
        *this = DecreaseDateByXMonths(numMonths, *this);
    }

    static Date DecreaseDateByOneYear(Date date)
    {
        if (date.Month() == 2 && date.Day() == 29)
            date.SetDay(28);

        date.SetYear(date.Year() - 1);
        return date;
    }

    void DecreaseDateByOneYear()
    {
        *this = DecreaseDateByOneYear(*this);
    }

    static Date DecreaseDateByXYears(int numYears, Date date)
    {
        if (date.Month() == 2 && date.Day() == 29)
            date.SetDay(28);

        date.SetYear(date.Year() - numYears);
        return date;
    }

    void DecreaseDateByXYears(int numYears)
    {
        *this = DecreaseDateByXYears(numYears, *this);
    }

    static Date DecreaseDateByOneDecade(Date date)
    {
        if (date.Month() == 2 && date.Day() == 29)
            date.SetDay(28);

        date.SetYear(date.Year() - 10);
        return date;
    }

    void DecreaseDateByOneDecade()
    {
        *this = DecreaseDateByOneDecade(*this);
    }

    static Date DecreaseDateByXDecades(int numDecades, Date date)
    {
        if (date.Month() == 2 && date.Day() == 29)
            date.SetDay(28);

        date.SetYear(date.Year() - (10 * numDecades));
        return date;
    }

    void DecreaseDateByXDecades(int numDecades)
    {
        *this = DecreaseDateByXDecades(numDecades, *this);
    }

    static Date DecreaseDateByOneCentury(Date date)
    {
        if (date.Month() == 2 && date.Day() == 29)
            date.SetDay(28);

        date.SetYear(date.Year() - 100);
        return date;
    }

    void DecreaseDateByOneCentury()
    {
        *this = DecreaseDateByOneCentury(*this);
    }

    static Date DecreaseDateByOneMillennium(Date date)
    {
        if (date.Month() == 2 && date.Day() == 29)
            date.SetDay(28);

        date.SetYear(date.Year() - 1000);
        return date;
    }

    void DecreaseDateByOneMillennium()
    {
        *this = DecreaseDateByOneMillennium(*this);
    }

    static int CalculateDayDifference(const Date& date1, const Date& date2, bool isEndDayIncluded = false)
    {
        int DaysSummation = 0;

        if (IsDate1EqualDate2(date1, date2))
            return isEndDayIncluded ? 1 : 0;

        if (IsDate1BeforeDate2(date1, date2))
            DaysSummation = CalculateDayDiffLogic(date1, date2);
        else
            DaysSummation = CalculateDayDiffLogic(date2, date1);

        return isEndDayIncluded ? ++DaysSummation : DaysSummation;
    }

    int CalculateDifferenceWithDate(const Date& date2, bool isEndDayIncluded = false) const
    {
        return CalculateDayDifference(*this, date2, isEndDayIncluded);
    }

    // The next 4 functions serve the system of work days Sun-->Thu with the rest representing the weekend days
    static bool IsEndOfWeek(const Date& date)
    {
        return CalculateDayOrder(date.Day(), date.Month(), date.Year()) == 6;
    }

    bool IsEndOfWeek() const
    {
        return IsEndOfWeek(*this);
    }

    static bool IsBusinessDay(const Date& date)
    {
        return !IsWeekend(date);
    }

    bool IsBusinessDay() const
    {
        return IsBusinessDay(*this);
    }

    static bool IsWeekend(const Date& date)
    {
        short dayIndex = CalculateDayOrder(date.Day(), date.Month(), date.Year());
        return (dayIndex == 5) || (dayIndex == 6);
    }

    bool IsWeekend() const
    {
        return IsWeekend(*this);
    }

    static short CalculateActualVacationDays(short startDay, short startMonth, short startYear,
        short endDay, short endMonth, short endYear)
    {
        return CalculateActualVacationDays(Date(startDay, startMonth, startYear),
            Date(endDay, endMonth, endYear));
    }

    static short CalculateActualVacationDays(const Date& initialDate, const Date& finalDate)
    {
        Date trackDate = initialDate;

        int dayDifference = initialDate.CalculateDifferenceWithDate(finalDate);
        int dayDifferenceAlternateCounter = dayDifference;

        int normalVacationCounter = 0;

        // Reach Saturday if possible
        while (trackDate.CalculateDayOrder() != 6)
        {
            if (trackDate.IsWeekend())
                normalVacationCounter++;

            trackDate.AddOneDay();
            dayDifferenceAlternateCounter--;
        }

        if (dayDifferenceAlternateCounter >= 28)
        {
            int numBlocksOf28Days = (dayDifferenceAlternateCounter / 28);

            normalVacationCounter += 8 * numBlocksOf28Days;

            trackDate.IncreaseDateByXDays(28 * numBlocksOf28Days);
            dayDifferenceAlternateCounter -= 28 * numBlocksOf28Days;
        }

        while (dayDifferenceAlternateCounter > 0)
        {
            if (trackDate.IsWeekend())
                normalVacationCounter++;

            trackDate.AddOneDay();
            dayDifferenceAlternateCounter--;
        }

        return dayDifference - normalVacationCounter - 1;
    }

    short CalculateActualVacationDays(const Date& finalDate) const
    {
        return CalculateActualVacationDays(*this, finalDate);
    }

    static short NumberOfDaysToYearEnd(const Date& date)
    {
        int numDays = 0;
        for (short i = 12; i > date.Month(); i--)
            numDays += NumberOfDaysInMonth(i, date.Year());

        numDays += NumberOfDaysInMonth(date.Month(), date.Year()) - date.Day();
        return numDays;
    }

    short NumberOfDaysToYearEnd() const
    {
        return NumberOfDaysToYearEnd(*this);
    }

    static Date DateBeforeNumberOfDays(int days, Date date)
    {
        int RemainingDays = days + NumberOfDaysToYearEnd(date);
        short MonthDays = 0;

        date.SetMonth(12);
        date.SetDay(31);

        while (true)
        {
            MonthDays = NumberOfDaysInMonth(date.Month(), date.Year());

            if (RemainingDays > MonthDays)
            {
                RemainingDays -= MonthDays;

                if (date.Day() != NumberOfDaysInMonth(date.Month(), date.Year()))
                    date.SetMonth(date.Month() - 1);
                else
                {
                    date.SetMonth(date.Month() - 1);
                    date.SetDay(NumberOfDaysInMonth(date.Month(), date.Year()));
                }

                if (date.Month() < 1)
                {
                    date.SetMonth(12);
                    date.SetYear(date.Year() - 1);
                }
            }
            else
            {
                date.SetDay(NumberOfDaysInMonth(date.Month(), date.Year()));
                for (int i = 0; i < RemainingDays; i++)
                    date = DecreaseDateByOneDay(date);

                return date;
            }
        }
    }

    Date DateBeforeNumberOfDays(int days) const
    {
        return DateBeforeNumberOfDays(days, *this);
    }

    static short NumberOfDaysToEndOfWeek(const Date& date)
    {
        return 6 - CalculateDayOrder(date.Day(), date.Month(), date.Year());
    }

    short NumberOfDaysToEndOfWeek() const
    {
        return NumberOfDaysToEndOfWeek(*this);
    }

    static short NumberOfDaysToEndOfMonth(const Date& date)
    {
        return NumberOfDaysInMonth(date.Month(), date.Year()) - date.Day();
    }

    short NumberOfDaysToEndOfMonth() const
    {
        return NumberOfDaysToEndOfMonth(*this);
    }

    static Date CalculateVacationReturnDate(Date date, short vacationDays)
    {
        while (date.CalculateDayOrder() != Saturday)
        {
            if (date.IsBusinessDay())
            {
                vacationDays++;
            }
            date.DecreaseDateByOneDay();
        }

        while (vacationDays >= 20)
        {
            date = DateAfterNumberOfDays(28, date);
            vacationDays -= 20;
        }

        while (vacationDays > 0)
        {
            if (date.IsBusinessDay())
            {
                vacationDays--;
            }
            date.AddOneDay();
        }

        while (!date.IsBusinessDay())
        {
            date.AddOneDay();
        }

        return date;
    }

    Date CalculateVacationReturnDate(short vacationDays) const
    {
        return CalculateVacationReturnDate(*this, vacationDays);
    }

    static Date StringToDate(const string& line, string delimiter = "/")
    {
        vector<string> dateElements = RichString::Split(line, delimiter);

        short day = stoi(dateElements[0]);
        short month = stoi(dateElements[1]);
        short year = stoi(dateElements[2]);

        return Date(day, month, year);
    }

    static string DateToString(const Date& date, string delimiter = "/")
    {
        return to_string(date.Day()) + delimiter +
            to_string(date.Month()) + delimiter +
            to_string(date.Year());
    }

    string DateToString(string delimiter = "/") const
    {
        return DateToString(*this, delimiter);
    }

    static string FormatDate(const Date& Date, const string& DateFormat = "dd/mm/yyyy", const string& delimiter = "/")
    {
        string FormattedDateString = "";

        FormattedDateString = RichString::ReplaceWord(
            DateFormat,
            "dd",
            to_string(Date.Day()),
            delimiter
        );

        FormattedDateString = RichString::ReplaceWord(
            FormattedDateString,
            "mm",
            to_string(Date.Month()),
            delimiter
        );

        FormattedDateString = RichString::ReplaceWord(
            FormattedDateString,
            "yyyy",
            to_string(Date.Year()),
            delimiter
        );

        return FormattedDateString;
    }

    string FormatDate(const string& DateFormat = "dd/mm/yyyy", const string& delimiter = "/")
    {
        return FormatDate(*this, DateFormat, delimiter);
    }

    static bool IsValidDate(const Date& date)
    {
        return (date.Year() >= 0) && (date.Month() > 0 && date.Month() < 13) &&
            (date.Day() >= 1 && date.Day() <= NumberOfDaysInMonth(date.Month(), date.Year()));
    }

    bool IsValidDate()
    {
        return IsValidDate(*this);
    }
};
