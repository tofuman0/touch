#pragma once
#include <Windows.h>
#include <stdint.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#define VERSION "1.0"

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
