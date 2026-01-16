#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>

#include "compiler.h"

int main (int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <file.ggh>\n", argv[0]);
        return -1;
    }
    if (strlen(argv[1]) == 0 || argv[1] == NULL)
    {
        printf("Improper File Name\n");
        return -1;
    }
    
    // Validating And Opening File
    char filepath[BUFFER_LENGTH];
    memset(filepath, 0, sizeof(filepath));
    strncpy(filepath, argv[1], sizeof(filepath) - 1);
    filepath[sizeof(filepath) - 1] = '\0';
    if (checkFile(argv[1], "ggh") == ERROR)
    {
        printf("Not A .ggh File\n");
        return -1;
    }
    int fileFD = openFile(filepath);
    if (fileFD == ERROR)
    {
        printf("Failed To Open File\n");
        return -1;
    }
    
    // Get File Pointer
    FILE *fp = fdopen(fileFD, "r");
    if (fp == NULL)
    {
        perror("fdopen");
        printf("Failed To Open File Stream\n");
        close(fileFD);
        return -1;
    }
    
    // Temporary Storage For A Single Line Of Code
    char content[BUFFER_LENGTH];
    memset(content, 0, sizeof(content));
    // Struct To Store VARIABLE_ARRAY_LENGTH Variables
    struct Variable vars[VARIABLE_ARRAY_LENGTH];
    memset(&vars, 0, sizeof(vars));
    // Counter Of Numbers Of Variables In Array
    int varsCount = 0;
    
    // Stores Only 100 Lines Of Code
    char codeLines[100][BUFFER_LENGTH];
    memset(codeLines, 0, sizeof(codeLines));
    // Counter
    int currentCodeLine = 0;
    // Get Code By Line And Copy That To codeLines
    while (fgets(content, sizeof(content), fp))
    {
        if (currentCodeLine >= 0 && currentCodeLine < (sizeof(codeLines)/sizeof(codeLines[0]) - 1))
        {
            strncpy(codeLines[currentCodeLine], content, sizeof(codeLines[currentCodeLine]) - 1);
            codeLines[currentCodeLine][sizeof(codeLines[currentCodeLine]) - 1] = '\0';
            currentCodeLine++;
        }
        else if (currentCodeLine == (sizeof(codeLines)/sizeof(codeLines[0]) - 1))
        {
            // Set 99 char * To 0 So That It Is Our Full Stop During Execution
            memset(codeLines[currentCodeLine], 0, sizeof(codeLines[currentCodeLine]));
            break;
        }
        else
        {
            printf("Code Buffer Size Reached - %lu\n", sizeof(codeLines)/sizeof(codeLines[0]));
            break;
        }
    }
    
    // Error Flags
    struct ErrorFlags error_flagger = {0};
    
    // Flag For If Statements
    int IF_FLAG = 1;
    // Remembers Temporary Values For when
    char tempValue[BUFFER_LENGTH];
    memset(tempValue, 0, sizeof(tempValue));
    // Remembers Temporary Indexes For when Index
    int tempIndex = 0;
    // Program Counter
    int execCounter = 0;
    while (execCounter >= 0 && execCounter < sizeof(codeLines)/sizeof(codeLines[0]) && codeLines[execCounter][0] != 0 && error_flagger.NO_ERRORS == 0)
    {
        // Buffer To Store A Temporary Copy Of Current Line Of Code, Can Be Mutated
        char lineCopy[BUFFER_LENGTH];
        memset(lineCopy, 0, sizeof(lineCopy));
        strncpy(lineCopy, codeLines[execCounter], sizeof(lineCopy) - 1);
        lineCopy[sizeof(lineCopy) - 1] = '\0';
        executeLine(lineCopy, vars, &varsCount, &execCounter, &tempIndex, tempValue, &IF_FLAG, &error_flagger);
        execCounter++;
    }
    
    // Show Errors
    if (error_flagger.NO_ERRORS == 1)
    {
        if (error_flagger.VARIABLE_ARRAY_FULL == 1)
        {
            printf("Line: %d\nError: Variable Array Full\n", execCounter);
            printf("Content: %s\n", codeLines[execCounter - 1]);
        }
        else if (error_flagger.OUT_OF_BOUNDS == 1)
        {
            printf("Line: %d\nError: Out Of Bounds\n", execCounter);
            printf("Content: %s\n", codeLines[execCounter - 1]);
        }
        else if (error_flagger.DIVISION_BY_ZERO == 1)
        {
            printf("Line: %d\nError: Division By Zero\n", execCounter);
            printf("Content: %s\n", codeLines[execCounter - 1]);
        }
        else if (error_flagger.BAD_COMMAND == 1)
        {
            printf("Line: %d\nError: Bad Command\n", execCounter);
            printf("Content: %s\n", codeLines[execCounter - 1]);
        }
        else if (error_flagger.ARGUMENT_ERROR == 1)
        {
            printf("Line: %d\nError: Argument Error\n", execCounter);
            printf("Content: %s\n", codeLines[execCounter - 1]);
        }
    }
        
    fclose(fp);
    
    return 0;
}
