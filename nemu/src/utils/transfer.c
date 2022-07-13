#include <common.h>

void char2int(char* c){
    Log("转换");
    int i=0;
    while (c[i]!="\0")
    {
        i++;
        printf("%c",c[i]);
    }
    
}