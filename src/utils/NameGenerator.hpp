#pragma once
#include <string>
#include <map>
#include <cstdio>
#include <mutex>


class NameGenerator 
{
public:
    static std::string generate(const std::string& type) 
    {
        static std::map<std::string, int> counters;
        int count = ++counters[type];

        char buffer[32];
        sprintf_s(buffer, "%s%03d", type.c_str(), count);

        static std::mutex mtx;
        std::lock_guard<std::mutex> lock(mtx);
        return std::string(buffer);
    }
};
