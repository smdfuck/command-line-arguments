#include <iostream>
#include <cstring>

#include "./arguments.h"

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
    const char* arg, char*& arg_name, char*& arg_value
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
        arg_name = name;
    } else {
        delete [] name;
    }

    if (strcmp(value, "") != 0) {
        arg_value = value;
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
        return EXIT_SUCCESS;
    }

    if (!follow_arg) {
        return EXIT_FAILURE;
    }
    
    char* follow_name = nullptr;
    char* follow_value = nullptr;

    ParseOneArg(follow_arg, follow_name, follow_value);

    *(endpoint) = follow_value;

    delete [] follow_name;

    return EXIT_SUCCESS;
}

int Arguments::ProcessArg(
    const char* value, const char* follow_arg, const int** endpoint
) {
    if (value) {
        *(endpoint) = new int {std::atoi(value)};
        delete [] value;
    } else {
        if (!follow_arg) {
            return EXIT_FAILURE;
        }
        
        char* follow_name = nullptr;
        char* follow_value = nullptr;

        ParseOneArg(follow_arg, follow_name, follow_value);

        *(endpoint) = new int {std::atoi(follow_value)};

        delete [] follow_name;
        delete [] follow_value;

        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}

template<typename endp>
int Arguments::ParsingResult(
    endp endpoint, char* arg_value, char** argv, int argc, int& iter
) {
    if (iter + 1 > argc) {
        return EXIT_FAILURE;
    }
    
    int result = ProcessArg(arg_value, argv[++iter], endpoint);
    if (result == EXIT_FAILURE) {
        Arguments::ErrorOnParsingArgument(argv[iter - 1]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
