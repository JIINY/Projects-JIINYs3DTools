#include "FileDialogUtils.hpp"
#include <string>
#include <Windows.h>
#include <commdlg.h>
#include <filesystem>
using namespace std;

#define GLFW_EXPOSE_NATIVE_WIN32

string FileDialogUtils::OpenFile(const char* filter)
{
    OPENFILENAMEA file;
    char szFile[260] = { 0 };

    ZeroMemory(&file, sizeof(file));
    file.lStructSize = sizeof(file);
    file.hwndOwner = NULL; //메인 윈도우가 부모
    file.lpstrFile = szFile;
    file.nMaxFile = sizeof(szFile);
    file.lpstrFilter = filter;
    file.nFilterIndex = 1;
    file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileNameA(&file) == TRUE) { return string(file.lpstrFile); }
    return string();
}

string FileDialogUtils::SaveFile(const char* filter, const char* defaultName, const char* defExt, const char* initialDir)
{
    OPENFILENAMEA file;
    char szFile[260] = { 0 };
    string absInitialPath;

    if (defaultName && *defaultName) 
    {
        strncpy_s(szFile, defaultName, sizeof(szFile));
    }

    ZeroMemory(&file, sizeof(file));
    file.lStructSize = sizeof(file);
    file.hwndOwner = NULL;
    file.lpstrFile = szFile;
    file.nMaxFile = sizeof(szFile);
    file.lpstrFilter = filter;
    file.nFilterIndex = 1;
    file.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;
    file.lpstrDefExt = defExt;

    if (initialDir && *initialDir) 
    {
        try 
        {
            absInitialPath = filesystem::absolute(initialDir).string();
            replace(absInitialPath.begin(), absInitialPath.end(), '/', '\\');
            file.lpstrInitialDir = absInitialPath.c_str();
        }
        catch (...) {}
    }

    if (GetSaveFileNameA(&file)) { return string(file.lpstrFile); }
    return string();
}
