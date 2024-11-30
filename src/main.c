#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/vfs.h"

int main() {
    VirtualFileSystem vfs;
    vfs.count = 0;

    vfs_create_file("file_1", &vfs);
    vfs_create_file("file_2", &vfs);
    vfs_create_file("file_3", &vfs);
    vfs_create_file("file_4", &vfs);
    vfs_create_file("file_10", &vfs);
    vfs_create_file("file_6", &vfs);
    vfs_create_file("file_7", &vfs);
    vfs_create_file("file_8", &vfs);
    vfs_create_file("file_9", &vfs);
    vfs_create_file("file_10", &vfs);


    if (vfs_write_file("file_1", "Lorem ipsum dolor sit amet", 1, &vfs) == 0) {
        printf("Content written to file_1\n");
    } else {
        printf("Failed to write to file_1\n");
    }


    char *content = vfs_read_file("file_1", &vfs);
    if (content != NULL) {
        printf("Content of file_1: %s\n", content);
    }
    printf("%i", vfs.count);
    for (int i = 0; i < vfs.count; i++) {
        free(vfs.file[i].name);
        free(vfs.file[i].content);
    }

    return 0;
}