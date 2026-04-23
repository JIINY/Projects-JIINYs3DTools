#pragma once
#include <string>
#include <map>
#include <cstdio>


class NameGenerator 
{
public:
    static std::string generate(const std::string& type) 
    {
        static std::map<std::string, int> counters;
        int count = ++counters[type];

        char buffer[32];
        sprintf_s(buffer, "%s%03d", type.c_str(), count);

        return std::string(buffer);
    }
};
