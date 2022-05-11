#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../tests/include/testlib.h"
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#define MAX_CHARS		128		// for chars[] etc
#define MAX_PARAMS		4		// built in functions don't need more than this
#define MAX_FUNCTIONS	256

char output_directory[MAX_CHARS];

char declarations_name[MAX_CHARS];
bool output_declarations_enabled = false;
char redirects_header[MAX_CHARS];
char redirects_body[MAX_CHARS];
bool output_redirects_enabled = false;

char unused_define[MAX_CHARS];

char prefix[MAX_CHARS];
char *data;	// pointer to data from file

enum type {
    TYPE_NULL,
    TYPE_VOID,
    TYPE_BOOL,
    TYPE_BYTE,
    TYPE_CHAR,
    TYPE_SHORT,
    TYPE_USHORT,
    TYPE_INT,
    TYPE_UINT,
    TYPE_FLOAT,
    TYPE_VECTOR,
    TYPE_STRUCT,
    TYPE_INTPTR,
    TYPE_GENTITY,
    TYPE_MAX
};

const char *type_str[] = {
    "null",
    "void",
    "bool",
    "byte",
    "char",
    "int16_t",
    "uint16_t",
    "int32_t",
    "uint32_t",
    "float",
    "vec3_t",
    "struct",
    "intptr_t",
    "gentity_t",
    ""
};

enum type_flags {
    FLAG_NONE,
    FLAG_CONST,
    FLAG_POINTER,
    FLAG_DPOINTER = 4
};

char *flags_to_str(int flags)
{
    static char str[MAX_CHARS];
    char *p = str;
    memset(str, 0, sizeof(str));

    if (flags & FLAG_CONST)
        strcat(p, "const");
    if (flags & FLAG_POINTER)
        strcat(p, "*");
    if (flags & FLAG_DPOINTER)
        strcat(p, "*");

    return p;
}

struct parameter {
    int num;

    enum type type;
    enum type_flags type_flags;
    char struct_name[MAX_CHARS];	// optional if returning a structure

    char name[MAX_CHARS];
};

struct function_prototype {
    int num;
    bool used;
    bool no_prefix;

    enum type type;					// int
    enum type_flags type_flags;		// const, pointers
    char struct_name[MAX_CHARS];	// optional if returning a structure

    char name[MAX_CHARS];

    int num_params;
    struct parameter params[MAX_PARAMS];
};

// statically allocate because why not
struct function_prototype prototypes[MAX_FUNCTIONS];
int prototypes_count = 0;

/**
 * @brief wrapper for easy file read
*/
char *fopenread(const char *filename)
{
    int len;
    FILE *f;
    char *data;

    f = fopen(filename, "r");
    if (!f) {
        com_error(ERR_FATAL, "coulnd't open %s", filename);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);

    data = calloc(len, sizeof(char));
    if (!data) {
        com_error(ERR_FATAL, "couldn't allocate %d bytes of memory", len);
        return NULL;
    }

    fread(data, sizeof(char), len, f);
    fclose(f);

    return data;
}

/**
 * @brief Parse a variable
*/
void parse_type(int *flags, int *type, char **name)
{
    char *token;
    bool matched;
    int i;

    token = com_parse_on_line(&data);

    // catch const
    if (!strcasecmp(token, "const")) {
        *flags |= FLAG_CONST;

        token = com_parse_on_line(&data);
        if (!token[0])
            com_script_error("variable with no type");
    }

    matched = false;
    for (i = 0; i < TYPE_MAX; i++) {
        if (!strcasecmp(token, type_str[i])) {
            matched = true;
            break;
        }
    }

    if (!matched)
        com_script_error("variable type: %s", token);

    *type = i;

    // special check for structures
    if (*type == TYPE_STRUCT) {
        token = com_parse_on_line(&data);
        if (!token[0])
            com_script_error("struct with no name");

        *name = malloc(sizeof(char) * MAX_CHARS);
        strncpy(*name, token, sizeof(char) * MAX_CHARS);
    }

    // check for any extra *
    while (true) {
        token = com_parse_on_line(&data);
        if (!token[0])
            break;

        if (!strcmp(token, "*") && (*flags & FLAG_POINTER))
            *flags |= FLAG_DPOINTER;
        else if (!strcmp(token, "*"))
            *flags |= FLAG_POINTER;
        else
            com_script_error("invalid specifier: %s", token);
    }

    #ifdef DEBUG
    if (*type == TYPE_STRUCT)
        printf("parsed variable type of struct %s (%s)\n",
               *name,
               flags_to_str(*flags));
    else
        printf("parsed variable type of %s (%s)\n",
               type_str[*type],
               flags_to_str(*flags));
    #endif
}

/**
 * @brief Get a string value
*/
void parse_string_value(char *name, char *value, int size)
{
    char *token;

    token = com_parse_on_line(&data);
    if (!token[0])
        com_script_error("missing text for string value '%s'", name);

    strncpy(value, token, size);

    #ifdef DEBUG
    printf("set '%s' to '%s'\n", name, value);
    #endif
}

/**
 * @brief Get a bool value
*/
void parse_boolean_value(char *name, bool *value)
{
    char *token;

    token = com_parse_on_line(&data);
    if (!token[0])
        com_script_error("missing value for boolean '%s'", name);

    if (!strcasecmp(token, "true"))
        *value = true;
    else if (!strcasecmp(token, "false"))
        *value = false;
    else
        com_script_error("unknown value for boolean '%s'", token);

    #ifdef DEBUG
    printf("set '%s' to '%s'\n", name, token);
    #endif
}

/**
 * @brief Parse function block
*/
void parse_function(void)
{
    char func_name[MAX_CHARS];
    char *token, *name, *name2;
    struct function_prototype prototype;
    struct parameter param;
    bool matched;
    int i, flags, type;

    memset(&prototype, 0, sizeof(struct function_prototype));

    token = com_parse_on_line(&data);
    if (!token[0])
        com_script_error("function with no name");

    prototype.num = prototypes_count;
    prototype.used = true;
    prototype.type = TYPE_VOID;

    strncpy(func_name, token, sizeof(func_name));
    for (i = 0; i < prototypes_count; i++) {
        if (!strcasecmp(prototypes[i].name, func_name))
            com_script_error("duplicate function '%s'", func_name);
    }

    strncpy(prototype.name, func_name, sizeof(prototype.name));

    #ifdef DEBUG
    printf("entered function \"%s\"\n", func_name);
    #endif

    com_match_token(&data, "{", false);

    while (true) {
        token = com_parse(&data);
        if (!token[0])
            com_script_error("no matching '}' found");

        if (token[0] == '}')
            break;

        // parse return type
        if (!strcasecmp(token, "return")) {
            token = com_parse_on_line(&data);
            if (!token[0])
                com_script_error("return with no type");

            com_unget_token();

            flags = 0;
            type = 0;
            parse_type(&flags, &type, &name);

            prototype.type = type;
            prototype.type_flags = flags;

            if (type == TYPE_STRUCT && name[0]) {
                strncpy(prototype.struct_name, name, 
                        sizeof(prototype.struct_name));
                free(name);
            }
        } else if (!strcasecmp(token, "param")) {
            if (prototype.num_params == MAX_PARAMS)
                com_script_error("too many params (%d)", prototype.num_params);

            // parse argument
            token = com_parse_on_line(&data);
            if (!token[0])
                com_script_error("param with no data");

            memset(&param, 0, sizeof(struct parameter));

            if (!strcasecmp(token, "const") || !strcasecmp(token, "*"))
                com_script_error("param variable name goes before type");

            matched = false;
            for (i = 0; i < TYPE_MAX; i++) {
                if (!strcasecmp(token, type_str[i])) {
                    matched = true;
                    break;
                }
            }

            if (matched)
                com_script_error("param variable name goes before type");

            // first arg should be the name
            strncpy(param.name, token, sizeof(param.name));

            flags = 0;
            type = 0;
            parse_type(&flags, &type, &name);

            if (type == TYPE_VOID && !(flags & FLAG_POINTER))
                com_script_error("cannot have variable with type void");

            param.num = prototype.num_params;
            param.type = type;
            param.type_flags = flags;

            if (type == TYPE_STRUCT && name[0]) {
                strncpy(param.struct_name, name,
                        sizeof(param.struct_name));
                free(name);
            }

            // copy over
            com_memcpy(&prototype.params[prototype.num_params], &param, 
                   sizeof(struct parameter));
            prototype.num_params++;
        } else if (!strcasecmp(token, "unused")) {
            prototype.used = false;
        } else if (!strcasecmp(token, "noprefix")) {
            prototype.no_prefix = true;
        } else {
            com_script_error("unknown option: %s", token);
        }
    }

    #ifdef DEBUG
    printf("function %s defined as: \n", prototype.name);
    printf("  return: %s %s\n", type_str[prototype.type],
           flags_to_str(prototype.type_flags));
    printf("  params: %d\n", prototype.num_params);
    #endif

    // copy out
    com_memcpy(&prototypes[prototypes_count], &prototype,
           sizeof(struct function_prototype));
    prototypes_count++;
}

/**
 * @brief Read the file and get info into our structures
*/
void parse_prototypes(void)
{
    char *token;

    memset(&prototypes, 0, sizeof(prototypes));
    com_begin_parse_session("prototypes.txt");

    printf("parsing prototypes.txt...");

    while (true) {
        token = com_parse(&data);
        if (!token[0])
            break;

        if (!strcasecmp(token, "prefix")) {
            parse_string_value("prefix", prefix, sizeof(prefix));
        } else if (!strcasecmp(token, "output_directory")) {
            parse_string_value("output_directory", output_directory,
                               sizeof(output_directory));
        } else if (!strcasecmp(token, "output_declarations")) {
            parse_boolean_value("output_declarations", 
                                &output_declarations_enabled);
        } else if (!strcasecmp(token, "declarations_name")) {
            parse_string_value("declarations_name", 
                               declarations_name, 
                               sizeof(declarations_name));
        } else if (!strcasecmp(token, "output_redirects")) {
            parse_boolean_value("output_redirects", &output_redirects_enabled);
        } else if (!strcasecmp(token, "redirects_header")) {
            parse_string_value("redirects_header", redirects_header,
                               sizeof(redirects_header));
        } else if (!strcasecmp(token, "redirects_body")) {
            parse_string_value("redirects_body", redirects_body,
                               sizeof(redirects_body));
        } else if (!strcasecmp(token, "unused_define")) {
            parse_string_value("unused_define", unused_define,
                               sizeof(unused_define));
        } else if (!strcasecmp(token, "func")) {
            parse_function();
        } else {
            com_script_error("syntax error");
        }
    }

    com_end_parse_session();
    printf("ok\nparsed %d total functions\n", prototypes_count);
}

/**
 * @brief output header file declarations
*/
void output_declarations(void)
{
    char path[256];
    FILE *out;
    int i, j;
    struct function_prototype *prototype;
    struct parameter *param;

    printf("outputting declarations...");

    snprintf(path, sizeof(path), "%s/%s", output_directory, declarations_name);

    out = fopen(path, "wb");
    if (!out)
        com_error(ERR_FATAL, "couldn't open file '%s'", path);

    for (i = 0; i < prototypes_count; i++) {
        prototype = &prototypes[i];
        if (prototype->type_flags & FLAG_CONST)
            fprintf(out, "const ");

        if (prototype->type == TYPE_STRUCT) {
            fprintf(out, "struct %s %s%s", prototype->struct_name,
                    (prototype->type_flags & FLAG_POINTER) ? "*" : "",
                    (prototype->type_flags & FLAG_DPOINTER) ? "*" : "");
        } else {
            fprintf(out, "%s %s%s", type_str[prototype->type],
                    (prototype->type_flags & FLAG_POINTER) ? "*" : "",
                    (prototype->type_flags & FLAG_DPOINTER) ? "*" : "");
        }

        if (!prototype->used)
            fprintf(out, "%s%s ",
                    (prototype->type_flags & FLAG_POINTER) ? " " : "",
                    unused_define);

        fprintf(out, "%s%s(", prototype->no_prefix ? "" : prefix, 
                prototype->name);

        if (prototype->num_params) {
            for (j = 0; j < prototype->num_params; j++) {
                param = &prototype->params[j];

                if (j && prototype->num_params > 1)
                    fprintf(out, ", ");

                if (param->type_flags & FLAG_CONST)
                    fprintf(out, "const ");

                if (param->type == TYPE_STRUCT) {
                    fprintf(out, "struct %s %s%s%s", param->struct_name,
                            (param->type_flags & FLAG_POINTER) ? "*" : "",
                            (param->type_flags & FLAG_DPOINTER) ? "*" : "",
                            param->name);
                } else {
                    fprintf(out, "%s %s%s%s", type_str[param->type],
                            (param->type_flags & FLAG_POINTER) ? "*" : "",
                            (param->type_flags & FLAG_DPOINTER) ? "*" : "",
                            param->name);
                }
            }
        } else {
            fprintf(out, "void");
        }

        fprintf(out, ");\n");
    }

    fclose(out);
    printf("ok\n");
}

/**
 * @brief output the game dll redirects
*/
void output_redirects(void)
{
    char path_header[256], path_body[256];
    FILE *header, *body;
    int i, j;
    struct function_prototype *prototype;
    struct parameter *param;

    printf("outputting redirects...");

    snprintf(path_header, sizeof(path_header), "%s/%s", output_directory,
             redirects_header);
    snprintf(path_body, sizeof(path_body), "%s/%s", output_directory,
             redirects_body);

    header = fopen(path_header, "w");
    if (!header)
        com_error(ERR_FATAL, "couldn't open file '%s'", path_header);

    for (i = 0; i < prototypes_count; i++) {
        prototype = &prototypes[i];
        if (prototype->type_flags & FLAG_CONST)
            fprintf(header, "const ");

        if (prototype->type == TYPE_STRUCT) {
            fprintf(header, "struct %s %s%s", prototype->struct_name,
                    (prototype->type_flags & FLAG_POINTER) ? "*" : "",
                    (prototype->type_flags & FLAG_DPOINTER) ? "*" : "");
        } else {
            fprintf(header, "%s %s%s", type_str[prototype->type],
                    (prototype->type_flags & FLAG_POINTER) ? "*" : "",
                    (prototype->type_flags & FLAG_DPOINTER) ? "*" : "");
        }

        if (!prototype->used)
            fprintf(header, "%s%s ",
                    (prototype->type_flags & FLAG_POINTER) ? " " : "",
                    unused_define);

        fprintf(header, "(*_%s%s)(", prototype->no_prefix ? "" : prefix, 
                prototype->name);

        if (prototype->num_params) {
            for (j = 0; j < prototype->num_params; j++) {
                param = &prototype->params[j];

                if (j && prototype->num_params > 1)
                    fprintf(header, ", ");

                if (param->type_flags & FLAG_CONST)
                    fprintf(header, "const ");

                if (param->type == TYPE_STRUCT) {
                    fprintf(header, "struct %s %s%s%s", param->struct_name,
                            (param->type_flags & FLAG_POINTER) ? "*" : "",
                            (param->type_flags & FLAG_DPOINTER) ? "*" : "",
                            param->name);
                } else {
                    fprintf(header, "%s %s%s%s", type_str[param->type],
                            (param->type_flags & FLAG_POINTER) ? "*" : "",
                            (param->type_flags & FLAG_DPOINTER) ? "*" : "",
                            param->name);
                }
            }
        } else {
            fprintf(header, "void");
        }

        fprintf(header, ");\n");
    }

    fclose(header);

    body = fopen(path_body, "w");
    if (!body)
        com_error(ERR_FATAL, "couldn't open file '%s'", path_body);

    fprintf(body, "#include \"script_local.h\"\n");
    fprintf(body, "#include \"script_redirects.h\"\n\n");

    for (i = 0; i < prototypes_count; i++) {
        prototype = &prototypes[i];
        if (prototype->type_flags & FLAG_CONST)
            fprintf(body, "const ");

        if (prototype->type == TYPE_STRUCT) {
            fprintf(body, "struct %s %s%s", prototype->struct_name,
                    (prototype->type_flags & FLAG_POINTER) ? "*" : "",
                    (prototype->type_flags & FLAG_DPOINTER) ? "*" : "");
        } else {
            fprintf(body, "%s %s%s", type_str[prototype->type],
                    (prototype->type_flags & FLAG_POINTER) ? "*" : "",
                    (prototype->type_flags & FLAG_DPOINTER) ? "*" : "");
        }

        if (!prototype->used)
            fprintf(body, "%s%s ",
                    (prototype->type_flags & FLAG_POINTER) ? " " : "",
                    unused_define);

        fprintf(body, "%s%s(", prototype->no_prefix ? "" : prefix, 
                prototype->name);

        if (prototype->num_params) {
            for (j = 0; j < prototype->num_params; j++) {
                param = &prototype->params[j];

                if (j && prototype->num_params > 1)
                    fprintf(body, ", ");

                if (param->type_flags & FLAG_CONST)
                    fprintf(body, "const ");

                if (param->type == TYPE_STRUCT) {
                    fprintf(body, "struct %s %s%s%s", param->struct_name,
                            (param->type_flags & FLAG_POINTER) ? "*" : "",
                            (param->type_flags & FLAG_DPOINTER) ? "*" : "",
                            param->name);
                } else {
                    fprintf(body, "%s %s%s%s", type_str[param->type],
                            (param->type_flags & FLAG_POINTER) ? "*" : "",
                            (param->type_flags & FLAG_DPOINTER) ? "*" : "",
                            param->name);
                }
            }
        } else {
            fprintf(body, "void");
        }

        fprintf(body, ")\n");
        fprintf(body, "{\n");

        if (prototype->type == TYPE_VOID && !(prototype->type_flags & FLAG_POINTER))
            fprintf(body, "    (*_%s%s)(", prototype->no_prefix ? "" : prefix, 
                    prototype->name);
        else
            fprintf(body, "    return (*_%s%s)(", 
                    prototype->no_prefix ? "" : prefix, prototype->name);

        if (prototype->num_params) {
            for (j = 0; j < prototype->num_params; j++) {
                param = &prototype->params[j];

                if (j && prototype->num_params > 1)
                    fprintf(body, ", ");

                fprintf(body, "%s", param->name);
            }
        }

        fprintf(body, ");\n");
        fprintf(body, "}\n\n");
    }

    fclose(body);
    printf("ok\n");
}

/**
 * @brief Put the data out to the correct files
*/
void output_prototypes(void)
{
    if (mkdir(output_directory, 0777) == -1) {
        if (errno != EEXIST)
            com_error(ERR_FATAL,
                      "error while making directory: %s", strerror(errno));
    }

    if (output_declarations_enabled)
        output_declarations();

    if (output_redirects_enabled)
        output_redirects();
}

/**
 * @brief main entry
*/
int main(int argc, char *argv[])
{
    data = fopenread("prototypes.txt");
    parse_prototypes();
    output_prototypes();
    free(data);

    printf("all done!\n");

    return 0;
}
