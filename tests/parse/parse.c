#include "testlib.h"

// just only compare 2 digits of precision
#define FEQUAL(A, B) (fabs(A - B) < 0.01)

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
 * @brief test basic parsing functionality
*/
void test_functionality(void)
{
	char *p, *data, *token;

	data = fopenread("test.txt");
	if (!data)
		return;

	p = data;

	//
	// testing
	//
	com_begin_parse_session("test.txt");

	// integer
	assert(com_parse_int(&p) == 1);

	// float
	assert(FEQUAL(com_parse_float(&p), 3.14));

	// one word string
	token = com_parse(&p);
	assert(!strcmp(token, "text"));

	// quoted string
	token = com_parse(&p);
	assert(!strcmp(token, "this is a longer text"));

	// 1d matrix
	float vec[3];
	com_parse_1d_matrix(&p, 3, vec);

	assert(FEQUAL(vec[0], 1.45));
	assert(FEQUAL(vec[1], 2.99));
	assert(FEQUAL(vec[2], 3.64));

	// string on same line
	token = com_parse(&p);
	assert(!strcmp(token, "stand"));

	// ignore entire braced section
	com_skip_braced_section(&p);

	// string
	com_match_token(&p, "true", false);

	com_end_parse_session();
	//
	// testing
	//

	free(data);
	printf("test passed\n");
}

/**
 * @brief find a particular localized string in a given file
 * @param  
*/
char *get_localized_string(const char *match)
{
	char *data, *p, *token;
	char ref[1024];
	char *str = NULL;

	data = fopenread("exe.str");
	if (!data)
		return NULL;

	p = data;

	com_begin_parse_session("exe.str");

	while ((token = com_parse(&p)) && p != NULL) {
		if (strcmp(token, "REFERENCE") != 0) {
			com_skip_rest_of_line(&p);
			continue;
		}

		token = com_parse_on_line(&p);
		strncpy(ref, token, sizeof(ref));
		com_skip_rest_of_line(&p);

		do {
			token = com_parse(&p);
			if (!p) {
				com_error(ERR_FATAL, "stringed file exe.str has bad syntax");
				goto end;
			}
		} while (strcmp(token, "LANG_ENGLISH") != 0);

		token = com_parse_on_line(&p);

		if (!q_stricmp(match, ref)) {
			str = copy_string(token);
			goto end;
		}
	}

end:
	com_end_parse_session();
	free(data);
	return str;
}

void test_localized_strings(void)
{
	char *str;
	//
	// testing
	//
	str = get_localized_string("YES");
	assert(str != NULL);
	assert(!strcmp(str, "Yes"));
	
	str = get_localized_string("GAMESAVED");
	assert(str != NULL);
	assert(!strcmp(str, "Game Saved"));

	str = get_localized_string("INVALIDUPDATESERVERDOWNLOAD");
	assert(str != NULL);
	assert(!strcmp(str, "Invalid download from update server"));

	str = get_localized_string("SV_INFO_PURE");
	assert(str != NULL);
	assert(!strcmp(str, "Pure Server"));
	//
	// testing
	//

	printf("test passed\n");
}

int main(int argc, char *argv[])
{
	test_functionality();
	test_localized_strings();

	return 0;
}
