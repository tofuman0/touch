#pragma once
#include <Windows.h>
#include <stdint.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <chrono>

#define VERSION "1.0"

enum class CHANGETYPE {
    BOTH,
    ACCESSED,
    MODIFIED
};
struct TOUCH_SETTINGS {
    std::vector<std::wstring> FileNames;
    std::wstring FileReference;
    //FILETIME FileTime;
    SYSTEMTIME FileTime;
    bool CreateFiles;
    CHANGETYPE ChangeType;
    TOUCH_SETTINGS() :
        FileTime({ 0 }),
        CreateFiles(true),
        ChangeType(CHANGETYPE::BOTH)
    {}
};
struct FILETIMES
{
    FILETIME CreateTime;
    FILETIME ModifyTime;
    FILETIME AccessTime;
    FILETIMES() :
        CreateTime({0}),
        ModifyTime({0}),
        AccessTime({0})
    {}
};

void help()
{
    std::cout <<
        "Usage: touch [OPTION]... FILE... " << std::endl <<
        "Update the access and modification times of each FILE to the current time. " << std::endl <<
        std::endl <<
        "A FILE argument that does not exist is created empty, unless -c" << std::endl <<
        "is supplied." << std::endl <<
        std::endl <<
        "A FILE argument string of - is handled specially and causes touch to" << std::endl <<
        "change the times of the file associated with standard output." << std::endl <<
        std::endl <<
        "Mandatory arguments to long options are mandatory for short options too." << std::endl <<
        "  -a                       change only the access time" << std::endl <<
        "  -c, --no-create          do not create any files" << std::endl <<
        "  -d, --date=STRING        parse STRING and use it instead of current time" << std::endl <<
        "  -m                       change only the modification time" << std::endl <<
        "  -r, --reference=FILE     use this file's times instead of current time" << std::endl <<
        "  -t STAMP                 use [[CC]YY]MMDDhhmm[.ss] instead of current time" << std::endl <<
        "  --time=WORD              change the specified time :" << std::endl <<
        "                             WORD is access, atime, or use : equivalent to -a" << std::endl <<
        "                             WORD is modify or mtime : equivalent to -m" << std::endl <<
        "  -?, --help               display this help and exit" << std::endl <<
        "  --version                output version information and exit" << std::endl <<
        std::endl <<
        "Note that the -d and -t options accept different time-date formats." << std::endl << std::endl;
}
void version()
{
    std::cout <<
        "touch " << VERSION << std::endl <<
        "Windows port" << std::endl <<
        "Written by Danny Worth" <<
        std::endl;
}
void FileQuery(std::wstring& filename, FILETIMES& FileTimes)
{
    FILETIME ftc, ftm, fta;
    SYSTEMTIME stc, stm, sta;
    FileTimeToLocalFileTime(&FileTimes.CreateTime, &ftc);
    FileTimeToLocalFileTime(&FileTimes.ModifyTime, &ftm);
    FileTimeToLocalFileTime(&FileTimes.AccessTime, &fta);
    FileTimeToSystemTime(&ftc, &stc);
    FileTimeToSystemTime(&ftm, &stm);
    FileTimeToSystemTime(&fta, &sta);
    std::wcout << L"File: " << filename.c_str() << std::endl;
    std::wcout << L"Create Time: " << std::setfill(L'0') << std::setw(2) << stc.wDay << L"/" << std::setfill(L'0') << std::setw(2) << stc.wMonth << L"/" << stc.wYear << L" " << std::setfill(L'0') << std::setw(2) << stc.wHour << L":" << std::setfill(L'0') << std::setw(2) << stc.wMinute << L":" << std::setfill(L'0') << std::setw(2) << stc.wSecond << std::endl;
    std::wcout << L"Modify Time: " << std::setfill(L'0') << std::setw(2) << stm.wDay << L"/" << std::setfill(L'0') << std::setw(2) << stm.wMonth << L"/" << stm.wYear << L" " << std::setfill(L'0') << std::setw(2) << stm.wHour << L":" << std::setfill(L'0') << std::setw(2) << stm.wMinute << L":" << std::setfill(L'0') << std::setw(2) << stm.wSecond << std::endl;
    std::wcout << L"Access Time: " << std::setfill(L'0') << std::setw(2) << sta.wDay << L"/" << std::setfill(L'0') << std::setw(2) << sta.wMonth << L"/" << sta.wYear << L" " << std::setfill(L'0') << std::setw(2) << sta.wHour << L":" << std::setfill(L'0') << std::setw(2) << sta.wMinute << L":" << std::setfill(L'0') << std::setw(2) << sta.wSecond << std::endl;
    std::wcout << std::endl;
}
bool GetDateTimeFromString(std::wstring& stamp, SYSTEMTIME& time)
{   // Return true if error
    if (swscanf_s(stamp.c_str(), L"%04hd%02hd%02hd%02hd%02hd.%02hd", &time.wYear, &time.wMonth, &time.wDay, &time.wHour, &time.wMinute, &time.wSecond) == 6)
    {
        return false;
    }
    else if (swscanf_s(stamp.c_str(), L"%02hd%02hd%02hd%02hd%02hd.%02hd", &time.wYear, &time.wMonth, &time.wDay, &time.wHour, &time.wMinute, &time.wSecond) == 6)
    {
        time.wYear += 2000;
        return false;
    }
    else if (swscanf_s(stamp.c_str(), L"%04hd%02hd%02hd%02hd%02hd", &time.wYear, &time.wMonth, &time.wDay, &time.wHour, &time.wMinute) == 5)
    {
        return false;
    }
    else if (swscanf_s(stamp.c_str(), L"%02hd%02hd%02hd%02hd%02hd", &time.wYear, &time.wMonth, &time.wDay, &time.wHour, &time.wMinute) == 5)
    {
        time.wYear += 2000;
        return false;
    }
    ZeroMemory(&time, sizeof(SYSTEMTIME));
    return true;
}
int32_t touch(TOUCH_SETTINGS& settings)
{
    int32_t ret = 0;
    if (settings.FileReference.length() > 0)
    {   // using file reference for time stamp so -d and -t will be overridden
        FILETIMES FileTimes;
        HANDLE fRef(CreateFile(settings.FileReference.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, nullptr));

        if(fRef != INVALID_HANDLE_VALUE)
        {
            GetFileTime(fRef, &FileTimes.CreateTime, &FileTimes.AccessTime, &FileTimes.ModifyTime);
            FileQuery(settings.FileReference, FileTimes);
        }
        else
        {
            std::cout << L"touch: failed to get attributes of \'" << settings.FileReference.c_str() << L"\': No such file or directory" << std::endl;
            return -1;
        }
        CloseHandle(fRef);
    }
    for (auto& file : settings.FileNames)
    {
        FILETIMES FileTimes;
        HANDLE rFile(CreateFile(file.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, nullptr));
        if(rFile != INVALID_HANDLE_VALUE)
        {   // File Exists so we need to update the time
            // TODO: Update file time
            GetFileTime(rFile, &FileTimes.CreateTime, &FileTimes.AccessTime, &FileTimes.ModifyTime);
            FileQuery(file, FileTimes);
        }
        else if (settings.CreateFiles == true)
        {   // File doesn't exist and we want to create files that don't
            int32_t err = GetLastError();
            if (err == ERROR_FILE_NOT_FOUND)
            { // File doesn't exist
                HANDLE oFile(CreateFile(file.c_str(), GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr));
                if(oFile == INVALID_HANDLE_VALUE)
                {
                    wchar_t buf[256];
                    FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                        NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                        buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
                    std::wcout << L"touch: failed to create file \'" << file.c_str() << L"\' Error: " << buf << std::endl;
                    ret = -1;
                }
                FILETIME mt;
                FILETIME at;
                SystemTimeToFileTime(&settings.FileTime, &mt);
                SystemTimeToFileTime(&settings.FileTime, &at);

                SetFileTime(oFile, nullptr,
                    (settings.ChangeType == CHANGETYPE::ACCESSED || settings.ChangeType == CHANGETYPE::BOTH) ? &at : nullptr,
                    (settings.ChangeType == CHANGETYPE::MODIFIED || settings.ChangeType == CHANGETYPE::BOTH) ? &mt : nullptr
                );
                CloseHandle(oFile);
            }
            else
            { // File file exists but can't be updated
                wchar_t buf[256];
                FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
                std::wcout << L"touch: failed to update file \'" << file.c_str() << L"\' Error: " << buf << std::endl;
                ret = -1;
            }
        }
        CloseHandle(rFile);
    }
    return ret;
}