#include <iostream>
#include <cstring>

#include "arguments.h"

void Arguments::CountArgumentSize(
    const char* arg, int &value_size, int &name_size
) {
    int iter = 0;
    int count_dashes = 0;
    bool write_into_value = false;

    for (int i = 0; arg[i] != '\0'; ++i) {
        if (arg[i] == '-') {
            count_dashes++;
            continue;
        }
        if (arg[i] == '=') {
            write_into_value = true;
            continue;
        }

        if (write_into_value || count_dashes == 0) {
            value_size += 1;
        } else {
            name_size += 1;
        }
    }
}

void Arguments::WriteArgName(const char* arg, char* arg_name) {
    int name_iter = 0;

    if (arg[0] != '-') {
        return;
    }

    for (int i = 0; arg[i] != '\0' && arg[i] != '='; ++i) {
        if (arg[i] == '-') {
            continue;
        }
        arg_name[name_iter] = arg[i];
        name_iter++;
    }
}

void Arguments::WriteArgValue(const char* arg, char* arg_value) {
    int value_iter = 0;
    bool start_writing = false;

    if (arg[0] != '-') {
        start_writing = true;
    }

    for (int i = 0; arg[i] != '\0'; ++i) {
        if (arg[i] == '=') {
            start_writing = true;
            continue;
        }
        if (start_writing) {
            arg_value[value_iter] = arg[i];
            value_iter++;
        }
    }
}

void Arguments::ParseOneArg(
    const char* arg, char** arg_name, char** arg_value
) {
    int value_size = 0;
    int name_size = 0;

    CountArgumentSize(arg, value_size, name_size);

    char* name = new char[name_size];
    char* value = new char[value_size];

    if (value_size != 0)
    {
        WriteArgValue(arg, value);
    }
    if (name_size != 0)
    {
        WriteArgName(arg, name);
    }

    if (strcmp(name, "") != 0) {
        *arg_name = name;
    } else {
        delete [] name;
    }

    if (strcmp(value, "") != 0) {
        *arg_value = value;
    } else {
        delete [] value;
    }
}

bool Arguments::ArgCmp(
    const char* arg, const char* cmp_short, const char * cmp_long
) {
    if (arg == nullptr) {
        return false;
    }

    if (
        (cmp_short && strcmp(arg, cmp_short) == 0)
        || 
        (cmp_long && strcmp(arg, cmp_long) == 0) 
    ) {
        return true;
    } 

    return false;
}

int Arguments::ProcessArg(
    const char* value, const char* follow_arg, const char** endpoint
) {
        if (value) {
            *(endpoint) = value;
            return this->kProcessGivenArg;
        }

        if (!follow_arg) {
            return this->kProcessArgWithErr;
        }
        
        char* follow_name = nullptr;
        char* follow_value = nullptr;

        ParseOneArg(follow_arg, &follow_name, &follow_value);

        *(endpoint) = follow_value;

        delete [] follow_name;

        return this->kProcessFollowArg;
    }

int Arguments::ProcessArg(
    const char* value, const char* follow_arg, const int** endpoint
) {
    if (value) {
        *(endpoint) = new int {std::atoi(value)};
    } else {
        if (!follow_arg) {
            return this->kProcessArgWithErr;
        }
        
        char* follow_name = nullptr;
        char* follow_value = nullptr;

        ParseOneArg(follow_arg, &follow_name, &follow_value);

        *(endpoint) = new int {std::atoi(follow_value)};

        delete [] follow_name;

        return this->kProcessFollowArg;
    }

    return this->kProcessGivenArg;
}

template<typename endp>
int ParsingResult(endp endpoint, char** argv, int& iter) {

    int result = ProcessArg(argv[iter], argv[iter + 1], endpoint);
    if (result == this->kProccessArgWithErr) {
        this->ErrorOnParsingArgument(argv[iter]);
        return EXIT_FAILURE;
    }

    iter++;
}

// int Arguments::Parse(
//     int argc, char** argv, 
//     ComparatorValue* comparator(char* arg_name, char* arg_value)
// ) {
//     for (int i = 0; i < argc; ++i) {
//         char* arg_name = nullptr;
//         char* arg_value = nullptr;

//         ParseOneArg(argv[i], &arg_name, &arg_value);

//         ComparatorValue* status_code = comparator(arg_name, arg_value);
//         if (status_code->code == this->kComparatorShortArg)
//         {
//             status_code->endpoint = new bool {true};
//         }
//         else if (status_codecode == this->kComparatorLongArg)
//         {
//             this->ParsingResult(status_code->endpoint, argv, i);
//         }
//         else
//         {
//             this->ErrorOnParsingArgument(argv[i]);
//             return EXIT_FAILURE;
//         }

//         delete status_code;
//         delete [] arg_name;
//     }

//     return EXIT_SUCCESS;
// }

// int ParseArguments(const int argc, char** argv) {
//     for (int i = 1; i < argc; ++i) 
//     {
//         char* arg_name = nullptr;
//         char* arg_value = nullptr;

//         ParseOneArg(argv[i], &arg_name, &arg_value);
  
//         if (ArgCmp(arg_name, "p", "print")) 
//         {
//             this->print_request = new bool{true};
//         } 
//         else if (ArgCmp(arg_name, "o", "output")) 
//         {
//             ParsingResult(&this->path, argv, i);
//         } 
//         else if (ArgCmp(arg_name, "s", "stats")) 
//         {
//             ParsingResult(&this->stats, argv, i);
//         } 
//         else if (ArgCmp(arg_name, "w", "window")) 
//         {
//             ParsingResult(&this->window, argv, i);
//         } 
//         else if (ArgCmp(arg_name, "f", "from")) 
//         {
//             ParsingResult(&this->from_time, argv, i);
//         } 
//         else if (ArgCmp(arg_name, "e", "to"))
//         {
//             ParsingResult(&this->to_time, argv, i);
//         } 
//         else 
//         {
//             this->ErrorOnParsingArgument(argv[i]);
//             return EXIT_FAILURE;
//         }
        
//         delete [] arg_name;
//     }

//     return EXIT_SUCCESS;
// }
