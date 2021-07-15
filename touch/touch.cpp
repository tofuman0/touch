#include "touch.h"

int32_t wmain(int32_t argc, wchar_t* argv[])
{
    int32_t validArgCount = 0;
    TOUCH_SETTINGS settings;

	if (argc == 1)
	{
        help();
        return 0;
	}
    for (int32_t i = 1; i < argc; i++)
    {
        std::wstring argument = argv[i];
        if (argument.substr(0, 1) == L"-" || argument.substr(0, 1) == L"/")
        {
            if (argument == L"-a" || argument == L"/a" || argument == L"--time=access")
            {
                if (settings.ChangeType == CHANGETYPE::MODIFIED)
                    settings.ChangeType = CHANGETYPE::BOTH;
                else
                    settings.ChangeType = CHANGETYPE::ACCESSED;
                validArgCount++;
            }
            if (argument == L"-am" || argument == L"/am" || argument == L"-ma" || argument == L"/ma")
            {
                settings.ChangeType = CHANGETYPE::BOTH;
                validArgCount++;
            }
            else if (argument == L"-c" || argument == L"/c" || argument == L"--no-create")
            {
                settings.CreateFiles = false;
                validArgCount++;
            }
            else if (argument.substr(0, 2) == L"-d" || argument.substr(0, 2) == L"/d" || argument.substr(0, 6) == L"--date")
            {
                if (argument == L"-d" || argument == L"/d" || argument == L"--date")
                {   // Date string is in the next argument
                    if (i != (argc - 1) && wcslen(argv[i + 1]) > 0)
                    {
                        // TODO: Convert string to FILETIME
                        std::wstring DateStamp = argv[i + 1];
                        SYSTEMTIME ft = { 0 };
                        if (GetDateTimeFromString(DateStamp, ft))
                        {
                            std::wcout << L"touch: Syntax error with date argument" << std::endl;
                            return -1;
                        }
                        settings.FileTime = ft;
                        settings.UpdateCreate = true;
                        i++;
                    }
                    else
                        continue;
                }
                else if ((argument.substr(0, 3) == L"-d=" || argument.substr(0, 3) == L"/d=") && argument.length() > 3)
                {   // Date string is in this argument
                    std::wstring DateStamp = argument.substr(3, argument.length() - 3);
                    SYSTEMTIME ft = { 0 };
                    if (GetDateTimeFromString(DateStamp, ft))
                    {
                        std::wcout << L"touch: Syntax error with date argument" << std::endl;
                        return -1;
                    }
                    settings.FileTime = ft;
                    settings.UpdateCreate = true;
                }
                else if (argument.substr(0, 7) == L"--date=" && argument.length() > 7)
                {   // Date string is in this argument
                    std::wstring DateStamp = argument.substr(7, argument.length() - 7);
                    SYSTEMTIME ft = { 0 };
                    if (GetDateTimeFromString(DateStamp, ft))
                    {
                        std::wcout << L"touch: Syntax error with date argument" << std::endl;
                        return -1;
                    }
                    settings.FileTime = ft;
                    settings.UpdateCreate = true;
                }
                else
                    continue;
                validArgCount++;
            }
            else if (argument == L"-m" || argument == L"/m" || argument == L"--time=modify")
            {
                if (settings.ChangeType == CHANGETYPE::ACCESSED)
                    settings.ChangeType = CHANGETYPE::BOTH;
                else
                    settings.ChangeType = CHANGETYPE::MODIFIED;
                validArgCount++;
            }
            else if (argument.substr(0, 2) == L"-r" || argument.substr(0, 2) == L"/r" || argument.substr(0, 11) == L"--reference")
            {
                if (argument == L"-r" || argument == L"/r" || argument == L"--reference")
                {   // Reference file string is in the next argument
                    if (i != (argc - 1) && wcslen(argv[i + 1]) > 0)
                    {
                        settings.FileReference = argv[i + 1];
                        i++;
                    }
                    else
                        continue;
                }
                else if ((argument.substr(0, 3) == L"-r=" || argument.substr(0, 3) == L"/r=") && argument.length() > 3)
                {   // Reference file string is in this argument
                    settings.FileReference = argument.substr(3, argument.length() - 3);
                }
                else if (argument.substr(0, 12) == L"--reference=" && argument.length() > 12)
                {   // Reference file string is in this argument
                    settings.FileReference = argument.substr(12, argument.length() - 12);
                }
                else
                    continue;
                validArgCount++;
            }
            else if (argument == L"-t" || argument == L"/t")
            {
                if (i != (argc - 1) && wcslen(argv[i + 1]) > 0)
                {
                    std::wstring TimeStamp = argv[i + 1];
                    SYSTEMTIME ft = { 0 };
                    if (GetDateTimeFromString(TimeStamp, ft))
                    {
                        std::wcout << L"touch: Syntax error with date argument" << std::endl;
                        return -1;
                    }
                    settings.FileTime = ft;
                    i++;
                    validArgCount++;
                }
            }
            else if (argument == L"-?" || argument == L"/?" || argument == L"--help")
            {
                help();
                return 0;
            }
            else if (argument == L"--version")
            {
                version();
                return 0;
            }
        }
        else
        {   // Arguments will be handled as files.
            settings.FileNames.push_back(argument);
        }
    }
    if (validArgCount == 0 && settings.FileNames.size() == 0)
        help();
    else if (settings.FileNames.size() == 0)
        help();
    else
        return touch(settings);
	return 0;
}