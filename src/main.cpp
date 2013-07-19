#include <stdio.h>
#include <iostream>
#include <Windows.h>

//------------------------------------------------------------------------------

int printUsage()
{
    std::cout << "ln for Windows, Version 1.0" << std::endl;
    std::cout << "usage:\n ln [-s] target link-name" << std::endl;
    return -1;
}

//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    /// Parse command line
    if(argc < 3 || argc > 4 )
    {
        return printUsage();
    }

    bool isSymbolic = false;
    if(argc == 4 )
    {
        std::string option(argv[1]);
        if( option.compare("-s")==0 || option.compare("/s")==0 )
        {
            isSymbolic = true;
        }
        else
        {
            return printUsage();
        }
    }

    int targetIdx   = isSymbolic?2:1;
    int linknameIdx = targetIdx + 1;

    const std::string& target(argv[targetIdx]);
    const std::string& link(argv[linknameIdx]);

    const DWORD SYMBLOC_LINK_FLAG_FILE      = 0x0;
    const DWORD SYMBLOC_LINK_FLAG_DIRECTORY = 0x1;
    BOOL  res;
    if(isSymbolic)
    {
        DWORD dwAttrs = GetFileAttributes(target.c_str());
        if (dwAttrs==INVALID_FILE_ATTRIBUTES)
        {
            std::cerr << "Invalid file attributes" << std::endl;
            return -1;
        }
        DWORD symbloc_link_flag = SYMBLOC_LINK_FLAG_FILE;
        if (dwAttrs & FILE_ATTRIBUTE_DIRECTORY)
        {
            symbloc_link_flag = SYMBLOC_LINK_FLAG_DIRECTORY;
        }
        res = CreateSymbolicLink( link.c_str(), target.c_str(), symbloc_link_flag);
    }
    else
    {
        res = CreateHardLink(link.c_str(), target.c_str(), NULL );
    }
    if(!res)
    {
        // Retrieves the last error message.
        DWORD   lastError = GetLastError();
        char    buffer[1024];
        FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, 0, lastError, 0, buffer, 1024, 0 );
        std::cerr << buffer << std::endl;
    }

    return 0;
}

//------------------------------------------------------------------------------
