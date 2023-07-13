#ifndef _DATE_TIME_H_
#define _DATE_TIME_H_

#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <time.h>
#include <iomanip>

class DateTime
{
public:
    DateTime( int year, int month, int day, int hour, int minute, int second, int millisecond):
        _year(year), _month(month), _day(day), _hour(hour), _minute(minute), _second(second), _millisecond(millisecond)
    { }
    DateTime(DateTime&) = default;
    DateTime(DateTime&&) = default;

    ~DateTime() { }

    int GetYear()        { return _year;}
    int GetMonth()       { return _month;}
    int GetDay()         { return _day;}
    int GetHour()        { return _hour;}
    int GetMinute()      { return _minute;}
    int GetSecond()      { return _second;}
    int GetMillisecond() { return _millisecond;}

    static DateTime Now()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
        struct tm currentLocalTime;
#if _WIN32
        localtime_s(&currentLocalTime, &currentTime);
#else
        //currentLocalTime = *localtime(&currentTime);
        localtime_r(&currentTime, &currentLocalTime);
#endif
        
        return DateTime(currentLocalTime.tm_year + 1900,
						currentLocalTime.tm_mon + 1,
						currentLocalTime.tm_mday,
						currentLocalTime.tm_hour,
						currentLocalTime.tm_min,
						currentLocalTime.tm_sec,
						ms.count() % 1000);

    }

    std::string ToString()
    {
		std::string time;
        try{
            std::ostringstream ss;
            ss << std::setfill('0') << std::setw(4) << this->GetYear()
                << "/" << std::setfill('0') << std::setw(2) << this->GetMonth()
                << "/" << std::setfill('0') << std::setw(2) << this->GetDay()
                << " ";
            ss << std::setfill('0') << std::setw(2) << this->GetHour()
               << ":" << std::setfill('0') << std::setw(2) << this->GetMinute()
               << ":" << std::setfill('0') << std::setw(2) << this->GetSecond()
               << "." << std::setfill('0') << std::setw(2) << this->GetMillisecond();
            time = ss.str();
        }
        catch(std::exception ex){
            std::cout << "In LogArgs occur ERROR: " << ex.what() << std::endl;
        }
        return time;
    }

private:
    int _year = 1990;
    int _month = 1;
    int _day = 1;
    int _hour = 0;
    int _minute = 0;
    int _second = 0;
    int _millisecond = 0;
};

#endif // _DATE_TIME_H_