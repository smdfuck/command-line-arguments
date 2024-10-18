#pragma once
#include <iostream>

struct StringFlag {
    const char* short_name = nullptr;
    const char* long_name = nullptr;
    const char** endpoint;

    static StringFlag CreateFlag(
        const char* sh_name, const char* lg_name, const char** endp
    ) {
        StringFlag flag;
        flag.endpoint = endp;
        flag.short_name = sh_name;
        flag.long_name = lg_name;

        return flag;
    }

};

struct IntFlag {
    const char* short_name = nullptr;
    const char* long_name = nullptr;
    const int** endpoint = nullptr;

    static IntFlag CreateFlag(
        const char* sh_name, const char* lg_name, const int** endp
    ) {
        IntFlag flag;
        flag.endpoint = endp;
        flag.short_name = sh_name;
        flag.long_name = lg_name;

        return flag;
    }
};

struct BoolFlag {
    const char* short_name = nullptr;
    const char* long_name = nullptr;
    const bool** endpoint;
    bool set = false;

    static BoolFlag CreateFlag(
        const char* sh_name, const char* lg_name, const bool** endp, bool set
    ) {
        BoolFlag flag;
        flag.endpoint = endp;
        flag.short_name = sh_name;
        flag.long_name = lg_name;
        flag.set = set;

        return flag;
    }
};

struct FileArgument {
    const char** file_endp = nullptr;

    static FileArgument CreateArgument(
        const char** endp
    ) {
        FileArgument arg;
        arg.file_endp = endp;

        return arg;
    }
};

struct Flags {
    BoolFlag* bool_flags;
    size_t bool_flags_size = 0;
    
    IntFlag* int_flags;
    size_t int_flags_size = 0;

    StringFlag* string_flags;
    size_t string_flags_size = 0;

    FileArgument* file_arguments;
    size_t count_files = 0;

    static Flags CreateFlags(
        BoolFlag* bool_flags,
        const size_t bool_flags_size,
        IntFlag* int_flags,
        const size_t int_flags_size,
        StringFlag* string_flags,
        const size_t string_flags_size,
        FileArgument* file_arguments,
        const size_t count_files
    ) {
        Flags flags;
        flags.bool_flags = bool_flags;
        flags.bool_flags_size = bool_flags_size;
        flags.int_flags = int_flags;
        flags.int_flags_size = int_flags_size;
        flags.string_flags = string_flags;
        flags.string_flags_size = string_flags_size;
        flags.file_arguments = file_arguments;
        flags.count_files = count_files;

        return flags;
    }
};

class Arguments {
public:
    static int Parse(Flags flags, int argc, char** argv);

private:
    static void ParseOneArg(const char* arg, char*& name, char*& value);

    static int ParsingResult(
        const int** endpoint, char* arg_value, char** argv, int argc, int& iter
    );
    static int ParsingResult(
        const char** endpoint, char* arg_value, char** argv, int argc, int& iter
    );

    static bool ArgCmp(
        const char* value, const char* short_f, const char * long_f
    );

    static void ErrorOnParsingArgument(const char* argument_name) {
        std::cerr << "Error on parsing argument: " 
        << argument_name << std::endl;
    }

    static int ProcessArg(const char*, const char*, const int**);
    static int ProcessArg(const char*, const char*, const char**);

    static void WriteArgValue(const char* arg, char* value);
    static void WriteArgName(const char* arg, char* name);
    
    static void CountArgumentSize(
        const char* arg, int &value_size, int &name_size
    );
};
