#ifndef COMPILER_H
#define COMPILER_H

#define GOOD 0
#define ERROR -1

#define BUFFER_LENGTH 256
#define VARIABLE_ARRAY_LENGTH 10

struct Variable
{
    char name[256];
    char value[256];
};


int checkFile(char *fileName, char *realExtension);
int openFile(char *filepath);
void executeLine(char *content, struct Variable *vars, int *varsCount, int *execCounter, int *tempIndex, char *tempValue, int *IF_FLAG);
void mathOperations(char *operation, struct Variable *vars, int *varsCount, char *tempResult, char *tempVar1, char *tempVar2);
void setVariable(struct Variable *vars, int *varsCount, char *tempArg1, char *tempArg2);
void printr(struct Variable *vars, int *varsCount, char *tempArg1);

#endif
