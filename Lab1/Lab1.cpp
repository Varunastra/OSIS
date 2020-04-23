#include <Windows.h>
#include "shlwapi.h"
#include <tchar.h>
#include <string>
#include <tdh.h>
#include <iostream>

using namespace std;

string ext;

LPCTSTR toStr(string text)
{
    wchar_t wtext[40];
    mbstowcs(wtext, text.c_str(), text.length());
    LPCTSTR ptr = wtext;
    return ptr;
}

char *concat(const char *str, const char *str2)
{
    char destination[100];
    strcpy(destination, str);
    strcpy(destination, str2);
    return destination;
}

void AppendToFile(string filepath, string result)
{
    HANDLE hFile;
    HANDLE hAppend;
    DWORD dwPos;
    char *stringToWrite = concat(filepath.c_str(), "\n");

    DWORD dwBytesToWrite = strlen(stringToWrite);
    DWORD dwBytesWritten = 0;

    char *fullname = concat("D:\\merged", result.c_str());

    hAppend = CreateFile(toStr(fullname),
                         FILE_APPEND_DATA,
                         FILE_SHARE_READ,
                         NULL,
                         OPEN_ALWAYS,
                         FILE_ATTRIBUTE_NORMAL,
                         NULL);

    if (hAppend == INVALID_HANDLE_VALUE)
    {
        printf("Could not open second file");
        return;
    }

    dwPos = SetFilePointer(hAppend, 0, NULL, FILE_END);
    LockFile(hAppend, dwPos, 0, dwBytesToWrite, 0);
    WriteFile(hAppend, stringToWrite, dwBytesToWrite, &dwBytesWritten, NULL);
    UnlockFile(hAppend, dwPos, 0, dwBytesToWrite, 0);

    CloseHandle(hAppend);
}

void FindAndAppend(LPCTSTR lpFolder, string lpFilePattern)
{
    TCHAR szFullPattern[MAX_PATH];
    WIN32_FIND_DATA FindFileData;
    HANDLE hFindFile;
    LPCTSTR pattern = toStr(lpFilePattern);
    PathCombine(szFullPattern, lpFolder, _T("*"));
    hFindFile = FindFirstFile(szFullPattern, &FindFileData);
    if (hFindFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if ((FindFileData.dwFileAttributes | FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY &&
                (FindFileData.cFileName[0] != '.'))
            {
                PathCombine(szFullPattern, lpFolder, FindFileData.cFileName);
                // _tprintf("Found %s %s %s \n", _T("Subdirectory" ), szFullPattern, FindFileData.cFileName);
                FindAndAppend(szFullPattern, lpFilePattern);
            }
        } while (FindNextFile(hFindFile, &FindFileData));
        FindClose(hFindFile);
    }

    PathCombine(szFullPattern, lpFolder, pattern);
    hFindFile = FindFirstFile(szFullPattern, &FindFileData);
    if (hFindFile != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                PathCombine(szFullPattern, lpFolder, FindFileData.cFileName);
                _tprintf_s("%s\n", szFullPattern);
                // char ch[260];
                // char DefChar = '  ' ;
                wstring s = "";
                s.insert(FindFileData.cFileName);
                // WideCharToMultiByte(CP_ACP, 0, wc, -1, ch, 260, &DefChar, NULL);
                // string text(ch);
                AppendToFile(s, ext);
            }
        } while (FindNextFile(hFindFile, &FindFileData));
        FindClose(hFindFile);
    }
}

int main()
{
    cin >> ext;
    FindAndAppend(_T("D:\\Learning"), ext);
}
