#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILES 100



typedef struct main
{
    char *name;
    char *content;
} vfsFile;

struct VFS
{
    vfsFile file;
    int count;
};

unsigned int count_f = 0;

struct VFS vfs[1024];

int create_file(const char *filename) {

    if (count_f >= MAX_FILES) {
        return -1;
    }

    vfs[count_f].file.name = malloc(strlen(filename) + 1);
    strcpy(vfs[count_f].file.name, filename);
    count_f++;
    return 0;
}

int open_file(const char *filename, const char mode, char *content) {
    char isfile_found = 0;
    for (int i=0; i<=count_f;) {
        if (strcmp(vfs[i].file.name, filename) == 0) {
            vfs[i].file.content = malloc(strlen(content) + 1);
            strcpy(vfs[i].file.content, content);
            isfile_found = 1;
        }
        i++;
    }
    if (!isfile_found) {
        printf("%s", "File not found");
        return -1;
    }
    return 0;
}

int main() {
    create_file("a.txt");
    create_file("a1.txt");
    create_file("a2.txt");
    create_file("a3.txt");

    for (int i=0; i<=count_f;) {
        printf("%s\n", vfs[i].file.name);
        i++;
    }
    return 0;
}

