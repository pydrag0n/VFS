#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/vfs.h"

// beta (example)
int main() {
    VirtualFileSystem vfs;
    vfs_init("out/vfs_logs.log", &vfs);

    while (1) {
        printf("VFS> ");
        char command[1024];
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; // Remove newline character

        if (strcmp(command, "exit") == 0) {
            break;
        }

        char *token = strtok(command, " ");
        if (token == NULL) {
            continue;
        }

        if (strcmp(token, "create") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                vfs_file_create(token, &vfs);
            } else {
                printf("Error: filename required\n");
            }
        } else if (strcmp(token, "write") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                char *filename = token;
                token = strtok(NULL, " ");
                if (token != NULL) {
                    char *content = token;
                    char mode = 'w'; // Default mode
                    token = strtok(NULL, " ");
                    if (token != NULL) {
                        mode = token[0];
                    }
                    vfs_file_write(filename, content, mode, &vfs);
                } else {
                    printf("Error: content required\n");
                }
            } else {
                printf("Error: filename required\n");
            }
        } else if (strcmp(token, "read") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                char *filename = token;
                char *content = vfs_file_read(filename, &vfs);
                if (content != NULL) {
                    printf("%s\n", content);
                } else {
                    printf("Error: file not found\n");
                }
            } else {
                printf("Error: filename required\n");
            }
        } else if (strcmp(token, "delete") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                vfs_file_delete(token, &vfs);
            } else {
                printf("Error: filename required\n");
            }
        } else if (strcmp(token, "set_permission") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                char *filename = token;
                token = strtok(NULL, " ");
                if (token != NULL) {
                    int permission = atoi(token);
                    vfs_file_set_permission(filename, permission, &vfs);
                } else {
                    printf("Error: permission required\n");
                }
            } else {
                printf("Error: filename required\n");
            }
        } else if (strcmp(token, "login") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                char *username = token;
                token = strtok(NULL, " ");
                if (token != NULL) {
                    char *password = token;
                    vfs_user_authenticate(username, password, &vfs);
                } else {
                    printf("Error: password required\n");
                }
            } else {
                printf("Error: username required\n");
            }
        } else if (strcmp(token, "create_user") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                char *username = token;
                token = strtok(NULL, " ");
                if (token != NULL) {
                    char *password = token;
                    token = strtok(NULL, " ");
                    if (token != NULL) {
                        int permission = atoi(token);
                        vfs_user_create(username, password, permission, &vfs);
                    } else {
                        printf("Error: permission required\n");
                    }
                } else {
                    printf("Error: password required\n");
                }
            } else {
                printf("Error: username required\n");
            }
        } else if (strcmp(token, "get_permission") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                char *username = token;
                int permission = vfs_user_get_permission(username, &vfs);
                printf("Permission: %d\n", permission);
            } else {
                printf("Error: username required\n");
            }
        } else if (strcmp(token, "save") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                vfs_save(&vfs, token);
            } else {
                printf("Error: filename required\n");
            }
        } else if (strcmp(token, "load") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                vfs_load(&vfs, token);
            } else {
                printf("Error: filename required\n");
            }
        } else if (strcmp(token, "display_all") == 0) {
            vfs_display_all_data(&vfs);
        } else if (strcmp(token, "set_main_user") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) {
                int user_index = vfs_user_get_index(token, &vfs);
                vfs_set_main_user(user_index);
            } else {
                printf("Error: user index required\n");
            }
        } else {
            printf("Unknown command\n");
        }
    }

    vfs_free_memory(&vfs);
    return 0;
}
