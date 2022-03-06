/*
================================================================================
iocod
Copyright (C) 2021-2022 thecheeseman

This file is part of the iocod GPL source code.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
================================================================================
*/

/**
 * @file files.c
 * @date 2022-02-04
*/

#include "files_local.h"

char fs_gamedir[MAX_OSPATH];

struct cvar *fs_debug;
#define FS_DEBUG_PRINT(...) if (fs_debug->integer) { com_printf_dbginfo(__VA_ARGS__); }

struct cvar *fs_copyfiles;
struct cvar *fs_cdpath;
struct cvar *fs_basepath;
struct cvar *fs_basegame;
struct cvar *fs_homepath;
struct cvar *fs_gamedirvar;
struct cvar *fs_restrict;
struct cvar *fs_ignorelocalized;

int fs_readcount;
int fs_loadcount;
int _fs_loadstack;
int fs_packfiles;
int fs_fake_checksum;
int fs_checksumfeed;
struct searchpath *fs_searchpaths;
struct searchpath *fs_searchpaths2;

struct filehandledata fsh[MAX_FILE_HANDLES];

int fs_num_serverpaks;
int fs_serverpaks[MAX_SEARCH_PATHS]; // checksums
char *fs_server_paknames[MAX_SEARCH_PATHS]; // pk3 names

int fs_num_server_referencedpaks;
int fs_server_referencedpaks[MAX_SEARCH_PATHS]; // checksums
char *fs_server_referenced_paknames[MAX_SEARCH_PATHS]; // pk3 names

char lastvalidbase[MAX_OSPATH];
char lastvalidgame[MAX_OSPATH];

bool fs_initialized(void)
{
	return (fs_searchpaths != NULL);
}

void fs_clear_loadstack(void)
{
	_fs_loadstack = 0;
}

int fs_loadstack(void)
{
	return _fs_loadstack;
}

/**
 * @brief Check if a given pk3 file is pure
 * @author idsoftware
 * @param pack PK3 file to check
 * @return True if pure, false otherwise
*/
bool fs_pak_is_pure(struct pack *pack)
{
	int i;
	
	if (fs_num_serverpaks > 0) {
		for (i = 0; i < fs_num_serverpaks; i++) {
			if (pack->checksum == fs_serverpaks[i])
				return true; // approved
		}

		return false; // not on the pure server pak list
	}

	return true;
}

/**
 * @brief Return a hash value for the filename
 * @author idsoftware
 * @param fname File name
 * @param hashsize Hash size
 * @return A hash
*/
uint32_t fs_hash_filename(const char *fname, int hashsize)
{
	int i;
	uint32_t hash; // changed from long to maintain 32bit compatibility
	char letter;

	hash = 0, i = 0;
	while (fname[i] != '\0') {
		letter = tolower(fname[i]);
		if (letter == '.') // don't include extension
			break;

		if (letter == '\\')
			letter = '/';
		if (letter == PATH_SEP)
			letter = '/';

		hash += (uint32_t) (letter) * (i + 119);
		i++;
	}

	// q3 hash
	hash = hash ^ ((hash >> 10) & (hash >> 20));
	hash &= (hashsize - 1);
	return hash;

	// 1.5 hash:
	// return ((hash >> 20 ) ^ (hash >> 10) ^ hash) & hashsize - 1;

	// 1.1 hash:
	// return ((hash >> 10) ^ hash ^ (hash >> 20)) & hashsize - 1;
}

/**
 * @brief 
 * @param  
 * @return 
*/
filehandle fs_handle_for_file(void)
{
	int i;

	for (i = 1; i < MAX_FILE_HANDLES; i++) {
		if (fsh[i].handlefiles.file.o == NULL)
			return i;
	}

	com_error(ERR_DROP, "fs_handle_for_file: none free");
	return 0;
}

/**
 * @brief 
 * @param f 
 * @return 
*/
FILE *fs_file_for_handle(filehandle f)
{
	if (f < 0 || f > MAX_FILE_HANDLES)
		com_error(ERR_DROP, "fs_file_for_handle: out of range");

	if (fsh[f].zipfile == true)
		com_error(ERR_DROP, "fs_file_for_handle: can't get FILE on zip file");

	if (!fsh[f].handlefiles.file.o)
		com_error(ERR_DROP, "fs_file_for_handle: NULL");

	return fsh[f].handlefiles.file.o;
}

/**
 * @brief 
 * @param f 
*/
void fs_force_flush(filehandle f)
{
	FILE *file = fs_file_for_handle(f);
	setvbuf(file, NULL, _IONBF, 0);
}

/**
 * @brief Get file length. If this is called on a non unique file (from a pk3)
 * it will return the size of the pk3 file, not the actual file.
 * @param f File handle
 * @return Size of file
*/
int fs_file_length(filehandle f)
{
	int pos, end;
	FILE *h;

	h = fs_file_for_handle(f);
	pos = ftell(h);
	fseek(h, 0, SEEK_END);
	end = ftell(h);
	fseek(h, pos, SEEK_SET);

	return end;
}

/**
 * @brief Fix paths for different operating systems
 * @param path 
*/
void fs_replace_separators(char *path)
{
	char *s;

	for (s = path; *s != '\0'; s++) {
		if (*s == '/' || *s == '\\')
			*s = PATH_SEP;
	}
}

/**
 * @brief More accurate 1.1 fs_build_ospath
*/
void fs_build_ospath(const char *base, const char *game, const char *qpath, char *buf)
{
	int baselen, gamelen, qpathlen;

	if (game == NULL || *game == '\0')
		game = fs_gamedir;

	baselen = strlen(base);
	gamelen = strlen(game);
	qpathlen = strlen(qpath);

	if ((baselen + gamelen + qpathlen) > MAX_OSPATH)
		com_error(ERR_FATAL, "os path length exceeded");

	memcpy(buf, base, baselen);
	buf[baselen] = '/';
	memcpy(buf + baselen + 1, game, gamelen);
	buf[baselen + gamelen + 1] = '/';
	memcpy(buf + gamelen + baselen + 2, qpath, qpathlen + 1);

	fs_replace_separators(buf);
}

/**
 * @brief Create a given path
*/
bool fs_create_path(char *ospath)
{
	char *ofs;
	if (strstr(ospath, "..") != NULL || strstr(ospath, "::") != NULL) {
		com_warning("refusing to create relative path \"%s\"\n", ospath);
		return true;
	}

	for (ofs = ospath + 1; *ofs; ofs++) {
		if (*ofs == PATH_SEP) {
			// create the directory
			*ofs = 0;
			sys_mkdir(ospath);
			*ofs = PATH_SEP;
		}
	}

	return false;
}

/**
 * @brief 
 * @param s1 
 * @param s2 
 * @return 
*/
bool fs_filename_compare(const char *s1, const char *s2) 
{
	int c1, c2;

	do {
		c1 = *s1++;
		c2 = *s2++;

		if (c1 >= 'a' && c1 <= 'z')
			c1 -= ('a' - 'A');

		if (c2 >= 'a' && c2 <= 'z')
			c2 -= ('a' - 'A');

		if (c1 == '\\' || c1 == ':')
			c1 = '/';

		if (c2 == '\\' || c2 == ':')
			c2 = '/';

		if (c1 != c2)
			return -1;      // strings not equal
	} while (c1);

	return 0;       // strings are equal
}

/**
 * @brief 
 * @param string 
 * @param substring 
 * @param shift 
 * @return 
*/
char *fs_shifted_strstr(const char *string, const char *substring, int shift) 
{
	char buf[MAX_STRING_TOKENS];
	int i;

	for (i = 0; substring[i] != '\0'; i++)
		buf[i] = substring[i] + shift;

	buf[i] = '\0';
	return strstr(string, buf);
}

/**
 * @brief Perform simple string shifting to avoid scanning from the exe
 * @param string 
 * @param shift 
 * @return 
*/
char *fs_shift_str(const char *string, int shift) 
{
	static char buf[MAX_STRING_CHARS];
	int i, l;

	l = strlen(string);
	for (i = 0; i < l; i++)
		buf[i] = string[i] + shift;

	buf[i] = '\0';
	return buf;
}

/**
 * @brief best guess for what this is 
 * @param search 
 * @return 
*/
bool fs_search_localized(struct searchpath *search)
{
	if (search->localized || fs_ignorelocalized->integer == 0)
		return true;

	return false;
}

void fun_08061070(char *a, char *b, const char *c, const char *d)
{
	(void) a; (void) b; (void) c; (void) d; //-Werror=unusued-parameter
}

/**
 * @brief
 * @param folder 
 * @param ext 
*/
static void fs_load_folder_data(const char *folder, const char *ext)
{
	struct searchpath *p;

	for (p = fs_searchpaths; p != NULL; p = p->next) {
		if (p->pack == NULL)
			fun_08061070(p->dir->path, p->dir->gamedir, folder, ext);
	}
}

/**
 * @brief 
 * @param gamename 
*/
static void fs_startup(const char *gamename)
{
	const char *homepath;
	struct cvar *fs;

	com_print_header("fs_startup", 20, '-');

	fs_debug = cvar_get("fs_debug", "1", 0);
	fs_copyfiles = cvar_get("fs_copyfiles", "0", CVAR_INIT);
	fs_cdpath = cvar_get("fs_cdpath", sys_default_cd_path(), CVAR_INIT);
	fs_basepath = cvar_get("fs_basepath", sys_default_install_path(), CVAR_INIT);
	fs_basegame = cvar_get("fs_basegame", "", CVAR_INIT);
	homepath = sys_default_home_path();
	if (homepath == NULL || *homepath == '\0')
		homepath = fs_basepath->string;
	fs_homepath = cvar_get("fs_homepath", homepath, CVAR_INIT);
	fs_gamedirvar = cvar_get("fs_game", "", CVAR_INIT | CVAR_SYSTEM_INFO);
	fs_restrict = cvar_get("fs_restrict", "", CVAR_INIT);
	fs_ignorelocalized = cvar_get("fs_ignorelocalized", "0", 
								  CVAR_CHEAT | CVAR_LATCH);

	// cdpath
	if (*fs_cdpath->string != '\0')
		fs_add_game_directory(fs_cdpath->string, gamename);

	// basepath
	if (*fs_basepath->string != '\0') {
		fs_add_game_directory(fs_basepath->string, gamename);

		// *nix check
		if (*fs_homepath->string != '\0' && 
			q_stricmp(fs_homepath->string, fs_basepath->string) != 0)
			fs_add_game_directory(fs_homepath->string, gamename);
	}

	// check for additional base game so mods and be based upon other mods
	if (*fs_basegame->string != '\0' && 
		q_stricmp(gamename, BASEGAME) == 0 &&
		q_stricmp(fs_basegame->string, gamename) != 0) {

		// cdpath
		if (*fs_cdpath->string != '\0')
			fs_add_game_directory(fs_cdpath->string, fs_basegame->string);

		// basepath
		if (*fs_basepath->string != '\0') {
			fs_add_game_directory(fs_basepath->string, fs_basegame->string);

			// *nix check
			if (*fs_homepath->string != '\0' && 
				q_stricmp(fs_homepath->string, fs_basepath->string) != 0)
				fs_add_game_directory(fs_homepath->string, fs_basegame->string);
		}
	}

	// check additional game folder for mods
	if (*fs_gamedirvar->string != '\0' && 
		q_stricmp(gamename, BASEGAME) == 0 &&
		q_stricmp(fs_gamedirvar->string, gamename) != 0) {

		// cdpath
		if (*fs_cdpath->string != '\0')
			fs_add_game_directory(fs_cdpath->string, fs_gamedirvar->string);

		// basepath
		if (*fs_basepath->string != '\0') {
			fs_add_game_directory(fs_basepath->string, fs_gamedirvar->string);

			// *nix check
			if (*fs_homepath->string != '\0' &&
				q_stricmp(fs_homepath->string, fs_basepath->string) != 0)
				fs_add_game_directory(fs_homepath->string, fs_gamedirvar->string);
		}
	}

	fs_load_folder_data("xanim", "");
	fs_load_folder_data("xmodel", "");
	fs_load_folder_data("xmodelparts", "");
	fs_load_folder_data("xmodelsurfs", "");
	fs_load_folder_data("weapons", "");
	fs_load_folder_data("animtrees", ".atr");

	com_read_cdkey(BASEGAME);
	fs = cvar_get("fs_game", "", CVAR_INIT | CVAR_SYSTEM_INFO);
	if (fs != NULL && *fs->string != '\0')
		com_append_cdkey(fs->string);

	fs_add_commands();

	// diplay search path (cmd path)
	fs_path(false);

	fs_gamedirvar->modified = false;

	com_print_header("", 20, '-');
	com_printf("%d files in pk3 files\n", fs_packfiles);
}

int fs_path_compare(const char *s1, const char *s2) 
{
	int c1, c2;

	do {
		c1 = *s1++;
		c2 = *s2++;

		if (c1 >= 'a' && c1 <= 'z')
			c1 -= ('a' - 'A');
		if (c2 >= 'a' && c2 <= 'z')
			c2 -= ('a' - 'A');

		if (c1 == '\\' || c1 == ':') 
			c1 = '/';
		if (c2 == '\\' || c2 == ':')
			c2 = '/';

		if (c1 < c2)
			return -1;      // strings not equal

		if (c1 > c2)
			return 1;

	} while (c1);

	return 0;       // strings are equal
}

static int fs_localized_pakid = 0;
static char fs_localized_paks[2][MAX_QPATH];

/**
 * @brief 
 * @param pakname 
 * @return 
*/
static char *fs_get_localized_language_name(const char *pakname)
{
	int i;
	fs_localized_pakid ^= 1;

	if (strlen(pakname) < 10) {
		fs_localized_paks[fs_localized_pakid][0] = '\0';
	} else {
		memset(fs_localized_paks[fs_localized_pakid], 0, MAX_QPATH);

		for (i = 10; i < MAX_QPATH && pakname[i] != '\0' && pakname[i] != '_'; i++)
			fs_localized_paks[fs_localized_pakid][i - 10] = pakname[i];
	}

	return fs_localized_paks[fs_localized_pakid];
}

static int paksort(const void *a, const void *b)
{
	char *aa, *bb;
	char *l1, *l2;

	aa = *(char **) a;
	bb = *(char **) b;

	// check for localize files
	if (strncmp(aa, "          ", 10) == 0 && 
		strncmp(bb, "          ", 10) == 0) {
		l1 = fs_get_localized_language_name(aa);
		l2 = fs_get_localized_language_name(bb);

		if (q_stricmp(l1, "english") == 0 && q_stricmp(l2, "english") != 0)
			return -1;
		else if (q_stricmp(l1, "english") != 0 && q_stricmp(l2, "english") == 0)
			return 1;
	}

	return fs_path_compare(aa, bb);
}

/**
 * @brief reordering something
 * @param sp
*/
static void fs_update_search_paths(struct searchpath *sp)
{
	struct searchpath *p;

	if (!sp->localized || fs_searchpaths == NULL) {
		sp->next = fs_searchpaths;
		fs_searchpaths = sp;
	} else {

		for (p = fs_searchpaths; p->next != NULL && !p->next->localized; p = p->next);

		sp->next = p->next;
		p->next = sp;
	}
}

#define MAX_PAKFILES 1024
static void fs_find_pack_files(const char *path, const char *dir)
{
	char ospath[MAX_OSPATH];
	int i, numfiles;
	char **pakfiles;
	char *sorted[MAX_PAKFILES];
	struct pack *pak;
	struct searchpath *search;
	bool localized;

	fs_build_ospath(path, dir, "", ospath);
	ospath[strlen(ospath) - 1] = '\0';

	pakfiles = sys_list_files(ospath, ".pk3", NULL, &numfiles, false);
	if (numfiles > MAX_PAKFILES) {
		com_warning("Exceeded max number of pak files in %s/%s (%1/%1)\n", 
			path, dir, numfiles, MAX_PAKFILES);
		numfiles = MAX_PAKFILES; // cap at max
	}

	// sort them so that localized gets loaded first
	for (i = 0; i < numfiles; i++) {
		sorted[i] = pakfiles[i];
		
		if (q_strncmp(sorted[i], "localized_", 10) == 0)
			memcpy(sorted[i], "          ", 10);
	}

	// original: qsort(sorted, numfiles, 4, paksort)
	// since sizeof(char *) = 4 bytes on 32 bit systems
	// but   sizeof(char *) = 8 bytes on 64 bit systems
	qsort(sorted, numfiles, sizeof(char *), paksort); 

	for (i = 0; i < numfiles; i++) {
		localized = false;

		if (q_strncmp(sorted[i], "          ", 10) == 0) {
			memcpy(sorted[i], "localized_", 10);

			localized = true;

			char *lname = fs_get_localized_language_name(sorted[i]);
			if (lname == NULL) {
				com_warning("Localized assets pak file %s/%s/%s " \
							"has invalid name(no language specified). " \
							"Proper naming convention is : " \
							"localized_[language]_pak#.pk3\n",
					path, dir, sorted[i]);
				continue;
			}

			// skip non-english files
			if (q_stricmp(lname, "english") != 0)
				continue;
		}

		fs_build_ospath(path, dir, sorted[i], ospath);
		if ((pak = fs_load_zip_file(ospath, sorted[i])) == NULL)
			continue;

		strcpy(pak->pak_gamename, dir);

		search = z_malloc(sizeof(struct searchpath));
		search->pack = pak;
		search->localized = localized;
		search->b = 0;
		fs_update_search_paths(search);
	}

	sys_free_file_list(pakfiles);
}

/**
 * @brief 
 * @param path 
 * @param dir 
 * @param localized 
 * @param d 
*/
static void fs_add_game_directory_real(const char *path, const char *dir, 
									   int localized, int d) // d = language?
{
	struct searchpath *sp, *search;
	struct directory *directory;
	char newdir[MAX_OSPATH], ospath[MAX_OSPATH];

	if (localized)
		com_sprintf(newdir, sizeof(newdir), "%s_%s", dir, "english");
	else
		q_strncpyz(newdir, dir, sizeof(newdir));

	sp = fs_searchpaths;
	do {
		if (sp == NULL) {
			if (!localized) {
				q_strncpyz(fs_gamedir, newdir, sizeof(fs_gamedir));
			} else {
				fs_build_ospath(path, newdir, "", ospath);
				ospath[strlen(ospath) - 1] = '\0';

				if (!sys_directory_has_files(ospath))
					return;
			}

			search = z_malloc(sizeof(struct searchpath));
			directory = z_malloc(sizeof(struct directory));
			search->dir = directory;
			q_strncpyz(search->dir->path, path, sizeof(search->dir->path));
			q_strncpyz(search->dir->gamedir, newdir, 
					   sizeof(search->dir->gamedir));
			search->localized = localized;
			search->b = d;

			fs_update_search_paths(search);
			fs_find_pack_files(path, newdir);
			return;
		}

		if (sp->dir != NULL && q_stricmp(sp->dir->path, path) == 0) {
			if (q_stricmp(sp->dir->gamedir, newdir) == 0) {
				if (sp->localized != localized) {
					com_warning("WARNING: game folder %s/%s added as " \
								"both a localized & non - localized. " \
								"Using folder as % s\n",
						path, newdir, sp->localized ? "localized" : "non-localized");
				}

				if (!sp->localized)
					return;

				if (sp->b == d)
					return; // no idea

				com_warning("WARNING: game folder %s/%s re-added as " \
							"localized folder with a different language",
					path, newdir);

				return;
			}
		}

		sp = sp->next;
	} while (true);
}

/**
 * @brief 
 * @param path 
 * @param dir 
*/
void fs_add_game_directory(const char *path, const char *dir)
{
	fs_add_game_directory_real(path, dir, 1, 0);
	fs_add_game_directory_real(path, dir, 0, 0);
}

static bool fs_pak_is_not_localized(struct searchpath *path)
{
	if (!path->localized || fs_ignorelocalized->integer == 0)
		return true;

	return false;
}

/**
 * @brief This isn't really necessary but 1.1 code has it anyway
 * @param  
*/
static void fs_set_restrictions(void)
{
	struct searchpath *path;
	
	if (fs_restrict->integer == 0)
		return;

	cvar_set("fs_restrict", "1");
	com_printf("\nRunning in restricted demo mode.\n\n");

	fs_restart(0);
	fs_startup(BASEDEMO);

	for (path = fs_searchpaths; path != NULL; path = path->next) {
		if (path->pack != NULL && fs_pak_is_not_localized(path) &&
			path->pack->checksum != -0x4e0a6a0b) {
			com_error(ERR_FATAL, "Corrupted pak0.pk3: %u", 
					  path->pack->checksum);
		}
	}
}

/**
 * @brief 
 * @param  
*/
void fs_init_filesystem(void)
{
	com_startup_variable("fs_cdpath"); // really needed?
	com_startup_variable("fs_basepath");
	com_startup_variable("fs_homepath");
	com_startup_variable("fs_game");
	com_startup_variable("fs_copyfiles");
	com_startup_variable("fs_restrict");
	com_startup_variable("fs_usewolf"); // ?
	com_startup_variable("cl_language"); // ?

	fs_startup(BASEGAME); // BASEGAME

	// not needed really
	fs_set_restrictions();

	if (fs_read_file("default.cfg", NULL) <= 0)
		com_error(ERR_FATAL, "Couldn't load default.cfg. " \
				  "Make sure iocod is run from the correct folder.");

	q_strncpyz(lastvalidbase, fs_basepath->string, sizeof(lastvalidbase));
	q_strncpyz(lastvalidgame, fs_gamedirvar->string, sizeof(lastvalidgame));
}

void fs_pure_server_set_loaded_paks(const char *paksums, const char *paknames)
{
	(void) paksums; (void) paknames; //-Werror=unusued-parameter
}

/**
 * @brief 
 * @param checksumfeed 
*/
void fs_restart(int checksumfeed)
{
	// free anything we currently have loaded
	fs_shutdown(0);

	fs_checksumfeed = checksumfeed;

	fs_clear_pak_references(0);

	fs_startup(BASEGAME);
	fs_set_restrictions();

	// fs_readfile for config stuff here
	if (fs_read_file("default_mp.cfg", NULL) <= 0) {
		if (*lastvalidbase != '\0') {
			fs_pure_server_set_loaded_paks("", "");

			cvar_set("fs_basepath", lastvalidbase);
			cvar_set("fs_gamedirvar", lastvalidgame);

			lastvalidbase[0] = '\0';
			lastvalidgame[0] = '\0';

			cvar_set("fs_restrict", "0");

			fs_restart(checksumfeed);

			com_error(ERR_DROP, "Invalid game folder");
		}

		com_error(ERR_FATAL, "Couldn't load default_mp.cfg. " \
				  "Make sure iocod is run from the correct folder");
	}

	if (q_stricmp(fs_gamedirvar->string, lastvalidgame) != 0) {
		if (!com_safe_mode())
			cbuf_add_text("exec config_mp_server.cfg\n");
	}

	q_strncpyz(lastvalidbase, fs_basepath->string, sizeof(lastvalidbase));
	q_strncpyz(lastvalidgame, fs_gamedirvar->string, sizeof(lastvalidgame));
}

static void fs_close_searchpaths(struct searchpath *s)
{
	struct searchpath *p;

	while (s != NULL) {
		p = s->next;

		if (s->pack != NULL) {
			unzClose(s->pack->handle);
			z_free(s->pack->buildbuffer);
			z_free(s->pack);
		}

		if (s->dir != NULL)
			z_free(s->dir);

		z_free(s);
		s = p;
	}
}
/*
// no idea what this is yet
struct otherdata {
	int data[67];
	int *somepointer;
	struct otherdata *next;
};

static void fs_close_otherdata(struct otherdata *s)
{
	struct otherdata *p;

	while (s != NULL) {
		p = s->next;

		z_free(s->somepointer);
		z_free(s);

		s = p;
	}
}
*/
// TEMP
struct searchpath *currentpaths = NULL;

// 
// searchpaths2 is some other data structure relating to files
// probably script stuff
void fs_shutdown(bool closemfp)
{
	int i;

	for (i = 1; i < MAX_FILE_HANDLES; i++) {
		if (fsh[i].filesize > 0)
			fs_fclose_file(i);
	}

	if (closemfp) {
		// TEMP
		if (currentpaths != fs_searchpaths)
			fs_close_searchpaths(fs_searchpaths);

		//if (dat2 != fs_searchpaths2)
		//	fs_close_otherdata(fs_searchpaths2);

	} else {
		fs_close_searchpaths(fs_searchpaths);
		//fs_close_otherdata(fs_searchpaths2);
	}

	// any fs_ calls will now be an error until reintialized
	fs_searchpaths = NULL;
	//fs_searchpaths2 = NULL;

	// only 1.1 does this?
	#ifndef CODVERSION1_5
	fs_fake_checksum = 0;
	#endif

	cmd_remove_command("path");
	cmd_remove_command("fullpath");
	cmd_remove_command("dir");
	cmd_remove_command("fdir");
	cmd_remove_command("touchfile");
}

void fs_clear_pak_references(int flags)
{
	struct searchpath *search;

	if (flags == 0)
		flags = -1;

	for (search = fs_searchpaths; search != NULL; search = search->next) {
		if (search->pack != NULL)
			search->pack->referenced &= ~flags;
	}
}
