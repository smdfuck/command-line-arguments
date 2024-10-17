# command-line-arguments-parser
simple command line arguments parser on cpp
## TODO
- [ ] refactor this
## Usage
```
// all logics in Arguments::
// before start, you should create structs with args

struct Args {
    // args should be pointers
    const int* age = nullptr;
    const bool* has_education = nullptr;

    // create method for clearing this pointer
    // after app would end
    void Clear() {
        if (age) delete [] age;
        if (has_education) delete [] has_education;
    }

    // create method Parse (or another name)
    int Parse(int argc, char** argv) {

        if (argc == 0) {
            return EXIT_SUCCESS;
        }

        for (int i = 1; i < argc; ++i) {

            // vars with results of working functions
            char* arg_name = nullptr;
            char* arg_value = nullptr;

            // read one arg
            Arguments::ParseOneArg(argv[i], arg_name, arg_value);

            // check your conditions (arg_name may be nullptr)
            if (Arguments::ArgCmp(arg_name, "a", "age")) {
                // if long flag, proccess parsing result
                int status_code = Arguments::ParsingResult(
                    &this->age, arg_value, argv, i
                );
                if (status_code == EXIT_FAILURE) {
                    return EXIT_FAILURE;
                }
            } else if (Arguments::ArgCmp(arg_name, "e", "edu")) {
                // if short flag without value, just write your value to arg
                this->has_education = new bool {true};
            } else {
                // if can't find compared flags, exit failure
                return EXIT_FAILURE;
            }

            // delete this pointer, because this value useless after check conditions
            delete [] arg_name;
        }

        return EXIT_SUCCESS;
    }

}
```
## Functions
```
void ParseOneArg(const char* arg, char*& name, char*& value);
```
this function check arg value, and write his name and value if exists.

```
template<typename endp>
static int ParsingResult(
    endp endpoint, char* arg_value, char** argv, int& iter
);
```
take struct field, and write value into him (only int and char).
second argument it is arg_value after ParseOneArg work, if it null, this method
try to parse next argument and if he doesn't find a value, he exit failure
iter it is out for cycle iterator

```
bool ArgCmp(
    const char* value, const char* short_f, const char * long_f
);
```
if value == nullptr, return false
if compare with short_f or long_f return true
