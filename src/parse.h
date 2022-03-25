#ifndef PARSE_H
#define PARSE_H

#include "types/bool.h"

void com_begin_parse_session(const char *name);
void com_end_parse_session(void);
void com_reset_parse_session(void);
void com_set_field_0x40c(bool val);
int com_get_current_parse_line(void);
void com_script_error(const char *msg, ...);
void com_script_warning(const char *msg, ...);
void com_unget_token(void);
char *com_parse(char **buf_p);
char *com_parse_on_line(char **buf_p);
void com_match_token(char **buf_p, char *match, bool warning);
void com_skip_braced_section(char **program);
void com_skip_rest_of_line(char **data_p);
char *com_parse_rest_of_line(char **data_p);
float com_parse_float(char **buf_p);
int com_parse_int(char **buf_p);
void com_parse_1d_matrix(char **buf_p, int x, float *m);
void com_parse_2d_matrix(char **buf_p, int y, int x, float *m);
void com_parse_3d_matrix(char **buf_p, int z, int y, int x, float *m);

#endif /* PARSE_H */
