#include "FontUtils.hpp"
#include <string>
#include <filesystem>
using namespace std;

string toUTF8(const string& str)
{
    filesystem::path path(str);
    auto u8str = path.u8string();

    return string(reinterpret_cast<const char*>(u8str.c_str()));
}
