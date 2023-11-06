#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <limits.h>

#define MY_PATH_MAX 4500

void get_home_directory(char *path) {
#ifdef MAC
    // Linux and macOS - HOME environment variable
    const char *home_path = getenv("HOME");
    if (home_path) {
        strcpy(path, home_path);
    }
#endif
#ifdef DOCKER
    // Linux and macOS - HOME environment variable
    const char *home_path = getenv("DOCKERHOME");
    if (home_path) {
        strcpy(path, home_path);
    }
#endif
}

void read_ignore_list(const char *ignore_file, char *ignore_list[], int *num_ignore) {
    // Construct the full path to ignore_file
    char ignore_path[MY_PATH_MAX];
    get_home_directory(ignore_path);
    strcat(ignore_path, "/.latest/config/");
    strcat(ignore_path, ignore_file);

    FILE *file = fopen(ignore_path, "r");
    if (!file) {
        perror("Could not open ignore file");
        return;
    }

    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;

        // Allocate memory for the directory name
        ignore_list[count] = malloc(strlen(line) + 1);
        strcpy(ignore_list[count], line);

        count++;
    }

    fclose(file);
    *num_ignore = count;
}

void read_paths_from_file(const char *config_file, char *paths[], int *num_paths) {
    // Construct the full path to config_file
    char config_path[MY_PATH_MAX];
    get_home_directory(config_path);
    strcat(config_path, "/.latest/config/");
    strcat(config_path, config_file);
    
    FILE *file = fopen(config_path, "r");
    if (!file) {
        perror("Could not open config file");
        return;
    }

    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;

        // Allocate memory for the path
        paths[count] = malloc(strlen(line) + 1);
        strcpy(paths[count], line);

        count++;
    }

    fclose(file);
    *num_paths = count;
}

void read_extensions_from_file(const char *extensions_file, char *extensions[], int *num_extensions) {
    // Construct the full path to extensions_file
    char extensions_path[MY_PATH_MAX];
    get_home_directory(extensions_path);
    strcat(extensions_path, "/.latest/config/");
    strcat(extensions_path, extensions_file);

    FILE *file = fopen(extensions_path, "r");
    if (!file) {
        perror("Could not open extensions file");
        return;
    }

    int count = 0;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;

        // Allocate memory for the extension
        extensions[count] = malloc(strlen(line) + 1);
        strcpy(extensions[count], line);

        count++;
    }

    fclose(file);
    *num_extensions = count;
}

int is_ignored(const char *dirname, char *ignore_list[], int num_ignore) {
    for (int i = 0; i < num_ignore; i++) {
        if (strcmp(dirname, ignore_list[i]) == 0) {
            return 1; // Directory is ignored
        }
    }
    return 0; // Directory is not ignored
}

void list_files_recursive(const char *path, char *ignore_list[], int num_ignore, char *extensions[], int num_extensions, int hours) {
    DIR *dir = opendir(path);
    if (!dir) {
        perror("\n***START****\n");
        perror("\nCould not open directory\n");
        printf("\npath=%s\n", path);
        perror("\n***END****\n");
        return;
    }

    struct dirent *entry;
    char ignore_path[MY_PATH_MAX];
    while ((entry = readdir(dir))) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            if (is_ignored(entry->d_name, ignore_list, num_ignore))
                continue;

            char sub_path[256];
            snprintf(sub_path, sizeof(sub_path), "%s/%s", path, entry->d_name);
            list_files_recursive(sub_path, ignore_list, num_ignore, extensions, num_extensions, hours);
        } else if (entry->d_type == DT_REG) {
            char *file_extension = strrchr(entry->d_name, '.');
            if (file_extension) {
                for (int i = 0; i < num_extensions; i++) {
                    if (strcmp(file_extension, extensions[i]) == 0) {
                        char full_path[256];
                        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

                        struct stat file_stat;
                        if (stat(full_path, &file_stat) == 0) {
                            time_t current_time = time(NULL);
                            time_t modification_time = file_stat.st_mtime;
                            int hours_since_modification = (current_time - modification_time) / 3600;
                            if (hours_since_modification <= hours) {
                                char* name_swap = getenv("DOCKER_NAME_SWAP");
                                char* docker_home = getenv("DOCKERHOME");
                                memset(ignore_path, '\0', sizeof(ignore_path));
                                strcat(ignore_path, name_swap);
                                strcat(ignore_path, full_path + strlen(docker_home));
                                printf("%s (%d)\n", ignore_path, hours_since_modification);
                            }
                        }
                        break;  // Extension found, no need to check further
                    }
                }
            }
        }
    }

    closedir(dir);
}

#ifdef COMPILE
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <hours>\n", argv[0]);
        return 1;
    }

    int hours = atoi(argv[1]);

    // Read the ignore list from the file
    char *ignore_list[100]; // Assuming a maximum of 100 ignored directories
    int num_ignore = 0;
    read_ignore_list("ignore_me.txt", ignore_list, &num_ignore);

    // Read the paths from the config file
    char *paths[100]; // Assuming a maximum of 100 paths
    int num_paths = 0;
    read_paths_from_file("latest.config", paths, &num_paths);

    // Read the extensions from the file
    char *extensions[100]; // Assuming a maximum of 100 extensions
    int num_extensions = 0;
    read_extensions_from_file("extensions.txt", extensions, &num_extensions);

#ifdef DOCKER
    char ignore_path[MY_PATH_MAX];
    char* name_swap = getenv("DOCKER_NAME_SWAP");
    for (int i = 0; i < num_paths; i++) {
        memset(ignore_path, '\0', sizeof(ignore_path));
        get_home_directory(ignore_path);
        strcat(ignore_path, paths[i] + strlen(name_swap));
        printf("Files in %s modified within the last %d hours:\n", paths[i], hours);
        list_files_recursive(ignore_path, ignore_list, num_ignore, extensions, num_extensions, hours);
        printf("\n");
    }
#else
    for (int i = 0; i < num_paths; i++) {
        printf("Files in %s modified within the last %d hours:\n", paths[i], hours);
        list_files_recursive(paths[i], ignore_list, num_ignore, extensions, num_extensions, hours);
        printf("\n");
    }
#endif

    // Free allocated memory for ignore_list, paths, and extensions
    for (int i = 0; i < num_ignore; i++) {
        free(ignore_list[i]);
    }

    for (int i = 0; i < num_paths; i++) {
        free(paths[i]);
    }

    for (int i = 0; i < num_extensions; i++) {
        free(extensions[i]);
    }

    return 0;
}
#endif
