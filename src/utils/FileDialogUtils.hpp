#pragma once
#include <string>


class FileDialogUtils 
{
public:
    static std::string OpenFile(const char* filter);
    static std::string SaveFile(const char* filter, const char* defaultName = "", const char* defExt = NULL, const char* initialDir = "");
};
