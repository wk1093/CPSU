#include <stdio.h>

#define CPSU_IMPLEMENTATION
#include "cpsu/cpsu.h"

int main() {
    suPath p = suPathNewFile("./src/cpsu/test.txt");
    printf("%s\n", suPathToString(p));
    printf("%s\n", suPathGetExtension(p));
    printf("%s\n", suPathGetBasename(p));
   // const char* contents = suPathReadFileText(p); // if file doesnt exist this will return NULL, but the above functions will still work
   // printf("%s\n", contents);
}
