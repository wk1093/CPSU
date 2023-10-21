#include <stdio.h>

#define CPSU_IMPLEMENTATION
#include <cpsu/cpsu.h>

int main() {
    suPath p = suPathNewDir("./src/cpsu///");
    printf("%s\n", suPathToString(p));
}
