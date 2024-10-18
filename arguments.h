#pragma once
#include <iostream>

class Arguments {

public:
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

private:

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
