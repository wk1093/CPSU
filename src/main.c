#include <stdio.h>

#define CPSU_IMPLEMENTATION
#include <cpsu/cpsu.h>

int main() {
    suCompiler compiler = suCompilerNew(suFalse, SU_COMPILER_PREFERENCE_GNUC_LLVM_MSVC);
    suCompilerSetOutput(&compiler, "main.o");
    suCompilerAddInput(&compiler, "main.c");
    const char* comand = suCompilerGetCommand(&compiler);
    printf("%s\n", comand);
}
