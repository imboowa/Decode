#ifndef COMPILER_H
#define COMPILER_H

#define GOOD 0
#define ERROR -1
#define BUFFER_LENGTH 256
#define VARIABLE_ARRAY_LENGTH 10
#define LABELS_ARRAY_LENGTH 10

struct Variable
{
    char name[BUFFER_LENGTH];
    char value[BUFFER_LENGTH];
};

struct ErrorFlags
{
    unsigned int OUT_OF_BOUNDS;
    unsigned int DIVISION_BY_ZERO;
    unsigned int BAD_COMMAND;
    unsigned int ARGUMENT_ERROR;
    unsigned int VARIABLE_ARRAY_FULL;
    unsigned int NO_ERRORS;
};

struct Labels
{
    char name[BUFFER_LENGTH];
    int index;
};

int checkFile(char *fileName, char *realExtension);
int openFile(char *filepath);
void executeLine(char *content, struct Variable *vars, struct Labels *labels, int *varsCount, int *labelsCount, int *execCounter, int *tempIndex, char *tempValue, int *IF_FLAG, struct ErrorFlags *error_flagger, unsigned int *codeSpan);
void mathOperations(char *operation, struct Variable *vars, int *varsCount, char *tempResult, char *tempVar1, char *tempVar2, struct ErrorFlags *error_flagger);
void setVariable(struct Variable *vars, int *varsCount, char *tempArg1, char *tempArg2, struct ErrorFlags *error_flagger);
void printr(struct Variable *vars, int *varsCount, char *tempArg1, struct ErrorFlags *error_flagger);
int is_whole_string_integer(char *string);
#endif
