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

    for (int i = 0; i < count_f; i++) {
        if (strcmp(vfs[i].file.name, filename) == 0) {
            if (mode == 'w') {

                if (vfs[i].file.content != NULL) {
                    free(vfs[i].file.content);
                }

                vfs[i].file.content = malloc(strlen(content) + 1);
                strcpy(vfs[i].file.content, content);

            } else if (mode == 'r') {
                if (vfs[i].file.content != NULL) {
                    printf("%s", vfs[i].file.content);
                } else {
                    printf("File is empty.\n");
                }
            } else if (mode == 'a') {
                 if (vfs[i].file.content != NULL) {
                    vfs[i].file.content = realloc(vfs[i].file.content, strlen(vfs[i].file.content) + strlen(content)+1);
                     strcat(vfs[i].file.content, content);
                } else {
                    vfs[i].file.content = realloc(vfs[i].file.content, strlen(vfs[i].file.content) + strlen(content)+1);

                    strcpy(vfs[i].file.content, content);
                }
            }
            return 0;
        }
    }

    printf("File not found\n");
    return -1;
}

int main() {
    create_file("a.txt");
    create_file("a1.txt");
    create_file("a2.txt");
    create_file("a3.txt");
    open_file("a3.txt", 'w', "\nPRIVET PEOPLE");
    open_file("a3.txt", 'r', NULL);
    open_file("a3.txt", 'a', " NEW INFO");
    open_file("a3.txt", 'r', NULL);
    printf("%s\n", vfs[2].file.content);
    return 0;
}

