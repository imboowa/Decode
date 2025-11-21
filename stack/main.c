#include <stdio.h>
#include <stdlib.h>

#include "common.h"

int main(int argc, const char * argv[]) {
    
    struct Stack stack_t = {0};
    stackInit(&stack_t);
    stackPush(&stack_t, "Minecraft");
    stackPush(&stack_t, "Fortnite");
    stackPush(&stack_t, "FFVII");
    stackPush(&stack_t, "MortalKombat11");
    stackPush(&stack_t, "Injustice2");
    //for (int i = 0; i < stack_t.top; i++)
      //  printf("%s\n", stack_t.items[i]);
    stackPop(&stack_t);
    stackPop(&stack_t);
    stackPop(&stack_t);
    stackPop(&stack_t);
    stackPop(&stack_t);
    for (int i = 0; i < stack_t.top; i++)
        printf("%s\n", stack_t.items[i]);
    return 0;
}
