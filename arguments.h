#pragma once
#include <iostream>

struct ComparatorValue
{
    int code;
    void* endpoint;

    ComparatorValue(int code, void* endpoint) {
        this->code = code;
        this->endpoint = endpoint;
    }
};

class Arguments {

public:

    static const int kProcessArgWithErr = -1;
    static const int kProcessGivenArg = 0;
    static const int kProcessFollowArg = 1;
    static const int kErrOnArgumentParsing = 1;

    static const int kComparatorFalse = -1;
    static const int kComparatorLongArg = 1;
    static const int kComparatorShortArg = 2;

    void ParseOneArg(const char* arg, char** name, char** value);

    template<typename endp>
    int ParsingResult(endp endpoint, char** argv, int& iter);

    // int Parse(
        // int argc, char** argv, 
        // ComparatorValue* comparator(char* arg_name, char* arg_value)
    // );

    bool ArgCmp(const char* value, const char* short_f, const char * long_f);

private:

    void ErrorOnParsingArgument(const char* argument_name) {
        std::cerr << "Error on parsing argument: " 
        << argument_name << std::endl;
    }

    int ProcessArg(const char*, const char*, const int**);
    int ProcessArg(const char*, const char*, const char**);

    void WriteArgValue(const char* arg, char* value);
    void WriteArgName(const char* arg, char* name);
    void CountArgumentSize(const char* arg, int &value_size, int &name_size);
};