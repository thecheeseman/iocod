#include <stdlib.h>

#include "iocod.h"

static const char *punctuation[] = {
    "+=", "-=",  "*=",  "/=", "&=", "|=", "++", "--",
    "&&", "||",  "<=",  ">=", "==", "!=",
    NULL
};

extern struct parse_info *pi;

/* roll forward until we encounter a non-whitespace char */
static char *skip_whitespace(char *data, bool *new_lines)
{
    char c;
    while ((c = *data) <= ' ') {
        if (c == '\0')
            return NULL;

        if (c == '\n') {
            pi->lines++;

            if (!(*new_lines))
                *new_lines = true;
        }

        data++;
    }

    return data;
}

static char *parse_ext2(char **data_p, bool line_breaks)
{
    char *c = *data_p;

    pi->token[0] = '\0';

    if (!line_breaks) {
        if (*c == '\r' || *c == '\n')
            return pi->token;
    } else {
        for (; (*c == '\r') || (*c == '\n'); c++) {
            /* loop until we encounter a line feed */
        }
    }

    return NULL;
}

static char *parse_ext(char **data_p, bool line_breaks)
{
    if (data_p == NULL) {
        ic_error("NULL data_p");
        return NULL;
    }

    char *data = *data_p;
    size_t len = 0;
    pi->token[0] = '\0';

    /* make sure incoming data is valid */
    if (data == NULL) {
        *data_p = NULL;
        goto out;
    }

    pi->old_lines = pi->lines;
    pi->buf_p = *data_p;

    /* skip any leading whitespace or comments */
    bool new_lines = false;
    char c;
    while (true) {
        data = skip_whitespace(data, &new_lines);
        if (data == NULL) {
            *data_p = NULL;
            goto out;
        }

        /* encountered a new line, but we don't want to check past linebreaks */
        if (new_lines && !line_breaks)
            goto end;

        c = *data;

        /* skip C++ style comments */
        if (c == '/' && data[1] == '/') {
            while (*data != '\0' && *data != '\n')
                data++;

            pi->lines++;
            continue;
        }

        /* skip C style comments */
        if (c == '/' && data[1] == '*') {
            while (*data != '\0' && (*data != '*' || data[1] != '/')) {
                if (*data == '\n')
                    pi->lines++;

                data++;
            }

            if (*data != '\0')
                data += 2;

            continue;
        }

        /* made it this far, now we have a real token we can parse */
        break;
    }

    /* handle quoted strings */
    if (data[0] == '\"') {
        data++;

        while (true) {
            c = *data++;

            if ((c == '\\') && (*data == '\"')) {
                /* allow quoted strings to use \" to indicate " character */
                data++;
            } else if (c == '\"' || c == '\0') {
                pi->token[len] = '\0';

                goto end;
            } else if (*data == '\n') {
                pi->lines++;
            }

            if (len < MAX_TOKEN_CHARS - 1) {
                pi->token[len] = c;
                len++;
            }
        }
    }

    /* check for numbers */
    if ((c >= '0' && c <= '9') || 
        (c == '-' && data[1] >= '0' && data[1] <= '9') ||
        (c == '.' && data[1] >= '0' && data[1] <= '9')) {
        do {
            if (len < MAX_TOKEN_CHARS - 1) {
                pi->token[len] = c;
                len++;
            }
            data++;

            c = *data;
        } while ((c >= '0' && c <= '9') || c == '.');

        /* parse the exponent */
        if (c == 'e' || c == 'E') {
            if (len < MAX_TOKEN_CHARS - 1) {
                pi->token[len] = c;
                len++;
            }

            data++;
            c = *data;

            if (c == '-' || c == '+') {
                if (len < MAX_TOKEN_CHARS - 1) {
                    pi->token[len] = c;
                    len++;
                }
                data++;
                c = *data;
            }

            do {
                if (len < MAX_TOKEN_CHARS - 1) {
                    pi->token[len] = c;
                    len++;
                }
                data++;

                c = *data;
            } while (c >= '0' && c <= '9');
        }

        if (len == MAX_TOKEN_CHARS)
            len = 0;

        pi->token[len] = '\0';

        goto end;
    }

    /* 
     * check for a regular word
     * we still allow forward and back slashes in name tokens for pathnames
     * and also colons for drive letters
     */
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' ||
        c == '/' || c == '\\') {
        do {
            if (len < MAX_TOKEN_CHARS - 1) {
                pi->token[len] = c;
                len++;
            }
            data++;

            c = *data;
        } while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'
                 || (c >= '0' && c <= '9') || c == '/' || c == '\\'
                 || c == ':' || c == '.');

        if (len == MAX_TOKEN_CHARS)
            len = 0;

        pi->token[len] = '\0';

        goto end;
    }

    /* check for multi - character punctuation token */
    for (const char **punc = punctuation; *punc; punc++) {
        size_t j;

        size_t l = strlen(*punc);
        for (j = 0; j < l; j++) {
            if (data[j] != (*punc)[j])
                break;
        }

        if (j == l) {
            /* a valid multi - character punctuation */
            memcpy(pi->token, *punc, l);
            pi->token[l] = 0;
            data += l;

            goto end;
        }
    }

    /* single character punctuation */
    pi->token[0] = *data;
    pi->token[1] = '\0';
    data++;
    
end:
    *data_p = data;
out:
    return pi->token;
}

IC_PUBLIC
IC_NON_NULL(1)
char *ps_parse(char **buf_p)
{
    if (pi->unget_token) {
        pi->unget_token = false;

        *buf_p = pi->buf_p;
        pi->lines = pi->old_lines;
    }

    return parse_ext(buf_p, true);
}

IC_PUBLIC
IC_NON_NULL(1)
char *ps_parse_on_line(char **buf_p)
{
    if (pi->unget_token) {
        pi->unget_token = false;

        *buf_p = pi->buf_p;
        pi->lines = pi->old_lines;
    }

    return parse_ext(buf_p, false);
}

IC_PUBLIC
IC_NON_NULL(1, 2)
void ps_match_token(char **buf_p, char *match, bool warning)
{
    char *token = ps_parse(buf_p);

    if (strcmp(token, match) != 0) {
        if (warning)
            ps_script_warning("match token: '%s' != '%s'", token, match);
        else
            ps_script_error("match token: '%s' != '%s'", token, match);
    }
}

IC_PUBLIC
IC_NON_NULL(1)
void ps_skip_braced_section(char **buf_p)
{
    int depth = 0;

    do {
        const char *token = ps_parse(buf_p);

        if (token[1] == '\0') {
            if (token[0] == '{')
                depth++;
            else if (token[0] == '}')
                depth--;
        }
    } while (depth && *buf_p != NULL);
}

IC_PUBLIC
IC_NON_NULL(1)
void ps_skip_rest_of_line(char **data_p)
{
    char *p = *data_p;
    char c;

    while ((c = *p++) != '\0') {
        if (c == '\n') {
            pi->lines++;
            break;
        }
    }

    *data_p = p;
}

IC_PUBLIC
IC_NON_NULL(1)
char *ps_parse_rest_of_line(char **data_p)
{
    static char	line[MAX_TOKEN_CHARS];
    const char *token;

    line[0] = '\0';
    while (true) {
        token = ps_parse_on_line(data_p);
        if (token[0] == '\0')
            break;

        if (line[0] != '\0')
            strncatz(line, sizeof(line), " ");

        strncatz(line, sizeof(line), token);
    }

    return line;
}

IC_PUBLIC
IC_NON_NULL(1)
double ps_parse_value(char **buf_p)
{
    const char *token = ps_parse(buf_p);

    if (token[0] == '\0')
        return 0;

    return strtod(token, NULL);
}

IC_PUBLIC
IC_NON_NULL(1)
long ps_parse_integer(char **buf_p)
{
    const char *token = ps_parse(buf_p);

    if (token[0] == '\0')
        return 0;

    return strtol(token, NULL, 10);
}

IC_PUBLIC
IC_NON_NULL(1, 3)
void ps_parse_1d_matrix(char **buf_p, size_t x, vec_t *m)
{
    ps_match_token(buf_p, "(", false);

    for (size_t i = 0; i < x; i++) {
        const char *token = ps_parse(buf_p);
        m[i] = strtod(token, NULL);
    }

    ps_match_token(buf_p, ")", false);
}

IC_PUBLIC
IC_NON_NULL(1, 4)
void ps_parse_2d_matrix(char **buf_p, size_t y, size_t x, vec_t *m)
{
    ps_match_token(buf_p, "(", false);

    for (size_t i = 0; i < y; i++)
        ps_parse_1d_matrix(buf_p, x, m + i * x);

    ps_match_token(buf_p, ")", false);
}

IC_PUBLIC
IC_NON_NULL(1, 5)
void ps_parse_3d_matrix(char **buf_p, size_t z, size_t y, size_t x, vec_t *m)
{
    ps_match_token(buf_p, "(", false);

    for (size_t i = 0; i < z; i++)
        ps_parse_2d_matrix(buf_p, y, x, m + i * x * y);

    ps_match_token(buf_p, ")", false);
}

IC_PUBLIC
void ps_unget_token(void)
{
    if (pi->unget_token) {
        ps_script_error("unget_token called twice");
        return;
    }

    pi->unget_token = true;
}
