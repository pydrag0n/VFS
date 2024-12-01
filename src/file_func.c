
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "include/vfs.h"


char *vfs_get_datetime()
{
        int date_size = 256;
        char *datetime = malloc(date_size);
        if (datetime == NULL)
        {
            return NULL;
        }

        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        sprintf(datetime, "%s  %02d:%02d", __DATE__, timeinfo->tm_hour,  timeinfo->tm_min);

        return datetime;
}

char *vfs_get_file_name(int file_index, VirtualFileSystem *vfs)
{
    if (file_index < vfs->count)
    {
        return vfs->file[file_index].name;
    }
    return NULL;
}

long vfs_get_file_size(int file_index, VirtualFileSystem *vfs)
{
    if (file_index < vfs->count)
    {
        return vfs->file[file_index].metadata.size;
    }
    return -1;
}

char *vfs_get_file_content(int file_index, VirtualFileSystem *vfs)
{
    char *content = vfs->file[file_index].content;
    if (file_index < vfs->count && content != NULL)
    {
        return content;
    }
    return NULL;
}


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

    if (vfs_get_file_index(filename, vfs) > 0)
    {
        return -1; // File already exists
    }

    if (vfs->count >= MAX_FILES)
    {
        return -1; // Maximum file limit reached
    }

    vfs->file[vfs->count].name = malloc(strlen(filename) + 1);

    if (vfs->file[vfs->count].name == NULL)
    {
        return -1;
    }

    strcpy(vfs->file[vfs->count].name, filename);
    vfs->file[vfs->count].content = NULL;

    vfs->file[vfs->count].metadata.create_date = malloc(256);
    vfs->file[vfs->count].metadata.change_date = malloc(256);
    vfs->file[vfs->count].metadata.open_date = malloc(256);

    char *datetime_str = vfs_get_datetime();

    if (datetime_str != NULL)
    {
        strcpy(vfs->file[vfs->count].metadata.create_date, datetime_str);
        strcpy(vfs->file[vfs->count].metadata.change_date, datetime_str);
        strcpy(vfs->file[vfs->count].metadata.open_date, datetime_str);
        free(datetime_str);
    }

    vfs->file[vfs->count].metadata.size = DEFAULT_FILE_SIZE;

    vfs->count++;

    return 0;

}


char *vfs_read_file(const char *filename, VirtualFileSystem *vfs)
{
    int file_index = vfs_get_file_index(filename, vfs);
    if (file_index >= 0 && strcmp(vfs->file[file_index].name, filename) == 0)
    {
        if (vfs->file[file_index].content == NULL)
        {
            printf("%s", "File is empty\n"); // debug
            return NULL;
        }

        char *datetime_str = vfs_get_datetime();

        if (datetime_str != NULL)
        {
            strcpy(vfs->file[file_index].metadata.open_date, datetime_str);
        }

        return vfs->file[file_index].content;
    }

    return NULL;

}


int vfs_write_file(const char *filename,
                    const char *content,
                    char mode, VirtualFileSystem *vfs)
{

    /*
     *   mode: 1  = write
     *   mode: 2  = append
     */

    int file_index = vfs_get_file_index(filename, vfs);

    if (file_index >= 0)
    {
        switch (mode)
        {
        case 'w':

            if (vfs->file[file_index].content != NULL)
            {
                free(vfs->file[file_index].content);
            }

            vfs->file[file_index].content = malloc(strlen(content) + 1);

            if (vfs->file[file_index].content == NULL) {  return -1;  }

            strcpy(vfs->file[file_index].content, content);
            break;

        case 'a':

            if (vfs->file[file_index].content != NULL)
            {
                vfs->file[file_index].content = realloc(vfs->file[file_index].content,
                                                        strlen(content) +
                                                        strlen(vfs->file[file_index].content) + 1);

                strcat(vfs->file[file_index].content, content);
            } else {
                vfs->file[file_index].content = malloc(strlen(content) + 1);
                strcpy(vfs->file[file_index].content, content);
            }
            break;

        default:
            return -1;
            break;
        }

        char *datetime_str = vfs_get_datetime();

        if (datetime_str != NULL)
        {
            strcpy(vfs->file[file_index].metadata.open_date, datetime_str);
            strcpy(vfs->file[file_index].metadata.change_date, datetime_str);
        }

        vfs->file[file_index].metadata.size = strlen(vfs->file[file_index].content) + 1;

        return 0;
    }

    return -1;

}

char *vfs_get_file_metadata(const char *filename, VirtualFileSystem *vfs)
{

    int file_index = vfs_get_file_index(filename, vfs);

    if (file_index >= 0 && file_index < vfs->count)
    {
        unsigned int i = file_index;

        const char *create_date = vfs->file[i].metadata.create_date
                                ? vfs->file[i].metadata.create_date : "N/A";

        const char *change_date = vfs->file[i].metadata.change_date
                                ? vfs->file[i].metadata.change_date : "N/A";

        const char *open_date = vfs->file[i].metadata.open_date
                                ? vfs->file[i].metadata.open_date : "N/A";

        size_t info_size = strlen(vfs->file[i].name) +
                            strlen(create_date) +
                            strlen(change_date) +
                            strlen(open_date) + 50;

        char *info = malloc(info_size);

        if (info == NULL) {
            printf("Failed to allocate memory\n");
            return NULL;
        }

        sprintf(info, "%s     | %s | %s | %s | %ld",
                                    vfs->file[i].name,
                                    create_date,
                                    change_date,
                                    open_date,
                                    vfs->file[i].metadata.size);

        return info;
    }

    // printf("%s", "File not found");
    return NULL;

}

char *vfs_get_file_index_metadata(int file_index, VirtualFileSystem *vfs)
{

    if (file_index >= 0 && file_index < vfs->count) {
        unsigned int i = file_index;

        const char *create_date = vfs->file[i].metadata.create_date
                                ? vfs->file[i].metadata.create_date : "N/A";

        const char *change_date = vfs->file[i].metadata.change_date
                                ? vfs->file[i].metadata.change_date : "N/A";

        const char *open_date = vfs->file[i].metadata.open_date
                                ? vfs->file[i].metadata.open_date : "N/A";

        size_t info_size = strlen(vfs->file[i].name) +
                            strlen(create_date) +
                            strlen(change_date) +
                            strlen(open_date) + 50;

        char *info = malloc(info_size);
        if (info == NULL) {
            // printf("Failed to allocate memory\n");
            return NULL;
        }

        sprintf(info, "%s     | %s | %s | %s | %ld",
                                    vfs->file[i].name,
                                    create_date,
                                    change_date,
                                    open_date,
                                    vfs->file[i].metadata.size);

        return info;
    }

    // printf("Index out of array\n");
    return NULL;

}

void vfs_free_memory(VirtualFileSystem *vfs)
{
    for (int i = 0; i < vfs->count; i++)
    {
        free(vfs->file[i].name);
        free(vfs->file[i].content);
        free(vfs->file[i].metadata.change_date);
        free(vfs->file[i].metadata.create_date);
        free(vfs->file[i].metadata.open_date);
    }
    vfs->count = 0;

}
