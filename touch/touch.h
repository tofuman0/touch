#pragma once
#include <Windows.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#define VERSION "1.0"
#define TOUCH_OK                    0
#define TOUCH_ERROR                 0b10000000000000000000000000000000
#define TOUCH_ERROR_FILE_LOCKED     0b11000000000000000000000000000000
#define TOUCH_ERROR_FILE_NOTFOUND   0b10100000000000000000000000000000
#define TOUCH_ERROR_FILE_CREATE     0b10010000000000000000000000000000
#define TOUCH_ERROR_SYNTAX          0b10001000000000000000000000000000

enum class CHANGETYPE {
    BOTH,
    ACCESSED,
    MODIFIED
};
struct TOUCH_SETTINGS {
    std::vector<std::wstring> FileNames;
    std::wstring FileReference;
    SYSTEMTIME FileTime;
    bool CreateFiles;
    bool UpdateCreate;
    CHANGETYPE ChangeType;
    TOUCH_SETTINGS() :
        FileTime({ 0 }),
        CreateFiles(true),
        UpdateCreate(false),
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
template <typename T>
bool IsEmpty(T& in)
{
    int8_t* data = (int8_t*)&in;
    for (int32_t i = 0; i < sizeof(in); i++)
    {
        if (data[i] != 0) return false;
    }
    return true;
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
void TakeDays(uint32_t days, SYSTEMTIME& time)
{
    FILETIME ft;
    ULARGE_INTEGER temptime;
    time.wHour = 0;
    time.wMinute = 0;
    SystemTimeToFileTime(&time, &ft);
    temptime.HighPart = ft.dwHighDateTime;
    temptime.LowPart = ft.dwLowDateTime;
    temptime.QuadPart -= (uint64_t)(days * 86400ull * 1000ull * 1000ull * 10ull);
    ft.dwHighDateTime = temptime.HighPart;
    ft.dwLowDateTime = temptime.LowPart;
    FileTimeToSystemTime(&ft, &time);
}
void AddDays(uint32_t days, SYSTEMTIME& time)
{
    FILETIME ft;
    ULARGE_INTEGER temptime;
    SystemTimeToFileTime(&time, &ft);
    temptime.HighPart = ft.dwHighDateTime;
    temptime.LowPart = ft.dwLowDateTime;
    temptime.QuadPart += (uint64_t)(days * 86400ull * 1000ull * 1000ull * 10ull);
    ft.dwHighDateTime = temptime.HighPart;
    ft.dwLowDateTime = temptime.LowPart;
    FileTimeToSystemTime(&ft, &time);
}
std::wstring toLower(std::wstring& in)
{
    std::wstring lowercase;
    lowercase.resize(in.length());
    uint32_t i = 0;
    for (i = 0; i < in.length(); i++)
    {
        lowercase[i] = ::tolower(in[i]);
    }
    //lowercase[i] = 0;
    return lowercase;
}
std::vector<std::wstring> explode(std::wstring& in, const std::wstring& delimiter)
{
    std::vector<std::wstring> tokens(0);
    wchar_t* token = nullptr, *buffer = nullptr;
    token = wcstok_s((wchar_t*)in.c_str(), delimiter.c_str(), &buffer);
    while(token)
    {
        tokens.push_back(token);
        token = wcstok_s(NULL, delimiter.c_str(), &buffer);
    }

    return tokens;
}
bool GetDateFromString(std::wstring& stamp, SYSTEMTIME& time)
{   // Return true if error
    // TODO: Complete implementation
    if (stamp == L"today")
    {
        GetSystemTime(&time);
        time.wHour = 0;
        time.wMinute = 0;
        time.wSecond = 0;
        return false;
    }
    else if (stamp == L"now")
    {
        GetSystemTime(&time);
        return false;
    }
    else if (stamp == L"tomorrow")
    {
        GetSystemTime(&time);
        time.wHour = 0;
        time.wMinute = 0;
        time.wSecond = 0;
        AddDays(1, time);
        return false;
    }
    else if (stamp == L"yesterday")
    {
        GetSystemTime(&time);
        time.wHour = 0;
        time.wMinute = 0;
        time.wSecond = 0;
        TakeDays(1, time);
        return false;
    }
    else if (stamp.find(L" day ago") != std::wstring::npos)
    {
        uint32_t days = 0;
        if (swscanf_s(stamp.c_str(), L"%u day ago", &days) == 1)
        {
            GetSystemTime(&time);
            time.wHour = 0;
            time.wMinute = 0;
            time.wSecond = 0;
            TakeDays(days, time);
            return false;
        }
        return true;
    }
    else if (stamp.find(L" days ago") != std::wstring::npos)
    {
        uint32_t days = 0;
        if (swscanf_s(stamp.c_str(), L"%u days ago", &days) == 1)
        {
            GetSystemTime(&time);
            time.wHour = 0;
            time.wMinute = 0;
            time.wSecond = 0;
            TakeDays(days, time);
            return false;
        }
        return true;
    }
    else if (stamp.find(L" days time") != std::wstring::npos)
    {
        uint32_t days = 0;
        if (swscanf_s(stamp.c_str(), L"%u days time", &days) == 1)
        {
            GetSystemTime(&time);
            time.wHour = 0;
            time.wMinute = 0;
            time.wSecond = 0;
            AddDays(days, time);
            return false;
        }
        return true;
    }
    else if (stamp.find(L" ") != std::wstring::npos)
    {   // If space is in stamp it may contain a date
        std::vector<std::wstring> tokens = explode(stamp, L" ");
        if (tokens.size() == 3)
        {
            std::wstring MMM = toLower(tokens[1]);
            if(MMM == L"jan" || MMM == L"january")            time.wMonth = 1;
            else if (MMM == L"feb" || MMM == L"february")     time.wMonth = 2;
            else if (MMM == L"mar" || MMM == L"march")        time.wMonth = 3;
            else if (MMM == L"apr" || MMM == L"april")        time.wMonth = 4;
            else if (MMM == L"may" || MMM == L"may")          time.wMonth = 5;
            else if (MMM == L"jun" || MMM == L"june")         time.wMonth = 6;
            else if (MMM == L"jul" || MMM == L"july")         time.wMonth = 7;
            else if (MMM == L"aug" || MMM == L"august")       time.wMonth = 8;
            else if (MMM == L"sept" || MMM == L"september")   time.wMonth = 9;
            else if (MMM == L"oct" || MMM == L"october")      time.wMonth = 10;
            else if (MMM == L"nov" || MMM == L"november")     time.wMonth = 11;
            else if (MMM == L"dec" || MMM == L"december")     time.wMonth = 12;
            else return true;

            time.wHour = 0;
            time.wMinute = 0;
            time.wSecond = 0;

            time.wDay = std::stoi(tokens[0]);
            time.wYear = std::stoi(tokens[2]);

            FILETIME ft;
            SystemTimeToFileTime(&time, &ft);
            FileTimeToLocalFileTime(&ft, &ft);
            FileTimeToSystemTime(&ft, &time);
            
            return false;
        }
        else if (tokens.size() == 4)
        {
            std::wstring MMM = toLower(tokens[1]);
            if (MMM == L"jan" || MMM == L"january")            time.wMonth = 1;
            else if (MMM == L"feb" || MMM == L"february")     time.wMonth = 2;
            else if (MMM == L"mar" || MMM == L"march")        time.wMonth = 3;
            else if (MMM == L"apr" || MMM == L"april")        time.wMonth = 4;
            else if (MMM == L"may" || MMM == L"may")          time.wMonth = 5;
            else if (MMM == L"jun" || MMM == L"june")         time.wMonth = 6;
            else if (MMM == L"jul" || MMM == L"july")         time.wMonth = 7;
            else if (MMM == L"aug" || MMM == L"august")       time.wMonth = 8;
            else if (MMM == L"sept" || MMM == L"september")   time.wMonth = 9;
            else if (MMM == L"oct" || MMM == L"october")      time.wMonth = 10;
            else if (MMM == L"nov" || MMM == L"november")     time.wMonth = 11;
            else if (MMM == L"dec" || MMM == L"december")     time.wMonth = 12;
            else return true;

            std::vector<std::wstring> timeValues = explode(tokens[3], L":");

            if (timeValues.size() > 1 && timeValues.size() < 4)
            {
                time.wHour = std::stoi(timeValues[0]);
                time.wMinute = std::stoi(timeValues[1]);
                time.wSecond = (timeValues.size() == 2) ? 0 : std::stoi(timeValues[2]);
            }
            else
            {
                time.wHour = 0;
                time.wMinute = 0;
                time.wSecond = 0;
            }

            time.wDay = std::stoi(tokens[0]);
            time.wYear = std::stoi(tokens[2]);

            FILETIME ft;
            SystemTimeToFileTime(&time, &ft);
            FileTimeToLocalFileTime(&ft, &ft);
            FileTimeToSystemTime(&ft, &time);
            
            return false;
        }
        return true;
    }
    return true;
}
bool GetTimeFromString(std::wstring& stamp, SYSTEMTIME& time)
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
int32_t UpdateFileTime(std::wstring file, TOUCH_SETTINGS& settings, FILETIMES& FileTimes)
{
    int32_t ret = 0;
    HANDLE rFile(CreateFile(file.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, nullptr));
    
    // Correct for Local Time
    FILETIME cft, aft, mft;
    LocalFileTimeToFileTime(&FileTimes.CreateTime, &cft);
    LocalFileTimeToFileTime(&FileTimes.AccessTime, &aft);
    LocalFileTimeToFileTime(&FileTimes.ModifyTime, &mft);

    if (rFile != INVALID_HANDLE_VALUE)
    {   // File Exists so we need to update the time
        CloseHandle(rFile);
        HANDLE oFile(CreateFile(file.c_str(), GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr));
        if (oFile == INVALID_HANDLE_VALUE)
        {
            wchar_t buf[256];
            FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
            std::wcout << L"touch: failed to update file \'" << file.c_str() << L"\' Error: " << buf << std::endl;
            ret = TOUCH_ERROR_FILE_LOCKED;
        }
        
        SetFileTime(oFile, (settings.UpdateCreate == true) ? &cft : nullptr,
            (settings.ChangeType == CHANGETYPE::ACCESSED || settings.ChangeType == CHANGETYPE::BOTH) ? &aft : nullptr,
            (settings.ChangeType == CHANGETYPE::MODIFIED || settings.ChangeType == CHANGETYPE::BOTH) ? &mft : nullptr
        );
        CloseHandle(oFile);
    }
    else if (settings.CreateFiles == true)
    {   // File doesn't exist and we want to create files that don't
        CloseHandle(rFile);
        int32_t err = GetLastError();
        if (err == ERROR_FILE_NOT_FOUND)
        { // File doesn't exist
            HANDLE oFile(CreateFile(file.c_str(), GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr));
            if (oFile == INVALID_HANDLE_VALUE)
            {
                wchar_t buf[256];
                FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                    buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
                std::wcout << L"touch: failed to create file \'" << file.c_str() << L"\' Error: " << buf << std::endl;
                ret = TOUCH_ERROR_FILE_CREATE;
            }

            SetFileTime(oFile, (settings.UpdateCreate == true) ? &cft : nullptr,
                (settings.ChangeType == CHANGETYPE::ACCESSED || settings.ChangeType == CHANGETYPE::BOTH) ? &aft : nullptr,
                (settings.ChangeType == CHANGETYPE::MODIFIED || settings.ChangeType == CHANGETYPE::BOTH) ? &mft : nullptr
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
            ret = TOUCH_ERROR_FILE_LOCKED;
        }
    }
    return ret;
}
int32_t touch(TOUCH_SETTINGS& settings)
{
    int32_t ret = 0;
    FILETIMES FileTimes;
    if (settings.FileReference.length() > 0)
    {   // using file reference for time stamp so -d and -t will be overridden
        HANDLE fRef(CreateFile(settings.FileReference.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, nullptr));

        if(fRef != INVALID_HANDLE_VALUE)
        {
            GetFileTime(fRef, &FileTimes.CreateTime, &FileTimes.AccessTime, &FileTimes.ModifyTime);
        }
        else
        {
            std::cout << L"touch: failed to get attributes of \'" << settings.FileReference.c_str() << L"\': No such file or directory" << std::endl;
            return TOUCH_ERROR_FILE_NOTFOUND;
        }
        CloseHandle(fRef);
    }
    else
    {
        if (IsEmpty(settings.FileTime))
        {   // Use system time
            GetSystemTime(&settings.FileTime);
            SystemTimeToFileTime(&settings.FileTime, &FileTimes.CreateTime);
            SystemTimeToFileTime(&settings.FileTime, &FileTimes.ModifyTime);
            SystemTimeToFileTime(&settings.FileTime, &FileTimes.AccessTime);
            FileTimeToLocalFileTime(&FileTimes.CreateTime, &FileTimes.CreateTime);
            FileTimeToLocalFileTime(&FileTimes.ModifyTime, &FileTimes.ModifyTime);
            FileTimeToLocalFileTime(&FileTimes.AccessTime, &FileTimes.AccessTime);
        }
        else
        {
            SystemTimeToFileTime(&settings.FileTime, &FileTimes.CreateTime);
            SystemTimeToFileTime(&settings.FileTime, &FileTimes.ModifyTime);
            SystemTimeToFileTime(&settings.FileTime, &FileTimes.AccessTime);
        }
    }
    for (auto& file : settings.FileNames)
    {
        if (file.find(L'*') != std::wstring::npos || file.find(L'?') != std::wstring::npos)
        {   // File is wild card so need to loop through files that match the wildcard
            WIN32_FIND_DATA fd;
            HANDLE h = FindFirstFile(file.c_str(), &fd);

            if (h == INVALID_HANDLE_VALUE)
            {
                continue; // no files found
            }
            while (1)
            {
                if (wcscmp(fd.cFileName, L".") && wcscmp(fd.cFileName, L".."))
                    ret |= UpdateFileTime(fd.cFileName, settings, FileTimes);

                if (FindNextFile(h, &fd) == FALSE)
                    break;
            }
        }
        if (file.find(L'{') != std::wstring::npos && file.find(L'}') != std::wstring::npos)
        {   // Range of files specified i.e. {1..99} or {a..z}
            std::vector<std::wstring> FileNamesRange;
            uint32_t RangeCount = 0;
            std::wstring RangeString = file.substr(file.find(L'{')  + 1, file.find(L'}') - 1 - file.find(L'{'));
            std::vector<std::wstring> RangeValues = explode(RangeString, L"..");
            if (RangeValues.size() == 2)
            {
                if(std::isalpha(RangeValues[0].c_str()[0]))
                {
                    wchar_t First = RangeValues[0].c_str()[0];
                    wchar_t Second = RangeValues[1].c_str()[0];
                    if (First <= Second && ((::isupper(First) && ::isupper(Second)) || (::islower(First) && ::islower(Second))))
                    {
                        while (First <= Second)
                        {
                            std::wstring RangeFileName = file.substr(0, file.find(L'{')) + First++ + file.substr(file.find(L'}') + 1);
                            ret |= UpdateFileTime(RangeFileName, settings, FileTimes);
                        }
                    }
                    else
                        return TOUCH_ERROR_SYNTAX;
                }
                else if (std::isdigit(RangeValues[0].c_str()[0]))
                {
                    uint32_t First = std::stoi(RangeValues[0]);
                    uint32_t Second = std::stoi(RangeValues[1]);
                    if (First <= Second)
                    {
                        if (Second - First > 100)
                        {
                            std::wcout << L"touch: WARNING! Range is over 100. This may take some time. Press CTRL+C to cancel." << std::endl;
                        }
                        while (First <= Second)
                        {
                            std::wstring RangeFileName = file.substr(0, file.find(L'{')) + std::to_wstring(First++) + file.substr(file.find(L'}') + 1);
                            ret |= UpdateFileTime(RangeFileName, settings, FileTimes);
                        }
                    }
                    else
                        return TOUCH_ERROR_SYNTAX;
                }
                else
                    return TOUCH_ERROR_SYNTAX;
            }
        }
        else
        {
            ret |= UpdateFileTime(file, settings, FileTimes);
        }
    }
    return ret;
}