#ifndef LATEST_H
#define LATEST_H

void get_home_directory(char *path);
void read_ignore_list(const char *ignore_file, char *ignore_list[], int *num_ignore);
void read_paths_from_file(const char *config_file, char *paths[], int *num_paths);
void read_extensions_from_file(const char *extensions_file, char *extensions[], int *num_extensions);
int is_ignored(const char *dirname, char *ignore_list[], int num_ignore);
void list_files_recursive(const char *path, char *ignore_list[], int num_ignore, char *extensions[], int num_extensions, int hours);

#endif  /* LATEST_H */

