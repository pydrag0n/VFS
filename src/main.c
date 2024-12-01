#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "include/vfs.h"

int main() {

    VirtualFileSystem vfs;
    vfs.count = 0;

    vfs_create_file("file_1", &vfs);
    vfs_create_file("file_2", &vfs);
    vfs_create_file("file_3", &vfs);
    vfs_create_file("file_4", &vfs);
    vfs_create_file("file_6", &vfs);
    vfs_create_file("file_7", &vfs);
    vfs_create_file("file_8", &vfs);
    vfs_create_file("file_9", &vfs);
    vfs_create_file("file_10", &vfs);


    if (vfs_write_file("file_4", "Lorem ipsum dolor sit amet abc", 'w', &vfs) == 0) {
        printf("Content written to file_4\n");

        char *content = vfs_read_file("file_4", &vfs);
        if (content != NULL) {
            printf("Content of file_4: %s\n", content);
        }
    } else {
        printf("Failed to write to file_4\n");
    }

     if (vfs_write_file("file_4", " NEW CONTENT", 'a', &vfs) == 0) {
        printf("Content append to file_4\n");
        char *content4 = vfs_read_file("file_4", &vfs);
        if (content4 != NULL) {
            printf("Content of file_4: %s\n", content4);
        }

    } else {
        printf("Failed to append to file_4\n");
    }

    for (int i=0; i < vfs.count;)
    {
        printf("%s\n", vfs.file[i].name);
        i++;
    }



    get_file_info("file_2", &vfs);
    for (int i=0; i < vfs.count;)
    {
        printf("%s\n", get_file_index_info(i, &vfs));
        i++;
    }

    for (int i = 0; i < vfs.count; i++) {
        free(vfs.file[i].name);
        free(vfs.file[i].content);
        free(vfs.file[i].metadata.change_date);
        free(vfs.file[i].metadata.create_date);
        free(vfs.file[i].metadata.open_date);
    }


    return 0;
}