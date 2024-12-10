#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/logger/logger.h"
#include "include/vfs.h"

int vfs_init(VirtualFileSystem *vfs)
{
    Config newConfig = { "test.log", DEFAULT_FORMAT, ALL};
    set_def_cfg(&newConfig);

    vfs->count.file = 0;
    vfs->count.user = 0;
    vfs_user_create("root", "root", ADMIN, vfs);

    return 0;
}

// ===================хуйня переделывай===================
int vfs_save(VirtualFileSystem *vfs, const char *filename)
{
    FILE *file = fopen(filename, "wb");
    if (!file) {
        log_error("Failed to open file for writing");
        return -1;
    }

    // Write the counter
    fwrite(&vfs->count, sizeof(vfs->count), 1, file);

    // Write users
    for (unsigned int i = 0; i < vfs->count.user; i++) {
        fwrite(&vfs->user[i].permission, sizeof(int), 1, file);
        size_t name_len = strlen(vfs->user[i].name) + 1;
        fwrite(&name_len, sizeof(size_t), 1, file);
        fwrite(vfs->user[i].name, name_len, 1, file);
        size_t passwd_len = strlen(vfs->user[i].passwd) + 1;
        fwrite(&passwd_len, sizeof(size_t), 1, file);
        fwrite(vfs->user[i].passwd, passwd_len, 1, file);
    }

    // Write files
    for (unsigned int i = 0; i < vfs->count.file; i++) {
        // Metadata
        fwrite(&vfs->file[i].metadata.permission, sizeof(int), 1, file);
        size_t create_date_len = strlen(vfs->file[i].metadata.create_date) + 1;
        fwrite(&create_date_len, sizeof(size_t), 1, file);
        fwrite(vfs->file[i].metadata.create_date, create_date_len, 1, file);
        size_t change_date_len = strlen(vfs->file[i].metadata.change_date) + 1;
        fwrite(&change_date_len, sizeof(size_t), 1, file);
        fwrite(vfs->file[i].metadata.change_date, change_date_len, 1, file);
        size_t open_date_len = strlen(vfs->file[i].metadata.open_date) + 1;
        fwrite(&open_date_len, sizeof(size_t), 1, file);
        fwrite(vfs->file[i].metadata.open_date, open_date_len, 1, file);
        fwrite(&vfs->file[i].metadata.size, sizeof(long), 1, file);

        // File name
        size_t name_len = strlen(vfs->file[i].name) + 1;
        fwrite(&name_len, sizeof(size_t), 1, file);
        fwrite(vfs->file[i].name, name_len, 1, file);

        // File content
        size_t content_len = vfs->file[i].metadata.size + 1;
        fwrite(&content_len, sizeof(size_t), 1, file);
        if (vfs->file[i].content) {
            fwrite(vfs->file[i].content, content_len, 1, file);
        }
    }

    fclose(file);
    return 0;
}

int vfs_load(VirtualFileSystem *vfs, const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file) {
        log_error("Failed to open file for reading");
        return -1;
    }

    // Read the counter
    fread(&vfs->count, sizeof(vfs->count), 1, file);

    // Read users
    for (unsigned int i = 0; i < vfs->count.user; i++) {
        fread(&vfs->user[i].permission, sizeof(int), 1, file);
        size_t name_len;
        fread(&name_len, sizeof(size_t), 1, file);
        vfs->user[i].name = malloc(name_len);
        fread(vfs->user[i].name, name_len, 1, file);
        size_t passwd_len;
        fread(&passwd_len, sizeof(size_t), 1, file);
        vfs->user[i].passwd = malloc(passwd_len);
        fread(vfs->user[i].passwd, passwd_len, 1, file);
    }

    // Read files
    for (unsigned int i = 0; i < vfs->count.file; i++) {
        // Metadata
        fread(&vfs->file[i].metadata.permission, sizeof(int), 1, file);
        size_t create_date_len;
        fread(&create_date_len, sizeof(size_t), 1, file);
        vfs->file[i].metadata.create_date = malloc(create_date_len);
        fread(vfs->file[i].metadata.create_date, create_date_len, 1, file);
        size_t change_date_len;
        fread(&change_date_len, sizeof(size_t), 1, file);
        vfs->file[i].metadata.change_date = malloc(change_date_len);
        fread(vfs->file[i].metadata.change_date, change_date_len, 1, file);
        size_t open_date_len;
        fread(&open_date_len, sizeof(size_t), 1, file);
        vfs->file[i].metadata.open_date = malloc(open_date_len);
        fread(vfs->file[i].metadata.open_date, open_date_len, 1, file);
        fread(&vfs->file[i].metadata.size, sizeof(long), 1, file);

        // File name
        size_t name_len;
        fread(&name_len, sizeof(size_t), 1, file);
        vfs->file[i].name = malloc(name_len);
        fread(vfs->file[i].name, name_len, 1, file);

        // File content
        size_t content_len;
        fread(&content_len, sizeof(size_t), 1, file);
        if (content_len > 0) {
            vfs->file[i].content = malloc(content_len);
            fread(vfs->file[i].content, content_len, 1, file);
        } else {
            vfs->file[i].content = NULL;
        }
    }

    fclose(file);
    return 0;
}

// Function to display all data from the VFS
void vfs_display_all_data(VirtualFileSystem *vfs)
{
    // Display user data
    printf("Users:\n");
    for (int i = 0; i < vfs->count.user; i++) {
        printf("User %d:\n", i);
        printf("  Name: %s\n", vfs->user[i].name);
        printf("  Password: %s\n", vfs->user[i].passwd);
        printf("  Permission: %d\n", vfs->user[i].permission);
        printf("\n");
    }

    // Display file data
    printf("Files:\n");
    for (int i = 0; i < vfs->count.file; i++) {
        printf("File %d:\n", i);
        printf("  Name: %s\n", vfs->file[i].name);
        printf("  Create Date: %s\n", vfs->file[i].metadata.create_date);
        printf("  Change Date: %s\n", vfs->file[i].metadata.change_date);
        printf("  Open Date: %s\n", vfs->file[i].metadata.open_date);
        printf("  Size: %ld bytes\n", vfs->file[i].metadata.size);
        printf("  Permission: %d\n", vfs->file[i].metadata.permission);
        printf("  Content: %s\n", vfs->file[i].content);
        printf("\n");
    }

    // Display main user index
    printf("Main User Index: %d\n", vfs_main_user);
}