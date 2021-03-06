# Touch
While I was missing "touch" when using windows I've created an implementation of touch for windows. Symbolic links options aren't implemented but other than that it is pretty complete.

## Usage
```Commandline
Usage: touch [OPTION]... FILE... 
Update the access and modification times of each FILE to the current time. 
        
A FILE argument that does not exist is created empty, unless -c
is supplied.
        
A FILE argument string of - is handled specially and causes touch to
change the times of the file associated with standard output.
        
Mandatory arguments to long options are mandatory for short options too.
  -a                       change only the access time
  -c, --no-create          do not create any files
  -d, --date=STRING        parse STRING and use it instead of current time
  -m                       change only the modification time
  -r, --reference=FILE     use this file's times instead of current time
  -t STAMP                 use [[CC]YY]MMDDhhmm[.ss] instead of current time
  --time=WORD              change the specified time :
                             WORD is access, atime, or use : equivalent to -a
                             WORD is modify or mtime : equivalent to -m
  -?, --help               display this help and exit
  --version                output version information and exit
        
Note that the -d and -t options accept different time-date formats.
```
