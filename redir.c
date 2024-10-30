
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void splitter(char** arg, const char* command) 
{
    char* context = NULL;
    char* commandCopy = _strdup(command);

    int index = 0;
    char* token = strtok_s(commandCopy, " ", &context);
    while (token != NULL && index < 63) 
    {
        arg[index++] = _strdup(token);
        token = strtok_s(NULL, " ", &context);
    }
    arg[index] = NULL;
    free(commandCopy);
}

int main(int argc, char* argv[])
{
    char* command = argv[2];
    char* out = argv[3];
    char* command_args[64];
    char* input = argv[1];
    
    
    splitter(command_args, command);
    char commandLine[1024] = "";
    strcat_s(commandLine, 1024, command_args[0]);
    for (int i = 1; command_args[i] != NULL;   i++) 
    {
        strcat_s(commandLine, 1024,   " ");
        if (strchr(command_args[i],  ' ') != NULL)
        {
            strcat_s(commandLine , 1024, "\"");
            strcat_s(commandLine,   1024,  command_args[i]);
            strcat_s(commandLine, 1024, "\"");
        }
        else 
        {
            strcat_s(commandLine , 1024,   command_args[i]);
        }
    }
    if (strcmp(input, "-")   != 0) {
        strcat_s(commandLine  , 1024, " ");
        strcat_s(commandLine, 1024, input);
    }
    STARTUPINFOA startUpInfo;
    PROCESS_INFORMATION processInfo;
    HANDLE handleIn =  NULL;
    HANDLE handleOut =    NULL;
    ZeroMemory(&startUpInfo, sizeof(startUpInfo));
    startUpInfo.cb = sizeof(startUpInfo);
    startUpInfo.dwFlags |= STARTF_USESTDHANDLES;

    if (strcmp(input, "-") != 0) 
    {
        handleIn = CreateFileA(input, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        startUpInfo.hStdInput = handleIn;
        if (handleIn != NULL && handleIn != INVALID_HANDLE_VALUE) {
            SetHandleInformation(handleIn, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
        }
    }
    else 
    {
        startUpInfo.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    }
    if (strcmp(out, "-") != 0) {
        handleOut = CreateFileA(out, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        startUpInfo.hStdOutput = handleOut;

        if (handleOut != NULL && handleOut != INVALID_HANDLE_VALUE) {
            SetHandleInformation(handleOut, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT);
        }
    }
    else
    {
        startUpInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    startUpInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);
    BOOL success = CreateProcessA(NULL, commandLine, NULL, NULL, TRUE, 0, NULL, NULL, &startUpInfo, &processInfo);
    WaitForSingleObject(processInfo.hProcess, INFINITE);


    CloseHandle(processInfo.hProcess);
    
    
    
    CloseHandle(processInfo.hThread);

    if (handleIn != NULL && handleIn != INVALID_HANDLE_VALUE) CloseHandle(handleIn);


    if (handleOut != NULL && handleOut != INVALID_HANDLE_VALUE) CloseHandle(handleOut);
    for (int i = 0; command_args[i] != NULL; i++) 
    {
        free(command_args[i]);
    }
    return 0;
}