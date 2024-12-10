#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "include/vfs.h"
#include "include/logger/logger.h"

int vfs_user_get_index(const char *username, VirtualFileSystem *vfs)
{
    for (unsigned int i=0; i < vfs->count.user;) {
        if (strcmp(vfs->user[i].name, username) == 0 && vfs->count.user < MAX_USERS) {
            return i;
        }
        i++;
    }

    return -1; // Error: User not found
}

int vfs_user_get_permission(const char *username, VirtualFileSystem *vfs)
{
    int user_index = vfs_user_get_index(username, vfs);
    if (strcmp(vfs->user[user_index].name, username) == 0) {
        return vfs->user[user_index].permission;
    }

    log_error("User not found");
    return -1; // User not found
}

int vfs_user_create(const char *username,
                        const char *password,
                        int permission,
                        VirtualFileSystem *vfs)
{
    if (vfs->count.user >= MAX_USERS) {
        return -1; // Maximum users reached
    }

    for (unsigned int i = 0; i < vfs->count.user; i++) {
        if (strcmp(vfs->user[i].name, username) == 0) {
            log_error("User already exists");
            return -4; // User already exists
        }
    }

    vfs->user[vfs->count.user].permission = permission;
    vfs->user[vfs->count.user].name = strdup(username);
    vfs->user[vfs->count.user].passwd = strdup(password);

    vfs->count.user++;
    char msg[120];

    sprintf(msg, "user created %s", username);
    log_info(msg); // deleted1
    return 0;
}

int vfs_user_authenticate(const char *username, const char *password, VirtualFileSystem *vfs)
{
    for (unsigned int i = 0; i < vfs->count.user; i++) {
        if (strcmp(vfs->user[i].name, username) == 0 && strcmp(vfs->user[i].passwd, password) == 0) {
            char msg[120];
            sprintf(msg, "Authentication passed %s", username);
            log_info(msg); // deleted1
            return i; // User authenticated, return user index
        }
    }

    log_error("Authentication failed");
    return -1; // Authentication failed
}

