#include "compiler.h"

BEGIN_CPSU_CODE()

___suVectorDefine(suCompilerFlag, CompilerFlag);

suCompiler suCompilerNew(suBool cxx, suCompilerPreference preference) {
    suCompilerBackend backend = suCompilerBackendNew(preference);
    suCompiler compiler = { .cxx = cxx, .flags = suVectorNew_CompilerFlag(), .backend=backend };
    return compiler;
}

suCompilerBackend suCompilerBackendNew(suCompilerPreference preference) {
    suCompilerBackend be;
    be = suCompilerBackendGetFromType(SU_FIRST_PREFERENCE_ID(preference));
    if (be.type != SU_COMPILER_UNKNOWN) {
        return be;
    }
    be = suCompilerBackendGetFromType(SU_SECOND_PREFERENCE_ID(preference));
    if (be.type != SU_COMPILER_UNKNOWN) {
        return be;
    }
    be = suCompilerBackendGetFromType(SU_THIRD_PREFERENCE_ID(preference));
    if (be.type != SU_COMPILER_UNKNOWN) {
        return be;
    }
    fprintf(stderr, "No compiler backend found for preference %d\n", preference);
    exit(1);
}

suCompilerBackend suCompilerBackendGetFromType(suCompilerType type) {
    suCompilerBackend be;
    switch (type) {
        case SU_COMPILER_GNUC:
            be = suCompilerBackendGetGCC();
            break;
        case SU_COMPILER_LLVM:
            be = suCompilerBackendGetClang();
            break;
        case SU_COMPILER_MSVC:
            be = suCompilerBackendGetMSVC();
            break;
        default:
            be.type = SU_COMPILER_UNKNOWN;
            break;
    }
    return be;
}

suCompilerBackend suCompilerBackendGetGCC() {
    suCompilerBackend be;
    be.type = SU_COMPILER_UNKNOWN; // if error return be; if success set be.type

    // gcc should be in PATH

    int32_t ret = system("gcc --version > /dev/null 2>&1");
    int32_t ret2 = system("g++ --version > /dev/null 2>&1");
    if (ret == 0) {
        be.type = SU_COMPILER_GNUC;
        be.cCompiler32 = "gcc";
        be.cCompiler64 = "gcc";
    }
    if (ret2 == 0) {
        be.type = SU_COMPILER_GNUC;
        be.cxxCompiler32 = "g++";
        be.cxxCompiler64 = "g++";
    }
    return be;
}

suCompilerBackend suCompilerBackendGetClang() {
    suCompilerBackend be;
    be.type = SU_COMPILER_UNKNOWN; // if error return be; if success set be.type

    // clang should be in PATH
    // if it isn't, it might be windows, and clang can be in VSINSTALLDIR/VC/Tools/Llvm/bin/clang.exe
    // for now just look in PATH

    int32_t ret = system("clang --version > /dev/null 2>&1");
    int32_t ret2 = system("clang++ --version > /dev/null 2>&1");
    if (ret == 0) {
        be.type = SU_COMPILER_LLVM;
        be.cCompiler32 = "clang";
        be.cCompiler64 = "clang";
    }
    if (ret2 == 0) {
        be.type = SU_COMPILER_LLVM;
        be.cxxCompiler32 = "clang++";
        be.cxxCompiler64 = "clang++";
    }
    return be;
}

suCompilerBackend suCompilerBackendGetMSVC() {
    suCompilerBackend be;
    be.type = SU_COMPILER_UNKNOWN; // if error return be; if success set be.type

    // if msvc was in path, it would be so much easier
    // but by default it is not, so we have to find it
    // use VSINSTALLDIR env var to help find it
    // for now (maybe change later) if VSINSTALLDIR is not set, fail

    char* vsinstalldir = getenv("VSINSTALLDIR");
    if (vsinstalldir == NULL || vsinstalldir[0] == '\0') {
        fprintf(stderr, "VSINSTALLDIR not set\n");
        return be;
    }

    // VSINSTALLDIR is set, so we can try to find cl.exe
    // check areas:
    // VSINSTALLDIR/VC/Tools/MSVC/{SOME_VERSION}/bin/Host{OUR_ARCH}/{ARCH_DEP}/cl.exe

//    const char* dir_to_version = "/VC/Tools/MSVC/";
//#ifdef _WIN64
//    const char* dir_to_bin = "/bin/Hostx64/";
//#else
//    const char* dir_to_bin = "/bin/Hostx32/";
//#endif

    // find version dir name
    // find first directory in VSINSTALLDIR/VC/Tools/MSVC/
    // TODO: suFileUtils
    fprintf(stderr, "MSVC not supported yet\n");
    return be;
}

void suCompilerSetOutput(suCompiler* compiler, const char* output) {
    suCompilerStringFlag sf = { .flag = SU_COMPILER_FLAG_OUTPUT, .value = output };
    suCompilerFlag f = { .classification = SU_COMPILER_CLASSIFICATION_STRING, .stringFlag = sf };
    suVectorAdd_CompilerFlag(compiler->flags, f);
}

void suCompilerAddInput(suCompiler* compiler, const char* input) {
    suCompilerStringFlag sf = { .flag = SU_COMPILER_FLAG_INPUT, .value = input };
    suCompilerFlag f = { .classification = SU_COMPILER_CLASSIFICATION_STRING, .stringFlag = sf };
    suVectorAdd_CompilerFlag(compiler->flags, f);
}

void suCompilerAddFlag(suCompiler* compiler, suCompilerFlag flag) {
    suVectorAdd_CompilerFlag(compiler->flags, flag);
}

void suCompilerAddStringFlag(suCompiler* compiler, suCompilerStringFlag flag) {
    suCompilerFlag f = { .classification = SU_COMPILER_CLASSIFICATION_STRING, .stringFlag = flag };
    suVectorAdd_CompilerFlag(compiler->flags, f);
}

void suCompilerAddIntFlag(suCompiler* compiler, suCompilerIntFlag flag) {
    suCompilerFlag f = { .classification = SU_COMPILER_CLASSIFICATION_INT, .intFlag = flag };
    suVectorAdd_CompilerFlag(compiler->flags, f);
}

void suCompilerAddBoolFlag(suCompiler* compiler, suCompilerBoolFlag flag) {
    suCompilerFlag f = { .classification = SU_COMPILER_CLASSIFICATION_BOOL, .boolFlag = flag };
    suVectorAdd_CompilerFlag(compiler->flags, f);
}

const char* suCompilerGetCommand(suCompiler* compiler) {
    char command[1024] = { 0 };
    char* ptr = command;

#ifdef SU_BITS_64
    // add 64 bit compiler cxx or c
    if (compiler->cxx) {
        ptr += sprintf(ptr, "%s ", compiler->backend.cxxCompiler64);
    } else {
        ptr += sprintf(ptr, "%s ", compiler->backend.cCompiler64);
    }
#else
    // add 32 bit compiler cxx or c
    if (compiler->cxx) {
        ptr += sprintf(ptr, "%s ", compiler->backend.cxxCompiler32);
    } else {
        ptr += sprintf(ptr, "%s ", compiler->backend.cCompiler32);
    }
#endif

    const char** flags = NULL;
    switch (compiler->backend.type) {
        case SU_COMPILER_GNUC:
            flags = suGCCCompilerFlagTypes;
            break;
        case SU_COMPILER_LLVM:
            flags = suClangCompilerFlagTypes;
            break;
        case SU_COMPILER_MSVC:
            flags = suMSVCCompilerFlagTypes;
            break;
        default:
            fprintf(stderr, "Unknown compiler type %d\n", compiler->backend.type);
            exit(1);
    }

    char prefix = suCompilerFlagPrefix[compiler->backend.type];

    // add flags
    for (uint32_t i = 0; i < compiler->flags->size; i++) {
        suCompilerFlag flag = compiler->flags->data[i];
        switch (flag.classification) {
            case SU_COMPILER_CLASSIFICATION_BOOL:
                if (flags[flag.boolFlag.flag] == NULL || flags[flag.boolFlag.flag][0] == '\0') {
                    fprintf(stderr, "Unknown flag %d\n", flag.boolFlag.flag);
                }
                ptr += sprintf(ptr, "%c%s ", prefix, flags[flag.boolFlag.flag]);
                break;
            case SU_COMPILER_CLASSIFICATION_STRING:
                if (flags[flag.stringFlag.flag] == NULL || flags[flag.stringFlag.flag][0] == '\0') {
                    ptr += sprintf(ptr, " %s ", flag.stringFlag.value);
                } else {
                    ptr += sprintf(ptr, "%c%s %s ", prefix, flags[flag.stringFlag.flag], flag.stringFlag.value);
                }
                break;
            case SU_COMPILER_CLASSIFICATION_INT:
                if (flags[flag.intFlag.flag] == NULL || flags[flag.intFlag.flag][0] == '\0') {
                    ptr += sprintf(ptr, " %d ", flag.intFlag.value);
                } else {
                    ptr += sprintf(ptr, "%c%s %d ", prefix, flags[flag.intFlag.flag], flag.intFlag.value);
                }
                break;
        }
    }

    return strdup(command);
}

uint8_t suCompilerRun(suCompiler* compiler, suBool printCommand) {
    const char* command = suCompilerGetCommand(compiler);
    if (printCommand) {
        printf("%s\n", command);
    }
    uint8_t ret = system(command);
    free((void*)command);
    return ret;
}


END_CPSU_CODE()