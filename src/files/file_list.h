#ifndef FILES_FILE_LIST_H
#define FILES_FILE_LIST_H

#define MAX_FOUND_FILES 0x1000

char **list_filtered_files(const char *path, const char *extension,
                              char *filter, int *total);
char **list_files(const char *path, const char *extension, int *num_files);
void free_file_list(char **list);
int get_file_list(const char *path, const char *ext, char *list, int size);

#endif /* FILES_FILE_LIST_H */
