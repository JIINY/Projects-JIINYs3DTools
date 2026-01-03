#include "DebugLog.hpp"
#include <fstream>
#include <filesystem>
#include <mutex>
#include <string>
#include <ctime>
#include <iomanip> //시간서식
#include <cstdio>
#include <cstdarg>
#include <type_traits>
using namespace std;


bool setupPathFolder(const wstring& folderPath) 
{
    try
    {
        std::filesystem::create_directories(std::filesystem::path(folderPath));
        return true;
    }
    catch (...)
    {
        return false; //디렉토리 생성 실패하면 중단
    }
}

tm getTimeStamp() 
{
    time_t now = time(nullptr);
    tm localTime;
    localtime_s(&localTime, &now);
    return localTime;
}

string getTimeStampA() 
{
    tm localTime = getTimeStamp();
    char timeBuffer[32];
    strftime(timeBuffer, sizeof(timeBuffer), "%Y%m%d_%H%M%S ", &localTime);
    return string(timeBuffer);
}

wstring getTimeStampW() 
{
    tm localTime = getTimeStamp();
    wchar_t timeBuffer[32];
    wcsftime(timeBuffer, sizeof(timeBuffer) / sizeof(wchar_t), L"%Y%m%d_%H%M%S ", &localTime);
    return wstring(timeBuffer);
}

void DebugLogA(const char* format, va_list args, ofstream& logFileA)
{
    char buffer[1024]; // 가변 인자 처리
    vsnprintf(buffer, sizeof(buffer), format, args);
    logFileA << getTimeStampA() << buffer << endl;
}

void DebugLogW(const wchar_t* format, va_list args, wofstream& logFileW) 
{
    wchar_t buffer[1024];
    vswprintf_s(buffer, sizeof(buffer) / sizeof(wchar_t), format, args);
    logFileW << getTimeStampW() << buffer << endl;
}
