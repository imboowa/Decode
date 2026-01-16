#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdbool.h>

#include "compiler.h"

int checkFile(char *fileName, char *realExtension)
{
    // Checking For "."
    if (strcspn(fileName, ".") == strlen(fileName))
    {
        return ERROR;
    }
    // Validate Filename
    strtok(fileName, ".");
    char *extension = strtok(NULL, ".");
    
    if (strlen(realExtension) != strlen(extension))
    {
        return ERROR;
    }
    if (strcmp(extension, realExtension) == 0)
    {
        return GOOD;
    }
    return ERROR;
}

int openFile(char *filepath)
{
    int fd = open(filepath, O_RDONLY);
    if (fd > -1)
    {
        return fd;
    }
    perror("open");
    return ERROR;
}

void mathOperations(char *operation, struct Variable *vars, int *varsCount, char *tempResult, char *tempVar1, char *tempVar2, struct ErrorFlags *error_flagger)
{
    // Temporary Buffer For Storage Of Answer
    char buffer[BUFFER_LENGTH];
    memset(buffer, 0, sizeof(buffer));
    
    char *var1 = tempVar1;
    char *var2 = tempVar2;
    // Bounds Checking
    if ((*varsCount) >= 0 && (*varsCount) < VARIABLE_ARRAY_LENGTH)
    {
        for (int i = 0; i < (*varsCount); i++)
        {
            // Check If var1 Is A Variable
            if (strcmp(vars[i].name, var1) == 0)
            {
                var1 = vars[i].value;
            }
            // Check If var2 Is A Variable
            if (strcmp(vars[i].name, var2) == 0)
            {
                var2 = vars[i].value;
            }
        }
    }
    else
    {
        //printf("Execution Error: Out Of Bounds\n");
        error_flagger->OUT_OF_BOUNDS = 1;
        error_flagger->NO_ERRORS = 1;
        return;
    }
    int result = 0;
    // Do Operation
    if (strcmp(operation, "add") == 0)
    {
        result = atoi(var1) + atoi(var2);
    }
    else if (strcmp(operation, "sub") == 0)
    {
        result = atoi(var1) - atoi(var2);
    }
    else if (strcmp(operation, "mul") == 0)
    {
        result = atoi(var1) * atoi(var2);
    }
    else if (strcmp(operation, "div") == 0)
    {
        // Is It Dividion By Zero
        if (atoi(var2) == 0)
        {
            //printf("Division Error: Cannot divide by zero\n");
            error_flagger->DIVISION_BY_ZERO = 1;
            error_flagger->NO_ERRORS = 1;
            return;
        }
        result = atoi(var1) / atoi(var2);
    }
    // snprintf Variable To A Char Buffer
    snprintf(buffer, sizeof(buffer), "%d", result);
    // Checking Bounds
    if ((*varsCount) >= 0 && (*varsCount) < VARIABLE_ARRAY_LENGTH)
    {
        // Check If Destination Is A Variable
        for (int i = 0; i < (*varsCount); i++)
        {
            if (strncmp(vars[i].name, tempResult, sizeof(vars[i].name)) == 0)
            {
                strncpy(vars[i].value, buffer, sizeof(vars[i].value) - 1);
                vars[i].value[sizeof(vars[i].value) - 1] = '\0';
                return;
            }
        }
    }
    else
    {
        //printf("Execution Error: Out Of Bounds\n");
        error_flagger->OUT_OF_BOUNDS = 1;
        error_flagger->NO_ERRORS = 1;
        return;
    }
    // Check If It Is Safe To Add A Variable
    if ((*varsCount) >= 0 && (*varsCount) < VARIABLE_ARRAY_LENGTH)
    {
        // Add The Variable
        strncpy(vars[(*varsCount)].name, tempResult, sizeof(vars[(*varsCount)].name) - 1);
        strncpy(vars[(*varsCount)].value, buffer, sizeof(vars[(*varsCount)].value) - 1);
        vars[(*varsCount)].name[sizeof(vars[(*varsCount)].name) - 1] = '\0';
        vars[(*varsCount)].value[sizeof(vars[(*varsCount)].value) - 1] = '\0';
        (*varsCount)++;
    }
    else
    {
        //printf("Varaible Error: Variable Array Full\n");
        error_flagger->VARIABLE_ARRAY_FULL = 1;
        error_flagger->NO_ERRORS = 1;
    }
}

void setVariable(struct Variable *vars, int *varsCount, char *tempArg1, char *tempArg2, struct ErrorFlags *error_flagger)
{
    // Bounds Checking
    if ((*varsCount) >= 0 && (*varsCount) < VARIABLE_ARRAY_LENGTH)
    {
        // Is This Variable Already Existing
        for (int i = 0; i < (*varsCount); i++)
        {
            if (strncmp(vars[i].name, tempArg1, sizeof(vars[i].name)) == 0)
            {
                // Is Value A Variable Too
                for (int j = 0; j < (*varsCount); j++)
                {
                    if (strncmp(vars[j].name, tempArg2, sizeof(vars[j].name)) == 0)
                    {
                        // Update Variable To Variable Value
                        strncpy(vars[i].value, vars[j].value, sizeof(vars[i].value) - 1);
                        vars[i].value[sizeof(vars[i].value) - 1] = '\0';
                        return;
                    }
                }
                // Update That Variable To The New Value
                strncpy(vars[i].value, tempArg2, sizeof(vars[i].value) - 1);
                vars[i].value[sizeof(vars[i].value) - 1] = '\0';
                return;
            }
        }
        // Is Value A Variable
        for (int i = 0; i < (*varsCount); i++)
        {
            if (strncmp(vars[i].name, tempArg2, sizeof(vars[i].name)) == 0)
            {
                // Set tempArg2 To Carry The Value In That Value Variable
                tempArg2 = vars[i].value;
                break;
            }
        }
    }
    else
    {
        //printf("Execution Error: Out Of Bounds\n");
        error_flagger->OUT_OF_BOUNDS = 1;
        error_flagger->NO_ERRORS = 1;
        return;
    }
    if ((*varsCount) >= 0 && (*varsCount) < VARIABLE_ARRAY_LENGTH)
    {
        // All Set Then Add The Variable And Value To vars
        strncpy(vars[(*varsCount)].name, tempArg1, sizeof(vars[(*varsCount)].name) - 1);
        strncpy(vars[(*varsCount)].value, tempArg2, sizeof(vars[(*varsCount)].value) - 1);
        vars[(*varsCount)].name[sizeof(vars[(*varsCount)].name) - 1] = '\0';
        vars[(*varsCount)].value[sizeof(vars[(*varsCount)].value) - 1] = '\0';
        (*varsCount)++;
    }
    else
    {
        //printf("Variable Error: Variable Array Full\n");
        error_flagger->VARIABLE_ARRAY_FULL = 1;
        error_flagger->NO_ERRORS = 1;
    }
}

void printr(struct Variable *vars, int *varsCount, char *tempArg1, struct ErrorFlags *error_flagger)
{
    // Bounds Checking
    if ((*varsCount) >= 0 && (*varsCount) < VARIABLE_ARRAY_LENGTH)
    {
        // Is printr_arg A Variable
        for (int i = 0; i < (*varsCount); i++)
        {
            if (strcmp(vars[i].name, tempArg1) == 0)
            {
                printf("%s\n", vars[i].value);
                return;
            }
        }
        printf("%s\n", tempArg1);
        return;
    }
    else
    {
        //printf("Execution Error: Out Of Bounds\n");
        error_flagger->OUT_OF_BOUNDS = 1;
        error_flagger->NO_ERRORS = 1;
    }
}

void executeLine(char *content, struct Variable *vars, int *varsCount, int *execCounter, int *tempIndex, char *tempValue, int *IF_FLAG, struct ErrorFlags *error_flagger)
{
    // Error Checking
    if (content == NULL || strlen(content) == 0)
    {
        return;
    }
    // Remove New Line
    content[strcspn(content, "\n")] = '\0';
    // Get The Code
    char *tempCommand = strtok(content, " ");
    if (tempCommand == NULL)
    {
        return;
    }
    // Printing To stdout
    if (strcmp(tempCommand, "printr") == 0 && (*IF_FLAG) == 1)
    {
        // Gonna Capture Entire Rest Of Argument Till Null Terminator
        char *tempArg1 = strtok(NULL, "");
        if (tempArg1 == NULL)
        {
            //printf("Syntax Error: Missing argument after printr\n");
            error_flagger->ARGUMENT_ERROR = 1;
            error_flagger->NO_ERRORS = 1;
            return;
        }
        printr(vars, varsCount, tempArg1, error_flagger);
    }
    // Initializing Or Setting Variables
    else if (strcmp(tempCommand, "set") == 0 && (*IF_FLAG) == 1)
    {
        char *tempArg1 = strtok(NULL, " ");
        char *tempArg2 = strtok(NULL, " ");
        if (tempArg1 == NULL)
        {
            //printf("Syntax Error: Missing variable name after set\n");
            error_flagger->ARGUMENT_ERROR = 1;
            error_flagger->NO_ERRORS = 1;
            return;
        }
        if (tempArg2 == NULL)
        {
            //printf("Syntax Error: Missing argument value after set\n");
            error_flagger->ARGUMENT_ERROR = 1;
            error_flagger->NO_ERRORS = 1;
            return;
        }
        setVariable(vars, varsCount, tempArg1, tempArg2, error_flagger);
    }
    // Comments
    else if (strncmp(tempCommand, "//", 2) == 0 && (*IF_FLAG) == 1)
    {
        // If Comments
        return;
    }
    else if (strcmp(tempCommand, "add") == 0 && (*IF_FLAG) == 1)
    {
        char *tempResultVar = strtok(NULL, " ");
        char *tempNum1 = strtok(NULL, " ");
        char *tempNum2 = strtok(NULL, " ");
        if (tempResultVar == NULL)
        {
            //printf("Add Error: No destination for add result\n");
            error_flagger->ARGUMENT_ERROR = 1;
            error_flagger->NO_ERRORS = 1;
            return;
        }
        if (tempNum1 == NULL)
        {
            //printf("Add Error: Missing num 1 after add\n");
            error_flagger->ARGUMENT_ERROR = 1;
            error_flagger->NO_ERRORS = 1;
            return;
        }
        if (tempNum2 == NULL)
        {
            //printf("Add Error: Missing num 2 after add\n");
            error_flagger->ARGUMENT_ERROR = 1;
            error_flagger->NO_ERRORS = 1;
            return;
        }
        mathOperations(tempCommand, vars, varsCount, tempResultVar, tempNum1, tempNum2, error_flagger);
    }
    else if (strcmp(tempCommand, "sub") == 0 && (*IF_FLAG) == 1)
    {
        // Check For Error(s) In Inputs
        char *tempResultVar = strtok(NULL, " ");
        char *tempVar1 = strtok(NULL, " ");
        char *tempVar2 = strtok(NULL, " ");
        if (tempResultVar == NULL)
        {
            //printf("Syntax Error: Missing subtraction destination after sub\n");
            error_flagger->ARGUMENT_ERROR = 1;
            error_flagger->NO_ERRORS = 1;
            return;
        }
        if (tempVar1 == NULL)
        {
            //printf("Syntax Error: Missing num 1 in sub arguments\n");
            error_flagger->ARGUMENT_ERROR = 1;
            error_flagger->NO_ERRORS = 1;
            return;
        }
        if (tempVar2 == NULL)
        {
            //printf("Syntax Error: Missing num 2 in sub arguments\n");
            error_flagger->ARGUMENT_ERROR = 1;
            error_flagger->NO_ERRORS = 1;
            return;
        }
        mathOperations(tempCommand, vars, varsCount, tempResultVar, tempVar1, tempVar2, error_flagger);
    }
    else if (strcmp(tempCommand, "mul") == 0 && (*IF_FLAG) == 1)
    {
        // Check For Errors
        char *tempResultVar = strtok(NULL, " ");
        char *tempVar1 = strtok(NULL, " ");
        char *tempVar2 = strtok(NULL, " ");
        if (tempResultVar == NULL)
        {
            //printf("Syntax Error: Missing multiplication destination after mul\n");
            error_flagger->ARGUMENT_ERROR = 1;
            error_flagger->NO_ERRORS = 1;
            return;
        }
        if (tempVar1 == NULL)
        {
            //printf("Syntax Error: Missing num 1 in multiplication arguments\n");
            error_flagger->ARGUMENT_ERROR = 1;
            error_flagger->NO_ERRORS = 1;
            return;
        }
        if (tempVar2 == NULL)
        {
            //printf("Syntax Error: Missing num 2 in multiplication arguments\n");
            error_flagger->ARGUMENT_ERROR = 1;
            error_flagger->NO_ERRORS = 1;
            return;
        }
        mathOperations(tempCommand, vars, varsCount, tempResultVar, tempVar1, tempVar2, error_flagger);
    }
    else if (strcmp(tempCommand, "div") == 0 && (*IF_FLAG) == 1)
    {
        // Check For Errors
        char *tempResultVar = strtok(NULL, " ");
        char *tempVar1 = strtok(NULL, " ");
        char *tempVar2 = strtok(NULL, " ");
        if (tempResultVar == NULL)
        {
            //printf("Syntax Error: Missing division destination after div\n");
            error_flagger->ARGUMENT_ERROR = 1;
            error_flagger->NO_ERRORS = 1;
            return;
        }
        if (tempVar1 == NULL)
        {
            //printf("Syntax Error: Missing num 1 in division arguments\n");
            error_flagger->ARGUMENT_ERROR = 1;
            error_flagger->NO_ERRORS = 1;
            return;
        }
        if (tempVar2 == NULL)
        {
            //printf("Syntax Error: Missing num 2 in division arguments\n");
            error_flagger->ARGUMENT_ERROR = 1;
            error_flagger->NO_ERRORS = 1;
            return;
        }
        mathOperations(tempCommand, vars, varsCount, tempResultVar, tempVar1, tempVar2, error_flagger);
    }
    else if (strcmp(tempCommand, "when") == 0 && (*IF_FLAG) == 1)
    {
        // Check For Errors
        char *tempCondition = strtok(NULL, " ");
        if (tempCondition == NULL)
        {
            //printf("Syntax Error: Missing condition after when\n");
            error_flagger->ARGUMENT_ERROR = 1;
            error_flagger->NO_ERRORS = 1;
            return;
        }
        // Save The Condition
        strncpy(tempValue, tempCondition, sizeof(tempValue) - 1);
        tempValue[sizeof(tempValue) - 1] = '\0';
        // Save when Code Line Index
        (*tempIndex) = (*execCounter);
    }
    else if (strcmp(tempCommand, "end") == 0 && (*IF_FLAG) == 1)
    {
        // Preventing Incidence Where tempIndex Was Not Set
        // Due To Not Finding when Hence tempIndex Is 0 And After Subtraction It Is -1
        // Out Of Bounds Access
        if (tempValue[0] == '\0' || (*tempIndex) <= 0)
        {
            // Handles Nested Loops Hence No Errors
            return;
        }
        // Bounds Checking
        if ((*varsCount) >= 0 && (*varsCount) < VARIABLE_ARRAY_LENGTH)
        {
            // Is tempValue A Variable
            for (int i = 0; i < (*varsCount); i++)
            {
                if (strcmp(vars[i].name, tempValue) == 0)
                {
                    if (atoi(vars[i].value) != 0)
                    {
                        // main() Will Add 1 After This
                        (*execCounter) = (*tempIndex) - 1;
                    }
                    return;
                }
            }
        }
        else
        {
            //printf("Execution Error: Out Of Bounds\n");
            error_flagger->OUT_OF_BOUNDS = 1;
            error_flagger->NO_ERRORS = 1;
            return;
        }
        // Is It Just A Number
        if (atoi(tempValue) != 0)
        {
            (*execCounter) = (*tempIndex) - 1;
        }
        else
        {
            // The *execCounter Is Controlled In The Main Loop
            // This Code Just Takes Over Sometimes In when Statements
            memset(tempValue, 0, BUFFER_LENGTH);
            // No Use - Safety
            (*tempIndex) = 0;
        }
    }
    else if (strcmp(tempCommand, "if") == 0 && (*IF_FLAG) == 1)
    {
        char *tempConditionVar1 = strtok(NULL, " ");
        char *tempConditionVar2 = strtok(NULL, " ");
        if (tempConditionVar1 == NULL)
        {
            //printf("Syntax Error: Missing condition(s) after if\n");
            error_flagger->ARGUMENT_ERROR = 1;
            error_flagger->NO_ERRORS = 1;
            return;
        }
        if (tempConditionVar2 == NULL)
        {
            //printf("Syntax Error: Missing condition(s) after if\n");
            error_flagger->ARGUMENT_ERROR = 1;
            error_flagger->NO_ERRORS = 1;
            return;
        }
        // Bounds Checking
        if ((*varsCount) >= 0 && (*varsCount) < VARIABLE_ARRAY_LENGTH)
        {
            for (int i = 0; i < (*varsCount); i++)
            {
                // Is tempConditionVar1 A Variable
                if (strcmp(vars[i].name, tempConditionVar1) == 0)
                {
                    tempConditionVar1 = vars[i].value;
                }
                // Is tempConditionVar2 A Variable
                if (strcmp(vars[i].name, tempConditionVar2) == 0)
                {
                    tempConditionVar2 = vars[i].value;
                }
            }
        }
        else
        {
            //printf("Execution Error: Out Of Bounds\n");
            error_flagger->OUT_OF_BOUNDS = 1;
            error_flagger->NO_ERRORS = 1;
            return;
        }
        // If Condition Is Equal Then Set Global Flag To 1 So That Next Code Can Be Run
        if (atoi(tempConditionVar1) == atoi(tempConditionVar2))
        {
            (*IF_FLAG) = 1;
        }
        else
        {
            (*IF_FLAG) = 0;
        }
    }
    else if (strcmp(tempCommand, "done") == 0)
    {
        // Only When "if" Code Was Meant Not To Execute Resume Execution
        if ((*IF_FLAG) == 0)
        {
            (*IF_FLAG) = 1;
        }
    }
    else
    {
        // If That Code Could Have Been Executed
        if ((*IF_FLAG) == 1)
        {
            //printf("Unknown command: %s\n", tempCommand);
            error_flagger->BAD_COMMAND = 1;
            error_flagger->NO_ERRORS = 1;
        }
    }
}
