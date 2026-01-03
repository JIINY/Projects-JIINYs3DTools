#pragma once
#include <string>
#include <fstream>
#include <filesystem>
#include <mutex>
#include <cstdarg>
#include <type_traits>

inline const std::wstring LOG_PATH = L"D:/GitMine/Projects/JIINYs3DTools/DebugLog.txt"; //폴더 필수
inline const std::filesystem::path LOG_FOLDER = std::filesystem::path(LOG_PATH).parent_path();
inline std::mutex logMutex; //멀티스레드 보호


void DebugLogA(const char* format, va_list args, std::ofstream& logFileA);
void DebugLogW(const wchar_t* format, va_list args, std::wofstream& logFileW);

bool setupPathFolder(const std::wstring& path);
std::tm getTimeStamp();
std::string getTimeStampA();
std::wstring getTimeStampW();


template<typename TextFormat>
void DebugLog(TextFormat format, ...)
{
    std::lock_guard<std::mutex> lock(logMutex);

    if (!setupPathFolder(LOG_FOLDER)) return;

    va_list args;
    va_start(args, format);

    if constexpr (std::is_same_v<std::decay_t<TextFormat>, const char*>) 
    {
        std::ofstream logFileA(LOG_PATH, std::ios::app);
        if (logFileA.is_open())
            DebugLogA(format, args, logFileA);
    }
    else if constexpr (std::is_same_v<std::decay_t<TextFormat>, const wchar_t*>)
    {
        std::wofstream logFileW(LOG_PATH, std::ios::app);
        if (logFileW.is_open())
            DebugLogW(format, args, logFileW);
    }
    else
    {
        static_assert(std::is_same_v<TextFormat, void>, "DebugLog Format: char*, wchar_t*");
    }

    va_end(args);
}
