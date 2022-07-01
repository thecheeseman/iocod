# Coding Style

In order to maintain consistency within the project, please maintain the 
following coding style guidelines. 

The key words “MUST”, “MUST NOT”, “REQUIRED”, “SHALL”, “SHALL NOT”, 
“SHOULD”, “SHOULD NOT”, “RECOMMENDED”, “MAY”, and “OPTIONAL” in this 
document are to be interpreted as described in 
[RFC 2119](https://www.ietf.org/rfc/rfc2119.txt).

## Table of Contents
1. [Indentation](#Indentation)
2. [Line Length](#Line-Length)
3. [Bracing](#Bracing)
4. [Spacing](#Spacing)
5. [Conditions](#Conditions)
6. [Switch Statements](#Switch-Statements)
7. [Functions](#Functions)
8. [Naming Conventions](#Naming-Conventions)
9. [Comments](#Comments)
10. [Documentation](#Documentation)

## Indentation

Indentation MUST be exactly 4 spaces. Do not use tabs.

## Line Length

A line of code SHOULD be 80 characters or less in length. If necessary and to 
allow for ease of readability, a line of code MAY exceed 80 charcters, 
but MUST NOT exceed a strict maximum of 100 characters. Lines of code which 
exceed the limit MUST be continued on the following line, and SHOULD be 
indented to match the previous line. 

For example, in an `if` statement, the line is continued indented with the 
first letter after the opening parenthesis:
```c
if (this_function(param, param2) && this_other_function(param3, param4) && 
    this_third_function())) {
    // ...
}
```

Or if the line involves a function definition or function call, the parameters
are indented after the first paranthesis:
```c
int ret = some_function_with_many_params(parameter1, parameter2, parameter3,
                                         parameter4, parameter5); // etc.
```

## Bracing

Opening braces `{` for functions MUST be placed on their own line, after
the function definition.
```c
void function(int argument)
{
    // body
}
```

In multi-line statements, opening braces `{` MUST be placed immediately 
adjacent to the statement which they open. Closing braces `}` MUST be on 
their own line, unless they are part of a compound statement.

```c
if (some_boolean) {
    do_some_work();
    do_more_work();
    return true;
} else {
    return false;
}
```

Single-line statements MAY omit bracing only if all compound statements
are single-line.

```c
// OK because all statements are single-line
if (this_statement)
    do_this();
else if (this_other_statement)
    do_that();
else if (another_statement)
    do_something_else();
else
    finally();
```

Therefore, if _any_ of the statements in a compound statement block are
multi-line, bracing MUST be used for all statements. 

Some further examples:
```c
// example 1 - multi-line function call split across 3 lines
if (this_statement) {
    this_is_a_long_function_call(argument_that_is_long,
                                 argument_that_is_also_long,
                                 argument_three);
} else {
    other_thing();
}

// example 2 - multi-line if statement
if (this_condition &&
    this_other_condition &&
    some_other_thing) {
    actually_do_the_thing();	
} else {
    something_else();
}

// example 3 - nested ifs
if (regular_statement) {
    if (something_else)
        do_this();
    else
        do_that();
}
```

## Spacing

A space MUST be used between all of the following keywords:
```c
if switch for do while
```

`sizeof` MUST always be enclosed with `( )` parentheses and
MUST NOT contain a space between the opening parenthesis and `sizeof`.

```c
s = sizeof(struct my_struct);
```

Spaces MUST be used before and after each of the binary operators
`=  +  -  <  >  *  /  %  |  &  ^  <=  >=  ==  !=  ?  :`:
```c
a = b + c - d / e * f;

if (val % 2 == 0 || val & MASK)
    // ...

res = some_value ? true : false;
```

Spaces MUST NOT be used between the unary operators `& * - ~ !`:
```c
mask = ~SOME_MASK;

*p = 15;

function(&ptr);

if (!boolean)
    // ...

int a = -b;
```

Spaces MUST NOT be used before or after the decrement/increment operators `++`
and `--`:
```c
i++;
*ptr++;
--i;
```

Spaces MUST NOT be used before or after the structure reference and pointer 
operators `.` and `->`
```c
my_struct.do_this();
my_struct->do_that();
```

Pointers `*` MUST be placed adjacent to the variable or function name.
```c
char *this_value;
unsigned int *some_function(char *asdf, int **some_value);
```

## Conditionals

Non-boolean types SHOULD have explicit comparisons for the sake of code clarity.
```c
int value;
char *str;

if (value == 0)
if (str == NULL || *str == '\0')
```

Pointers SHOULD always be compared to `NULL` instead of negating:
```c
// Use
if (ptr == NULL)

// Instead of
if (!ptr)
```

## Switch Statements

Case statements inside of `switch` MUST indent each `case` on the same 
indentation level as the statement itself. The `default` case MUST always 
be included.
```c
switch (action) {
case KOBJ_ADD:
    return "add";
case KOBJ_REMOVE:
    return "remove";
case KOBJ_CHANGE:
    return "change";
default:
    return NULL;
}
```

Fallthrough statements are permitted, and MUST either use a comment to denote
the intentional fallthrough, or the `IC_FALLTHROUGH` keyword:
```c
switch (action) {
case ADD_STR:
    // do some work
    break;
case DEL_STR:
    // delete string
    break;
case OMIT_STR:
    /* fallthrough */ or IC_FALLTHROUGH;
case default:
    break;
}
```

## Functions

Functions should do only one thing, and do that thing well. They should strive 
to be as small and compact as possible. The only exceptions here are for long, 
but relatively simple single case-statement functions. Functions should try and
keep their local variables to a minimum.

Function declarations MUST be declared with their return type on a single
line. The `void` keyword MUST be used for functions which have no parameters.
```c
void test_function(void);
```

If a function is defined with additional attributes, such as `IC_PUBLIC`
or `IC_PRINTF_FORMAT(x, y)`, the attributes MUST be placed each on their
own lines before the return type:
```c
IC_PUBLIC
IC_PRINTF_FORMAT(1, 2)
void my_printf(const char *fmt, ...)
{
    // ...
}
```

Functions with a list of arguments MAY be broken into new lines per each
additional argument:
```c
struct big_data *get_all_of_my_data(int data1, 
                                    const char *data2, 
                                    struct big_data *data3, 
                                    int data4,
                                    struct other_structure *data5)
{
    // body
}
```

## Naming Conventions

Function and variable names SHOULD be clear, concise, and explain exactly
what they do.

- Names MUST use lowercase letters.
- Names MUST NOT use camelCase. 
- Names MUST NOT use Hungarian notation (mNumber, fFloat, etc).
- Names SHOULD prefer to use `_` underscores to separate nouns/words in longer names.
- Short names (`i`, `j`, etc.) MAY be used in loop counters.
- Macro functions and constants MUST use uppercase letters.

Examples:
```c
// Good
#define GRAVITY 800
char *message = NULL;
parse_weapon_struct();
float three_halfs = 1.5f;
for (int i = 0; i < 15; i++)

// Bad
#define grav 800
char *MYSTRING = NULL;
parseWeaponStruct();
fFloatNum = 1.5f;
for (int counter = 0; counter < 15; counter++)
```

## Comments

Comments MAY be either a C-style `/* */` or C++ style `//`.

Comments SHOULD be clear and concise and explain exactly _why_ a given piece
of code is doing what it's doing. Consider answering questions like 
"what does this block of code do?" and "why is this block implemented this way?" 
Comments SHOULD NOT be used to explain code which is self-evident or obvious. 
Well-written code documents itself. It is not necessary to comment every 
single line of code. 

Example code with _too many_ comments:
```c
struct cmd *cmd = NULL; // set cmd to NULL
struct cmd **prev;      // init prev
// loop through all functions until prev is NULL 
for (prev = &cmd_functions; *prev != NULL; prev = &cmd->next) {
    cmd = *prev; // set cmd to prev
    // if strcasecmp == 0
    if (strcasecmp(cmd->name, cmd_argv(0)) == 0) {
        *prev = cmd->next;          // copy next to prev
        cmd->next = cmd_functions;  // set next to function list
        cmd_functions = cmd;        // set function list to cmd

        if (cmd->function != NULL)  // if function isnt NULL
            cmd_call(cmd);          // call it
        else                        // otherwise
            break;                  // break

        return true;                // get out of here
    }
}
```

Instead, this is what the same section might look like with better comments:
```c
struct cmd *cmd = NULL;
struct cmd **prev;
for (prev = &cmd_functions; *prev != NULL; prev = &cmd->next) {
    cmd = *prev;

    if (strcasecmp(cmd->name, cmd_argv(0)) == 0) {
        // rearrange the links so that the command will be
        // near the head of the list next time it is used
        *prev = cmd->next;
        cmd->next = cmd_functions;
        cmd_functions = cmd;

        if (cmd->function != NULL)
            cmd_call(cmd);
        else
            break; // let the VMs handle it

        return true;
    }
}
```

In the above example, there are only two comments. A comment is used to 
describe _why_ the linked list is being restructured and a second comment 
is used to describe _what_ will happen if `cmd->function` is NULL (the VMs 
will handle it). It should be evident already what this code does (loops 
through the linked list `cmd_functions` searching for a matching name), 
so it is unnecessary to point this out.

## Documentation

Public functions marked with `IC_PUBLIC` MUST be documented in their respective
global header file (`include/iocod/xxx.h`) and MUST use Javadoc-style 
documentation. All documentation MUST begin with `/**` on its own single line 
and end with `*/` on its own single line.
```c
// include/iocod/system.h

/**
 * @brief Get last library error, if applicable. Clears last error.
 * @return NULL-terminated string containing error message, otherwise NULL
*/
IC_PUBLIC
char *sys_library_error(void);
```

Documentation SHOULD be as complete, clear and concise as possible. 
At a minimum, all documented functions MUST have a `@brief` description.

Functions which are local to a specific module (e.g. `network`) MUST be
documented in their local header file (e.g. `core/network/net_local.h`).

```c
// core/network/net_local.h

/**
 * @brief Gets local addresses and populates @ref localip structure.
*/
void net_get_local_address(void);
```

## asdf
