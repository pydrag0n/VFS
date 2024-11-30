/*
    функции для работы с файлами
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/vfs.h"


int vfs_get_file_index(const char *filename, VirtualFileSystem *vfs)
{
    for (int i = 0; i < vfs->count;)
    {
        if (strcmp(vfs->file[i].name, filename) == 0)
        {
            return i;
        }
        i++;
    }

    // printf("%s", "File not found"); // debug
    return -1;
}

int vfs_create_file(const char *filename, VirtualFileSystem *vfs)
{
    if (vfs_get_file_index(filename, vfs) > 0) {
        printf("File already exists %s\n", filename);
        return -1;
    }

    if (vfs->count >= MAX_FILES)
    {
        printf("%s\n", "Maximum file limit reached");
        return -1;
    }

    vfs->file[vfs->count].name = malloc(strlen(filename) + 1);

    if (vfs->file[vfs->count].name == NULL)
    {
        return -1;
    }

    strcpy(vfs->file[vfs->count].name, filename);
    vfs->file[vfs->count].content = NULL;
    vfs->count++;

    return 0;
}


char *vfs_read_file(const char *filename, VirtualFileSystem *vfs)
{
    int file_index = vfs_get_file_index(filename, vfs);
    if (file_index >= 0)
    {
        if (strcmp(vfs->file[file_index].name, filename) == 0)
        {
            if (vfs->file[file_index].content == NULL)
            {
                printf("%s", "File is emplty"); // debug
                return NULL;
            }

            return vfs->file[file_index].content;
        }
    }

    return NULL;
}

/*
*   mode: 1  = write
*   mode: 2  = append
*   mode: 3  = read
*/

int vfs_write_file(const char *filename, const char *content, int mode, VirtualFileSystem *vfs)
{
    int file_index = vfs_get_file_index(filename, vfs);

    if (file_index >= 0)
    {

        if (vfs->file[file_index].content != NULL)
        {
            free(vfs->file[file_index].content);
        }

        vfs->file[file_index].content = malloc(strlen(content) + 1);
        if (vfs->file[file_index].content == NULL)
        {
            return -1;
        }

        strcpy(vfs->file[file_index].content, content);

        return 0;
    }

    return -1;

}
