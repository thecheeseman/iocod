#include "script_local.h"

struct game_script_data g_scr_data;

// temp
void scr_set_entity_field(int a, int b)
{
	UNUSED(a);
	UNUSED(b);
}

void scr_get_entity_field(int a, int b)
{
	UNUSED(a);
	UNUSED(b);
}

void scr_set_hud_elem_field(int a, int b)
{
	UNUSED(a);
	UNUSED(b);
}

void scr_get_hud_elem_field(int a, int b)
{
	UNUSED(a);
	UNUSED(b);
}
// temp

struct gsc_function *scr_get_function(const char **name, int *developer)
{
	UNUSED(name);
	UNUSED(developer);

	return NULL;
}

struct gsc_function *scr_get_method(const char **name, int *developer)
{
	UNUSED(name);
	UNUSED(developer);

	return NULL;
}

void scr_set_object_field(int a, int b, int c)
{
	if (a == 0)
		scr_set_entity_field(b, c);
	else if (a == 1)
		scr_set_hud_elem_field(b, c);
}

void scr_get_object_field(int a, int b, int c)
{
	if (a == 0)
		scr_get_entity_field(b, c);
	else if (a == 1)
		scr_get_hud_elem_field(b, c);
}

int scr_load_read(int a)
{
	UNUSED(a);

	return 0;
}

// these just hold the memory addresses of their respective functions
// so the exe can memcpy these addresses over its own function redirects
struct gsc_function *(*_scr_get_function)(const char **name, int *developer);
struct gsc_function *(*_scr_get_method)(const char **name, int *developer);
void (*_scr_set_object_field)(int a, int b, int c);
void (*_scr_get_object_field)(int a, int b, int c);
int (*_scr_load_read)(int a);
//

// defined in script_redirects.c
extern bool (*_scr_get_bool)(int32_t index);

/**
 * @brief Allows us to link functions across the game dll and exe
*/
intptr_t scr_far_hook(intptr_t addr)
{
	if (addr)
		memcpy(&_scr_get_bool, &addr, sizeof(intptr_t) * 102);

	_scr_get_function = scr_get_function;
	_scr_get_method = scr_get_method;
	_scr_set_object_field = scr_set_object_field;
	_scr_get_object_field = scr_get_object_field;
	_scr_load_read = scr_load_read;

	return (intptr_t) &_scr_get_function;
}

#define MAX_DESCRIPTION_LENGTH 1024
void scr_parse_gametype_list(void)
{
	int nfiles, i, len, tlen, gtnamelen, num_gametypes, flen;
	char list[4096], description[MAX_DESCRIPTION_LENGTH];
	char *file, *filename, *token, *data;
	struct gametype *gametype;
	filehandle fp;

	memset(g_scr_data.gametypes, 0, sizeof(g_scr_data.gametypes));
	nfiles = trap_fs_get_file_list("maps/mp/gametypes", "gsc", list, 
								   sizeof(list));

	num_gametypes = 0;
	file = list;
	for (i = 0; i < nfiles; i++) {
		len = strlen(file);

		// skip _gsc files as they're not gametypes
		if (file[0] == '_') {
			file += len + 1;
			continue;
		}

		// strip extension
		if (q_stricmp(file + (len - 4), ".gsc") == 0)
			file[len - 4] = '\0';

		if (num_gametypes == MAX_GAMETYPES) {
			g_printf("Too many gametypes found, only loading the first %d\n",
					 MAX_GAMETYPES);
			g_scr_data.num_gametypes = num_gametypes;
			return;
		}

		gametype = &g_scr_data.gametypes[num_gametypes];

		// load name and lowercase it
		gtnamelen = strlen(file);
		gametype->name = trap_hunk_alloc_low_internal(gtnamelen);
		q_strncpyz(gametype->name, file, gtnamelen + 1);
		q_strlwr(gametype->name);

		// description file
		filename = va("maps/mp/gametypes/%s.txt", file);
		flen = trap_fs_fopen_file(filename, &fp, FS_READ);
		if (flen < 1 || flen > MAX_DESCRIPTION_LENGTH) {
			if (flen < 1) {
				g_printf("WARNING: Could not load gametype description file " \
						 "'%s' for gametype '%s'\n", filename, file);
			} else {
				g_printf("WARNING: Gametype description file '%s' " \
						 "is too big to load\n", filename);
			}

			gametype->description = gametype->name;
			gametype->team_based = false;
		} else {
			// this section is _basically_ useless for cod1
			// but it does this anyway
			memset(description, 0, MAX_DESCRIPTION_LENGTH);

			trap_fs_read(description, flen, fp);
			data = description;
			token = com_parse(&data);

			tlen = strlen(token);
			gametype->description = trap_hunk_alloc_low_internal(tlen);
			q_strncpyz(gametype->description, token, tlen + 1);

			token = com_parse(&data);
			if (token != NULL && q_stricmp(token, "team") == 0)
				gametype->team_based = true;
			else
				gametype->team_based = false;
		}

		if (flen)
			trap_fs_fclose_file(fp);

		num_gametypes++;
		file += len + 1;
	}
}

void scr_load_gametype(void)
{

}

void scr_load_level(void)
{

}

void scr_startup_gametype(void)
{

}