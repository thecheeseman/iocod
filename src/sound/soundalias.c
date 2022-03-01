#include "shared.h"
#include "common.h"

#define MAX_ALIAS_STRING_LEN	64
#define MAX_ALIAS_SUBTITLE_LEN	4096

struct soundalias
{
	char name[MAX_ALIAS_STRING_LEN];
	char alias[MAX_ALIAS_STRING_LEN];

	char subtitle[MAX_ALIAS_SUBTITLE_LEN];

	int sequence;
	char file[MAX_ALIAS_STRING_LEN];
	int a;
	float vol_min;
	float vol_max;
	float pitch_min;
	float pitch_max;
	float dist_min;
	float dist_max;
	int channel;
	int type;
	char loop_type;
	char master;
	char slave;

	char d;

	float master_slave;
	float probability;
	int load_spec;

	int g;

	struct soundalias *next;
};

enum {
	ALIAS_BLANK,
	ALIAS_NAME,
	ALIAS_SEQUENCE,
	ALIAS_FILE,
	ALIAS_SUBTITLE,
	ALIAS_VOL_MIN,
	ALIAS_VOL_MAX,
	ALIAS_PITCH_MIN,
	ALIAS_PITCH_MAX,
	ALIAS_DIST_MIN,
	ALIAS_DIST_MAX,
	ALIAS_CHANNEL,
	ALIAS_TYPE,
	ALIAS_LOOP,
	ALIAS_PROBABILITY,
	ALIAS_LOADSPEC,
	ALIAS_MASTERSLAVE,
	ALIAS_KEY_COUNT
};

static const char *alias_keys[] = {
	NULL,
	"name",
	"sequence",
	"file",
	"subtitle",
	"vol_min",
	"vol_max",
	"pitch_min",
	"pitch_max",
	"dist_min",
	"dist_max",
	"channel",
	"type",
	"loop",
	"probability",
	"loadspec",
	"masterslave"
};

static const char *alias_channels[] = {
	"auto",
	"menu",
	"weapon",
	"voice",
	"item",
	"body",
	"local",
	"music",
	"announcer",
	"shellshock"
};

static int alias_channels_count = sizeof(alias_channels) / sizeof(*alias_channels);

static char *alias_file;

static bool snd_validate_token(const char *token)
{
	return true;
}

/**
 * @brief 
 * @param token 
 * @return 
*/
static int snd_parse_alias_channel(const char *token)
{
	char channels[16384]; // wat
	int i;
	bool matched = false;

	// check if its in our list
	for (i = 0; i < alias_channels_count; i++) {
		if (q_stricmp(token, alias_channels[i])) {
			matched = true;
			break;
		}
	}

	if (matched)
		return i;

	sprintf(channels, "%s", alias_channels[0]);
	for (i = 1; i < alias_channels_count; i++) {
		strcat(channels, " ");
		strcat(channels, alias_channels[i]);
	}

	com_error(ERR_DROP, "Sound alias file %s: unknown sound channel '%s', " \
			  "should be %s", alias_file, token, channels);
	return 0;
}

/**
 * @brief 
 * @param token 
 * @return 
*/
static int snd_parse_alias_type(const char *token)
{
	if (!q_stricmp(token, "streamed"))
		return 2;
	else if (!q_stricmp(token, "loaded"))
		return 1;
	
	com_error(ERR_DROP, "Sound alias file %s: unknown sound type '%s' " \
			  "should be streamed or loaded", alias_file, token);
	return 0;
}

/**
 * @brief 
 * @param token 
 * @return 
*/
static int snd_parse_alias_loop_type(const char *token)
{
	if (!q_stricmp(token, "looping"))
		return 1;
	else if (!q_stricmp(token, "nonlooping"))
		return 0;

	com_error(ERR_DROP, "Sound alias file %s: unknown sound loop type '%s' " \
			  "should be looping or nonlooping", alias_file, token);
	return 0;
}

/**
 * @brief 
 * @param mapname 
 * @param token 
 * @return 
*/
#define TLEN 65536
static int snd_parse_alias_load_spec(const char *mapname, const char *token)
{
	int len;
	char t[TLEN];
	char *p, *pch;

	len = strlen(mapname);

	t[TLEN - 1] = '\0';
	strncpy(t, token, sizeof(t));
	if (t[TLEN - 1] != '\0')
		com_error(ERR_DROP, "Sound alias file %s: loadspec is > %i characters",
				  alias_file, TLEN - 1);

	q_strlwr(t);

	// inverse
	p = t;
	if (t[0] == '!') {
		while (*p != '\0' && *p != ' ') 
			p++;

		if (!strcmp(p, mapname)) {
			return 0;
		} else {
			if (!strcmp(p, "all_mp"))
				return 0;
			else
				return 1;
		}
	} else {
		pch = strstr(p, mapname);
		while (pch != NULL) {
			if (((pch == t) || (pch[-1] < '!')) && (pch[len] < '!'))
				return 1;

			p = pch + 1;
			pch = strstr(p, mapname);
		}

		if (!strcmp(p, "all_mp"))
			return 1;
		else
			return 0;
	}
}

/**
 * @brief 
 * @param token 
 * @param alias 
*/
static void snd_parse_alias_master_slave(const char *token, 
										 struct soundalias *alias)
{
	if (!q_stricmp(token, "master")) {
		alias->master = 1;
		alias->slave = 0;
	} else {
		alias->master = 0;
		alias->slave = 1;
		alias->master_slave = atof(token);
	}
}

void snd_parse_alias_line(const char *mapname, char *token, int id,
						  char *fields, struct soundalias *alias)
{
	int i;

	if (!id)
		return;

	if (fields[id])
		com_error(ERR_DROP, "Sound alias file %s: duplicate entry for the "\
				  "'%s' column", alias_file, alias_keys[id]);

	fields[id] = 1;

	switch (id) {
		case ALIAS_NAME:
			if (strlen(token) >= MAX_ALIAS_STRING_LEN - 1)
				com_error(ERR_DROP, "Sound alias file %s: alias name '%s' " \
						  "is longer than %i characters", alias_file, token,
						  MAX_ALIAS_STRING_LEN - 1);

			if (!snd_validate_token(token))
				com_error(ERR_DROP, "Sound alias file %s: alias name '%s' " \
						  "contains invalid characters", alias_file, token);

			strncpy(alias->alias, token, sizeof(alias->alias));
			break;
		case ALIAS_SEQUENCE:
			alias->sequence = atoi(token);
			break;
		case ALIAS_FILE:
			if (strlen(token) >= MAX_ALIAS_STRING_LEN - 1)
				com_error(ERR_DROP, "Sound alias file %s: sound file '%s' " \
						  "is longer than %i characters", alias_file, token,
						  MAX_ALIAS_STRING_LEN - 1);

			strncpy(alias->file, token, sizeof(alias->file));
			break;
		case ALIAS_SUBTITLE:
			if (strlen(token) >= MAX_ALIAS_SUBTITLE_LEN - 1)
				com_error(ERR_DROP, "Sound alias file %s: subtitle '%s' " \
						  "is longer than %i characters", alias_file, token,
						  MAX_ALIAS_SUBTITLE_LEN - 1);

			// verify only ascii characters are present
			for (i = 0; token[i]; i++) {
				if (token[i] < '\0') {
					com_error(ERR_DROP, "Sound alias file %s: subtitle '%s' " \
							  "has invalid character '%c' ascii %i", alias_file,
							  token, token[i], token[i]);
				}
			}

			strncpy(alias->subtitle, token, sizeof(alias->subtitle));
			break;
		case ALIAS_VOL_MIN:
			alias->vol_min = atof(token);

			if (!fields[ALIAS_VOL_MAX])
				alias->vol_max = alias->vol_min;
			break;
		case ALIAS_VOL_MAX:
			alias->vol_max = atof(token);
			break;
		case ALIAS_PITCH_MIN:
			alias->pitch_min = atof(token);

			if (!fields[ALIAS_PITCH_MAX])
				alias->pitch_max = alias->pitch_min;
			break;
		case ALIAS_PITCH_MAX:
			alias->pitch_max = atof(token);
			break;
		case ALIAS_DIST_MIN:
			alias->dist_min = atof(token);
			break;
		case ALIAS_DIST_MAX:
			alias->dist_max = atof(token);
			break;
		case ALIAS_CHANNEL:
			alias->channel = snd_parse_alias_channel(token);
			break;
		case ALIAS_TYPE:
			alias->type = snd_parse_alias_type(token);
			break;
		case ALIAS_LOOP:
			alias->loop_type = snd_parse_alias_loop_type(token);
			break;
		case ALIAS_PROBABILITY:
			alias->probability = atof(token);
			break;
		case ALIAS_LOADSPEC:
			alias->load_spec = snd_parse_alias_load_spec(mapname, token);
			break;
		case ALIAS_MASTERSLAVE:
			snd_parse_alias_master_slave(token, alias);
			break;
	}
}

/**
 * @brief 
 * @param map 
 * @param filename 
 * @param a 
 * @param b 
 * @return 
*/
int snd_parse_alias_file(const char *map, const char *filename, int a, int b)
{
	int len;
	char *buf, *buf_p, *token;

	len = fs_read_file(filename, (void **) &buf);
	if (len < 0)
		return a;

	com_begin_parse_session(filename);
	com_set_field_0x40c(true);

	token = com_parse(&buf_p);

	if (buf_p) {

	}

	com_end_parse_session();
	return a;
}
