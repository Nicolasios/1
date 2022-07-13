#include <common.h>

void char2int(char* c){
    Log("%s转换",c);
    int i=0;
    while (i<20)
    {
        i++;
        printf("%d\n",c[i]);
    }
    
}