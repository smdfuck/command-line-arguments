# command-line-arguments-parser
simple command line arguments parser on cpp
## Usage example
```
struct Args {
    const int* age = nullptr;
    const bool* has_education = nullptr;
    const char* name = nullptr;
    const char* file_output = nullptr;

    ~Args() {
        if (age) delete [] age;
        if (has_education) delete [] has_education;
        if (name) delete [] name;
        if (file_output) delete [] file_output;
    }

    int Parse (int argc, char** argv) {
        const size_t int_flags_size = 1;
        IntFlag my_int_flags [int_flags_size] = {
            IntFlag::CreateFlag("a", "age", &this->age)
        };

        const size_t bool_flags_size = 1;
        BoolFlag my_bool_flags [bool_flags_size] = {
            BoolFlag::CreateFlag("e", "education", &this->has_education, true)
        };

        const size_t str_flags_size = 1;
        StringFlag my_str_flags [str_flags_size] = {
            StringFlag::CreateFlag("n", "name", &this->name)
        };

        const size_t count_files = 1;
        FileArgument files [count_files] = {
            FileArgument::CreateArgument(&this->file_output)
        };

        Flags flags = Flags::CreateFlags(
            my_bool_flags,
            bool_flags_size,
            my_int_flags,
            int_flags_size,
            my_str_flags,
            str_flags_size,
            files,
            count_files
        );
        int status_code = Arguments::Parse(flags, argc, argv);
        if (status_code == EXIT_FAILURE) {
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }
};

int main(int argc, char** argv) {

    Args args;

    int status_code = args.Parse(argc, argv);
    if (status_code == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
```
## Functions
```
static int Parse(Flags flags, int argc, char** argv);
```
Start main parsing with user arguments

## Structures
#### StringFlag
```
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
```


#### IntFlag
```
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
```

#### BoolFlag
```
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
```

#### FileArgument
```
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
```

#### Flags
```
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
```