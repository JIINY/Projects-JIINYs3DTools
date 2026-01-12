#pragma once
#include <string>
#include <filesystem>


#define utf8(text) reinterpret_cast<const char*>(u8##text)

std::string toUTF8(const std::string& str) 
{
    std::filesystem::path path(str);
    auto u8str = path.u8string();

    return std::string(reinterpret_cast<const char*>(u8str.c_str()));
}
