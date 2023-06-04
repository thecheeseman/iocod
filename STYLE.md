# Style Guide

In order to maintain consistency within the project, please maintain the 
following coding style guidelines. 

```
The key words “MUST”, “MUST NOT”, “REQUIRED”, “SHALL”, “SHALL NOT”, 
“SHOULD”, “SHOULD NOT”, “RECOMMENDED”, “MAY”, and “OPTIONAL” in this 
document are to be interpreted as described in 
[RFC 2119](https://www.ietf.org/rfc/rfc2119.txt).
```

## Table of Contents:
1. [Files](#files)
    - [Headers](#headers)
    - [Sources](#sources)
    - [Include Ordering](#include-ordering)
2. [Lines](#lines)

# Files

All files MUST use `snake_case` for naming.

## Headers

All header files for the project MUST use the extension `.h`.

All headers MUST use `#include` guards to prevent multiple inclusion, and each
header SHOULD use the file path as the defined label. The closing `#endif`
SHOULD be followed by a comment with the label used.

For example, for a file path of `src/core/subfolder/file_name.h`:

``` cpp
#ifndef CORE_SUBFOLDER_FILE_NAME_H
#define CORE_SUBFOLDER_FILE_NAME_H

...

#endif // CORE_SUBFOLDER_FILE_NAME_H
```

## Sources

Source files MUST use the extension `.cpp`. Source files SHOULD prefer to be
named as their contents.

For example, the source file containing the definition of the class `FunClass`
might be named `fun_class.cpp`.

## Include Ordering

Included files in both headers and source files SHOULD be sorted alphabetically,
preferring to separate blocks of C headers from C++ headers and logically group
related headers together.

For example:

``` cpp
#ifndef CORE_EXAMPLE_HEADER_H
#define CORE_EXAMPLE_HEADER_H

#include <stdint.h>
#include <stddef.h>

#include <array>
#include <vector>

#include "local_file.h"

// etc...
```

# Lines

Indentation MUST be exactly 4 spaces. All line endings MUST be `LF`.

A line of code SHOULD be 100 characters or less in length. If necessary and to 
allow for ease of readability, a line of code MAY exceed 100 charcters, 
but MUST NOT exceed a strict maximum of 120 characters. Lines of code which 
exceed the limit MUST be continued on the following line, and SHOULD be 
indented to match the previous line. 

For example, in an `if` statement, the line is continued indented with the 
first letter after the opening parenthesis:
```c
if (this_function(param, param2) && this_other_function(param3, param4) && 
    this_third_function()) {
    // ...
}
```

Or if the line involves a function definition or function call, the parameters
are indented after the first paranthesis:
```c
int ret = some_function_with_many_params(parameter1, parameter2, parameter3,
                                         parameter4, parameter5); // etc.
```

# Spacing

## Functions & Namespaces

Opening braces `{` for both functions and namespaces MUST be on their own line.

``` cpp title="Example"
namespace test
{

void SomeFreeFunction()
{
// ...
}

} // namespace test
```

The code between a namespace's braces `{ }` MUST NOT be indented.

## Statements

The basic guidelines for statement spacing are:

- Opening braces `{` MUST be placed on the same line as the statement they open
- Single-line statements MAY omit bracing, but only if all compound statements 
  are also single-line
- Closing braces `}` MUST be placed on their own line, unless they are part of 
  a compound statement
- One line MUST NOT contain more than one statement

``` cpp title="Example: Single-line Statements"
if (some_statement)
    do_this();
else
    do_that();
```

``` cpp title="Example: Multi-line Compound Statements"
if (some_boolean) {
    do_some_work();
    do_more_work();
    return true;
} else {
    return false;
}
```

``` cpp title="Example: Multi-line Function Call Split Aross 3 Lines"
if (this_statement) {
    this_is_a_long_function_call(argument_that_is_long,
                                 argument_that_is_also_long,
                                 argument_three);
} else {
    other_thing();
}
```

``` cpp title="Example: Multi-line If Statement"
if (this_condition &&
    this_other_condition &&
    some_other_thing) {
    actually_do_the_thing();	
} else {
    something_else();
}
```

``` cpp title="Example: Nested Ifs"
if (regular_statement) {
    if (something_else)
        do_this();
    else
        do_that();
}
```

## Classes, Structures, & Enums

- Opening braces `{` MUST be placed on the same line as the `class`, `struct`, 
  or `enum` keyword
- Closing braces `}` MUST be placed on their own line

``` cpp title="Example"
class CoolClass : public OtherClass {

// contents...

};
```

## Access Specifiers & Case Labels

Access specifiers and case labels MUST be placed on the same indentation
plane as the `class` and `switch` keywords. Classes SHOULD prefer to use
the access specifier order of `public`, `protected`, `private`.

``` cpp
// access specifiers
class NoIndents {
public:
    void do_thing();
protected:
    std::string my_string;
private:
    int some_var;
};

// case labels example
void some_func(int a)
{
    switch (a) {
    case SOME_LABEL:
        // do something
        break;
    case OTHER_LABEL:
        // something else
        break;
    default:
        break;
    }
}
```

## Keywords

A space MUST be used between the following keywords and a brace `{`, open parenthesis `(` or 
open bracket `<`:

``` cpp title="Keywords"
if else switch for do while try catch struct class template enum
```

`sizeof` MUST be treated as if it is a function, and be enclosed in `( )` parentheses.

``` cpp title="Example"
size_t size = sizeof(some_object);
```

## Operators

A space MUST be used between the following operators and their operands:

``` cpp title="Operators"
= + - < > * / % | & ^ <= >= == != <=> ? :
```

Pointers and references MUST be placed adjacent to their type:
``` cpp title="Example"
int* ptr;
int& ref;
```

# Naming Conventions

Function, class, enum, and variable names SHOULD be clear and concise and
express exactly what they do.

## Functions & Variables

- Variables SHOULD prefer `snake_case` in all scenarios
- Free functions SHOULD prefer to use `PascalCase`
- Free functions which mirror STL functionality (e.g., `make_xxx_type`) SHOULD
  prefer to use `snake_case`

``` cpp
// generic free function
void SomeFreeFunction(const std::string& value)
{
    int my_var = 42;
    // ...
}

// free function mirroring STL functionality
template <typename E>
constexpr Unexpected<std::decay_t<E>> make_unexpected(E&& value)
{
    return Unexpected<std::decay_t<E>>(std::forward<E>(value));
}
```

## Classes & Structures

- Classes and structures SHOULD prefer to use `PascalCase`
- Class methods SHOULD prefer to use `CamelCase` (unless implementing STL-like
  functionality, in which case use `snake_case`)
- Class members SHOULD prefer to use `snake_case` and end with a `_`

``` cpp title="Example"
class InterestingType {
public:
    bool HasSomethingHappened() const;

private:
    int some_variable_;
    std::string my_string_;
    // ...
};
```

## Enums
- Enums SHOULD prefer to use `PascalCase`

``` cpp title="Example"
enum class VariableType {
    Integer,
    Floating,
    String,
    // ...
};
```

# Comments

Comments SHOULD prefer to use C++ style `//`. 

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

# Documentation

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
