/*
    функции для работы с файлами
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "include/vfs.h"



char * _vfs_get_datetime() {
        int date_size = 256;
        char *datetime = malloc(date_size);
        if (datetime == NULL) {
            return NULL;
        }

        time_t rawtime;
        struct tm *timeinfo;
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        sprintf(datetime, "%02d-%02d-%04d %02d:%02d:%02d",
                timeinfo->tm_mday,
                timeinfo->tm_mon + 1,
                timeinfo->tm_year + 1900,
                timeinfo->tm_hour,
                timeinfo->tm_min,
                timeinfo->tm_sec
                );

        return datetime;
}


long _get_file_size(int file_index, VirtualFileSystem *vfs)
{
    return vfs->file[file_index].metadata.size;
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
    // char datetime[256];
    // strcpy(datetime, );
    strcpy(vfs->file[vfs->count].name, filename);
    vfs->file[vfs->count].content = NULL;

    vfs->file[vfs->count].metadata.create_date = malloc(100);
    vfs->file[vfs->count].metadata.change_date = malloc(100);
    vfs->file[vfs->count].metadata.open_date = malloc(100);
    char *datetime_str = _vfs_get_datetime();

    if (datetime_str != NULL) {
        strcpy(vfs->file[vfs->count].metadata.open_date, datetime_str);
        strcpy(vfs->file[vfs->count].metadata.change_date, datetime_str);
    }

    strcpy(vfs->file[vfs->count].metadata.create_date, _vfs_get_datetime());
    strcpy(vfs->file[vfs->count].metadata.change_date, _vfs_get_datetime());
    strcpy(vfs->file[vfs->count].metadata.open_date, _vfs_get_datetime());

    vfs->file[vfs->count].metadata.size = 0;

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
                printf("%s", "File is empty\n"); // debug
                return NULL;
            }

            char *datetime_str = _vfs_get_datetime();
            if (datetime_str != NULL) {
                strcpy(vfs->file[file_index].metadata.open_date, datetime_str);
            }

            return vfs->file[file_index].content;
        }
    }

    return NULL;
}


int vfs_write_file(const char *filename, const char *content, int mode, VirtualFileSystem *vfs)
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

            if (vfs->file[file_index].content == NULL)
            {
                return -1;
            }

            strcpy(vfs->file[file_index].content, content);
            break;

        case 'a':

            if (vfs->file[file_index].content != NULL)
            {
                vfs->file[file_index].content = realloc(vfs->file[file_index].content,
                                                        strlen(content) +
                                                        strlen(vfs->file[file_index].content) + 1);
                strcat(vfs->file[file_index].content, content);
            }

            else {
                vfs->file[file_index].content = malloc(strlen(content) + 1);
                strcpy(vfs->file[file_index].content, content);
            }
            break;

        default:
            break;
        }
        char *datetime_str = _vfs_get_datetime();
        if (datetime_str != NULL) {
            strcpy(vfs->file[file_index].metadata.open_date, datetime_str);
            strcpy(vfs->file[file_index].metadata.change_date, datetime_str);
        }

        vfs->file[file_index].metadata.size = strlen(vfs->file[file_index].content) + 1;

        return 0;
    }

    return -1;

}

int get_file_info(const char *filename, VirtualFileSystem *vfs)
{
    int file_index = vfs_get_file_index(filename, vfs);

    if (file_index >= 0 && file_index <= vfs->count)
    {
        unsigned int i = file_index;


        size_t info_size = strlen(vfs->file[i].name) +
                        strlen(vfs->file[i].metadata.create_date) +
                        strlen(vfs->file[i].metadata.change_date) +
                        strlen(vfs->file[i].metadata.open_date) + 4;

        char *info = malloc(info_size);
        if (info == NULL) {
            printf("%s", "Failed to allocate memory");
            return -1;
        }

        sprintf(info, "%s \t\t %s \t %s \t %s \t %ld\n",
                vfs->file[i].name,
                vfs->file[i].metadata.create_date,
                vfs->file[i].metadata.change_date,
                vfs->file[i].metadata.open_date,
                vfs->file[i].metadata.size);

        printf("%s \t %s \t\t %s \t\t %s \t\t %s\n", "filename", "create_date", "change_date", "open_date", "size");
        printf("%s\n", info);

        free(info);
        return 0;
    }

    printf("%s %s", "File not found:", filename);
    return -1;
}

char *get_file_index_info(int file_index, VirtualFileSystem *vfs)
{
    if (file_index < vfs->count)
    {
        unsigned int i = file_index;

        size_t info_size = strlen(vfs->file[i].name) +
                        strlen(vfs->file[i].metadata.create_date) +
                        strlen(vfs->file[i].metadata.change_date) +
                        strlen(vfs->file[i].metadata.open_date) + 4;

        char *info = malloc(info_size);
        if (info == NULL) {
            printf("%s", "Failed to allocate memory");
            return NULL;
        }

        sprintf(info, "%s | %s | %s | %s | %ld\n",
                vfs->file[i].name,
                vfs->file[i].metadata.create_date,
                vfs->file[i].metadata.change_date,
                vfs->file[i].metadata.open_date,
                vfs->file[i].metadata.size);


        return info;
    }

    printf("%s", "Index out of array");
    return NULL;
}