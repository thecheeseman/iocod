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
 * @file unix_shared.c
 * @date 2022-02-04
*/

#include "shared.h"
#include "common.h"

#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <errno.h>
#include <dirent.h>

static char cdpath[MAX_OSPATH];
static char installpath[MAX_OSPATH];
static char homepath[MAX_OSPATH];

unsigned long sys_timebase = 0;
int curtime;

void sys_mkdir(const char *path)
{
	mkdir(path, 0777);
}

void sys_end_streamed_file(filehandle f)
{
	UNUSED(f);
}

int sys_milliseconds(void)
{
	struct timeval tp;
	gettimeofday(&tp, NULL);

	if (!sys_timebase) {
		sys_timebase = tp.tv_sec;
		return tp.tv_usec / 1000;
	}

	curtime = (tp.tv_sec - sys_timebase) * 1000 + tp.tv_usec / 1000;
	return curtime;
}

char *sys_get_current_user(void)
{
	struct passwd *p;

	if ((p = getpwuid(getuid())) == NULL)
		return "player";

	return p->pw_name;
}

char *sys_cwd(void)
{
	static char cwd[MAX_OSPATH];

	getcwd(cwd, sizeof(cwd) - 1);
	cwd[MAX_OSPATH - 1] = 0;

	return cwd;
}

char *sys_default_base_path(void)
{
	if (installpath[0])
		return installpath;
	else
		return sys_cwd();
}

char *sys_default_install_path(void)
{
	if (*installpath)
		return installpath;
	else
		return sys_cwd();
}

char *sys_default_home_path(void)
{
	char *p;

	if (*homepath)
		return homepath;

	if ((p = getenv("HOME")) != NULL) {
		q_strncpyz(homepath, p, sizeof(homepath));
		q_strcat(homepath, sizeof(homepath), "/.callofduty");

		if (mkdir(homepath, 0777)) {
			if (errno != EEXIST)
				sys_error("Unable to create directory \"%s\", " \
						  "error is % s(% d)\n", 
						  homepath, strerror(errno), errno);
		}

		return homepath;
	}

	return "";
}

void sys_set_default_cd_path(const char *path)
{
	q_strncpyz(cdpath, path, sizeof(cdpath));
}

char *sys_default_cd_path(void)
{
	return cdpath;
}

bool sys_directory_has_files(const char *ospath)
{
	DIR *fdir;
	struct dirent *d;

	if (!(fdir = opendir(ospath)))
		return 0;

	while ((d = readdir(fdir)) != NULL) {
		if (strcasecmp(d->d_name, ".") || strcasecmp(d->d_name, ".."))
			continue;

		closedir(fdir);
		return 1;
	}

	return 0;
}

#define MAX_FOUND_FILES 4096

/**
 * @brief 
 * @param basedir 
 * @param subdirs 
 * @param filter 
 * @param list 
 * @param numfiles 
*/
void sys_list_filtered_files(const char *basedir, char *subdirs, char *filter, 
	char **list, int *numfiles)
{
	char search[MAX_OSPATH], newsubdirs[MAX_OSPATH], filename[MAX_OSPATH];
	DIR *fdir;
	struct dirent *d;
	struct stat st;

	if (*numfiles >= MAX_FOUND_FILES - 1)
		return;

	if (strlen(subdirs))
		com_sprintf(search, sizeof(search), "%s/%s", basedir, subdirs);
	else
		com_sprintf(search, sizeof(search), "%s", basedir);

	if ((fdir = opendir(search)) == NULL)
		return;

	while ((d = readdir(fdir)) != NULL) {
		com_sprintf(filename, sizeof(filename), "%s/%s", search, d->d_name);
		if (stat(filename, &st) == -1)
			continue;

		if (st.st_mode & S_IFDIR) {
			if (strcasecmp(d->d_name, ".") && strcasecmp(d->d_name, "..")) {
				if (strlen(subdirs))
					com_sprintf(newsubdirs, sizeof(newsubdirs), "%s/%s", 
								subdirs, d->d_name);
				else
					com_sprintf(newsubdirs, sizeof(newsubdirs), "%s", 
								d->d_name);

				// recurse
				sys_list_filtered_files(basedir, newsubdirs, filter, 
										list, numfiles);
			}
		}

		if (*numfiles >= MAX_FOUND_FILES - 1)
			break;

		com_sprintf(filename, sizeof(filename), "%s/%s", subdirs, d->d_name);
		if (!com_filter_path(filter, filename, false))
			continue;

		list[*numfiles] = copy_string(filename);
		(*numfiles)++;
	}

	closedir(fdir);
}

/**
 * @brief 
 * @param directory 
 * @param extension 
 * @param filter 
 * @param numfiles 
 * @param wantsubs 
 * @return 
*/
char **sys_list_files(const char *directory, const char *extension, 
					  char *filter, int *numfiles, bool wantsubs)
{
	struct dirent *d;
	DIR *fdir;
	int i, nfiles/*, extlen*/;
	char search[MAX_OSPATH], **listcopy, *list[MAX_FOUND_FILES];
	struct stat st;
	bool dironly = wantsubs;

	if (filter) {
		com_printf("filtered\n");
		nfiles = 0;

		sys_list_filtered_files(directory, "", filter, list, &nfiles);
		list[nfiles] = 0;

		*numfiles = nfiles;
		if (!nfiles)
			return NULL;

		listcopy = z_malloc((nfiles + 1) * sizeof(*listcopy));
		for (i = 0; i < nfiles; i++)
			listcopy[i] = list[i];

		listcopy[i] = NULL;
		return listcopy;
	}

	if (!extension)
		extension = "";

	if (extension[0] == '/' && extension[1] == '\0') {
		extension = "";
		dironly = true;
	}

	//extlen = strlen(extension);
	nfiles = 0;

	if ((fdir = opendir(directory)) == NULL) {
		com_dprintf("Couldn't open directory '%s'\n", directory);
		*numfiles = 0;
		return NULL;
	}

	while ((d = readdir(fdir)) != NULL) {
		com_sprintf(search, sizeof(search), "%s/%s", directory, d->d_name);
		if (stat(search, &st) == -1)
			continue;

		if ((dironly && !(st.st_mode & S_IFDIR)) ||
			(!dironly && (st.st_mode & S_IFDIR))) {
			continue;
		}

		if (*extension) {
			if (strlen(d->d_name) < strlen(extension) ||
				strcasecmp(d->d_name + strlen(d->d_name) - 
						   strlen(extension), extension)) {
				continue; // didn't match
			}
		}

		if (nfiles == MAX_FOUND_FILES - 1)
			break;

		list[nfiles] = copy_string(d->d_name);
		nfiles++;
	}

	list[nfiles] = NULL;
	closedir(fdir);

	// return a copy of the list
	*numfiles = nfiles;

	if (!nfiles)
		return NULL;

	listcopy = z_malloc((nfiles + 1) * sizeof(*listcopy));
	for (i = 0; i < nfiles; i++)
		listcopy[i] = list[i];
	listcopy[i] = NULL;

	return listcopy;
}

/**
 * @brief 
 * @param list 
*/
void sys_free_file_list(char **list)
{
	int i;

	if (!list)
		return;

	for (i = 0; list[i]; i++)
		z_free(list[i]);

	z_free(list);
}
