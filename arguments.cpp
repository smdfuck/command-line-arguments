#include <iostream>
#include <cstring>

#include "./arguments.h"

void Arguments::CountArgumentSize(
    const char* arg, int &value_size, int &name_size
) {
    int iter = 0;
    int count_dashes = 0;
    bool write_into_value = false;
    bool was_letter = false;

    for (int i = 0; arg[i] != '\0'; ++i) {
        if (arg[i] == '-' && !was_letter) {
            count_dashes++;
            continue;
        }
        if (arg[i] == '=') {
            write_into_value = true;
            continue;
        }
        was_letter = true;

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

    bool was_letter = false;

    for (int i = 0; arg[i] != '\0' && arg[i] != '='; ++i) {
        if (arg[i] == '-' && !was_letter) {
            continue;
        }
        was_letter = true;

        arg_name[name_iter] = arg[i];
        name_iter++;
    }
    arg_name[name_iter] = '\0';
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
    arg_value[value_iter] = '\0';
}

void Arguments::ParseOneArg(
    const char* arg, char*& arg_name, char*& arg_value
) {
    int value_size = 0;
    int name_size = 0;

    CountArgumentSize(arg, value_size, name_size);

    char* name = new char[name_size + 1];
    char* value = new char[value_size + 1];

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

int Arguments::ParsingResult(
    const int** endpoint, char* arg_value, char** argv, int argc, int& iter
) {
    if (!arg_value) {
        if (iter + 1 > argc) {
            return EXIT_FAILURE;
        }
        iter++;
    } 
    
    int result = ProcessArg(arg_value, argv[iter], endpoint);
    if (result == EXIT_FAILURE) {
        Arguments::ErrorOnParsingArgument(argv[iter - 1]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int Arguments::ParsingResult(
    const char** endpoint, char* arg_value, char** argv, int argc, int& iter
) {
    if (!arg_value) {
        if (iter + 1 > argc) {
            return EXIT_FAILURE;
        }
        iter++;
    } 
    
    int result = ProcessArg(arg_value, argv[iter], endpoint);
    if (result == EXIT_FAILURE) {
        Arguments::ErrorOnParsingArgument(argv[iter - 1]);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int Arguments::Parse(Flags flags, int argc, char** argv) {
    int f_iter = 0;
    for (int i = 1; i < argc; ++i) {
        char* arg_name = nullptr;
        char* arg_value = nullptr;

        Arguments::ParseOneArg(argv[i], arg_name, arg_value);

        bool found = false;

        if (flags.count_files != 0 && !arg_name && arg_value) {
            if (f_iter < flags.count_files)
            {
                FileArgument endp = flags.file_arguments[f_iter++];
                *(endp.file_endp) = arg_value;
                continue;
            }
        } 
        if (arg_name == nullptr && arg_value != nullptr) {
            delete [] arg_value;
            return EXIT_FAILURE;
        }
        if (flags.bool_flags_count != 0) {
            for (int j = 0; j < flags.bool_flags_count; ++j) {

                BoolFlag flag = flags.bool_flags[j]; 
                bool arg_cmp = Arguments::ArgCmp(
                    arg_name, 
                    flag.short_name,
                    flag.long_name
                );

                if (arg_cmp) {
                    found = true;
                    
                    *(flag.endpoint) = new bool {flag.set};
                }

            }
        }
        if (flags.int_flags_count != 0) {
            for (int j = 0; j < flags.int_flags_count; ++j) {

                IntFlag flag = flags.int_flags[j]; 
                bool arg_cmp = Arguments::ArgCmp(
                    arg_name, 
                    flag.short_name,
                    flag.long_name
                );

                if (arg_cmp) {
                    int status_code = Arguments::ParsingResult(
                        flag.endpoint, arg_value, argv, argc, i
                    );
                    if (status_code == EXIT_FAILURE) {
                        return EXIT_FAILURE;
                    }

                    found = true;
                }

            }
        }
        if (flags.string_flags_count != 0) {
            for (int j = 0; j < flags.string_flags_count; ++j) {

                StringFlag flag = flags.string_flags[j]; 
                bool arg_cmp = Arguments::ArgCmp(
                    arg_name, 
                    flag.short_name,
                    flag.long_name
                );

                if (arg_cmp) {
                    int status_code = Arguments::ParsingResult(
                        flag.endpoint, arg_value, argv, argc, i
                    );
                    if (status_code == EXIT_FAILURE) {
                        return EXIT_FAILURE;
                    }

                    found = true;
                }

            }
        }
        
        delete [] arg_name;
        if (!found) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
};
