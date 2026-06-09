#include "FontUtils.hpp"
#include <string>
#include <filesystem>
#include <Windows.h>
using namespace std;


string toUTF8(const string& str)
{
    filesystem::path path(str);
    auto u8str = path.u8string();

    return string(reinterpret_cast<const char*>(u8str.c_str()));
}

string convertWstrToStr(const wstring& w)
{
    if (w.empty()) { return {}; }

    int size = WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), nullptr, 0, nullptr, nullptr);
    string s(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), s.data(), size, nullptr, nullptr);

    return s;
}

wstring convertStrToWstr(const string& s)
{
    if (s.empty()) { return {}; }

    int size = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), nullptr, 0);
    wstring w(size, 0);
    MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), w.data(), size);
    return w;
}
