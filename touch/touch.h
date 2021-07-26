#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <stdint.h>
#include <iostream>
#include <iomanip>

constexpr auto TOUCH_OK                     = 0;
constexpr auto TOUCH_ERROR                  = 0b10000000000000000000000000000000;
constexpr auto TOUCH_ERROR_FILE_LOCKED      = 0b11000000000000000000000000000000;
constexpr auto TOUCH_ERROR_FILE_NOTFOUND    = 0b10100000000000000000000000000000;
constexpr auto TOUCH_ERROR_FILE_CREATE      = 0b10010000000000000000000000000000;
constexpr auto TOUCH_ERROR_SYNTAX           = 0b10001000000000000000000000000000;

class Touch
{
public:
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
            CreateTime({ 0 }),
            ModifyTime({ 0 }),
            AccessTime({ 0 })
        {}
    };

private:
    TOUCH_SETTINGS m_Settings;

public:
    Touch();
    CHANGETYPE ChangeType() { return m_Settings.ChangeType; }
    void ChangeType(CHANGETYPE ChangeType) { m_Settings.ChangeType = ChangeType; }
    bool CreateFiles() { return m_Settings.CreateFiles; }
    void CreateFiles(bool CreateFiles) { m_Settings.CreateFiles = CreateFiles; }
    SYSTEMTIME FileTime() { return m_Settings.FileTime; }
    void FileTime(SYSTEMTIME FileTime) { m_Settings.FileTime = FileTime; }
    bool UpdateCreate() { return m_Settings.UpdateCreate; }
    void UpdateCreate(bool UpdateCreate) { m_Settings.UpdateCreate = UpdateCreate; }
    std::wstring FileReference() { return m_Settings.FileReference; };
    void FileReference(std::wstring FileReference) { m_Settings.FileReference = FileReference; };
    void AddFile(std::wstring FileName) { m_Settings.FileNames.push_back(FileName); }
    int32_t FileCount() { return m_Settings.FileNames.size(); }
    bool GetDateFromString(std::wstring& stamp, SYSTEMTIME& time);
    bool GetTimeFromString(std::wstring& stamp, SYSTEMTIME& time);
    int32_t touch();

private:
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
    void FileQuery(std::wstring& filename, FILETIMES& FileTimes);
    void TakeDays(uint32_t days, SYSTEMTIME& time);
    void AddDays(uint32_t days, SYSTEMTIME& time);
    std::wstring toLower(std::wstring& in);
    std::vector<std::wstring> explode(std::wstring& in, const std::wstring& delimiter);
    int32_t UpdateFileTime(std::wstring file, FILETIMES& FileTimes);
};

