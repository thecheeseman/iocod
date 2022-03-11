Coding Style
============

In order to maintain consistency within the project, please maintain the 
following coding style guidelines. The stylistic choices that have been made 
are generally based on the Linux Kernel style guide, albeit with some changes.

Table of Contents
1. [Indentation](#Indentation)
2. [Line Width](#Line-Width)
3. [Bracing](#Bracing)
4. [Spacing](#Spacing)
5. [Conditions](#Conditions)
6. [Switch Statements](#Switch-Statements)
7. [Functions](#Functions)
8. [Naming Conventions](#Naming-Conventions)

## Indentation

Indentation should be with `tabs` and not spaces.

Linus would not approve, but for the best Visual Studio experience, set 
`Tab Size` and `Indent Size` to `4` (defaults). Tabs set to `8` unfortunately
break Visual Studio's ability to align function arguments properly.

## Line Width

All lines should be less than 80 characters in length. Any overflow should be 
continued on the next line and indented to match the indent level of the 
previous line.
```
if (this_function(param, param2) && this_other_function(param3, param4) && 
    this_third_function())) {
	// code...	
}
```

## Bracing

Open curly braces `{` should always be placed immediately after a given 
statement and on the same line. Ending curly braces `}` should inhabit their own
line (if there are no more compound statements), otherwise they should be placed
immediately before the next statement:
```
if (good_example) {
    do_some_work();
    do_more_work();
    return true;
} else {
    return false;
}
```

The only exception to this rule are functions, which have their opening curly
brace `{` placed on a new line.
```
void function(int argument)
{
    // body
}
```

Single-line statements do _not_ need any curly braces. Large single-line
compound statements are OK like this:
```
if (this_statement)
    do_this();
else if (this_other_statement)
    do_that();
else if (another_statement)
    do_something_else();
else
    finally();
```

There are 3 exceptions to the single-line statement rule:

1. If the statement composes multi-lines (e.g. a single function call spread
across 3 lines), curly braces _should_ be used for clarity. In addition, any
single-line `else` or `else if` statements _should also_ receive curly braces:
```
if (this_statement) {
    this_is_a_long_function_call(argument_that_is_long,
                                 argument_that_is_also_long,
                                 argument_three);
} else {
    other_thing();
}
```

2. If using a multi-line statement with many conditions, bracing should be used
regardless of how long the statement body is:
```
if (this_condition &&
    this_other_condition &&
    some_other_thing) {
    actually_do_the_thing();	
} else {
    something_else();
}
```

3. Nested if statements should _always_ have curly braces on the outermost `if`:
```
if (regular_statement) {
    if (something_else)
        do_this();
    else
        do_that();
}
```

## Spacing

A space _should_ be used after the keywords:
```
if  switch  case  for  do  while
```

but not with `sizeof`:

```
s = sizeof(struct my_struct);
```

Spaces _should_ be used around (on each side) of most binary operators:
```
=  +  -  <  >  *  /  %  |  &  ^  <=  >=  ==  !=  ?  :
```

but _no spacing_ after unary operators:
```
&  *  +  -  ~  !
```

There should there be no spaces before or after the prefix and postfix
increment / decrement operators:
```
++ --
```

Additionally, there should _not_ be any spacing around the `.` and `->` 
structure member operators.

Finally, the preferred location of the `*` when used as a pointer is adjacent
to the variable or function name, not adjacent to the type.
```
char *this_value;
unsigned int *some_function(char *asdf, int **some_value);
```

## Conditions

For boolean values, there is no need to check for equality as boolean logic
is already easily understandable:
```
if (!bool_value)
    do_that();
```

Any other type of variable should have an _explicit_ comparison. 
```
if (number == 0)
    do_foo();
```

This is true especially of `pointers`.
```
if (some_pointer == NULL) 
     do_whatever();
```

A similar logic applies to string expressions:
```
if (str != NULL && *str != '\0')
    do_bar();
```

## Switch Statements

Switch statements should follow Kernel guidelines and have each `case` indented
on the same indent level as the statement itself:

```
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

`default` case must _always_ be included.

## Functions

In general, functions should just do _one_ thing. They should strive to be as
small and compact as possible. The only exceptions here are for long, but
relatively simple single case-statement functions. Functions should try and
keep their local variables to a minimum.

Function declarations should be declared with their return type on a single
line. 
```
void test_function(void);
```

The `void` keyword _must_ be used for functions which have no parameters.

Functions with a list of arguments _can_ be broken into new lines per each
additional argument:

```
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

In line with the Linux Kernel style guide, variables, functions, and any named
values should be clear, concise, and explain exactly what they do.

- names should always be _lowercase_ and **not** use camelCase
- names should _never_ use Hungarian notation (mNumber, fFloat)
- underscores (`_`) should be used to separate different nouns / words in 
  longer names
    - e.g. `parseweaponstruct` -> `parse_weapon_struct`
- names like `i` or `j` are acceptable in cases where they are used to denote
  loop counters

The only exceptions are:

- macros constants, which should be defined in all _UPPERCASE_

An example of what not to do:
```
int mLoopCounter;
char *MYSTRING;
float SomeFloatingNumber = 1.5;
```

Better examples:
```
int i;
char *message;
float three_halfs = 1.5;
```