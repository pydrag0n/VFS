#include "permissions.h"

//macros
#define MAX_FILES           1024
#define MAX_USERS           64
#define DEFAULT_FILE_SIZE   0

// struct / typedef

typedef struct VFSAPI_USER {
    int permission;
    char *name;
    char *passwd;
} user;

typedef struct VFSAPI_META_DATA {
    char *create_date;
    char *change_date;
    char *open_date;
    long size;

    int permission;    // 1 only-read \
                            2 Read and write \
                            3 Read, write, and delete \
                            4  Administrative
} metadata;

typedef struct VFSAPI_FILE {
    metadata metadata;
    char *name;
    char *content;
} vfsFile;

typedef struct VFSAPI_COUNTER {
    unsigned int file;
    unsigned int user;
} counter;

typedef struct VFSAPI {
    user user[MAX_USERS];
    vfsFile file[MAX_FILES];
    counter count;
} VirtualFileSystem;

extern int vfs_main_user;

// ## functions

int vfs_init(VirtualFileSystem *vfs);

// file

int vfs_file_create(const char *filename, VirtualFileSystem *vfs);
int vfs_file_write(const char *filename, const char *content, char mode, VirtualFileSystem *vfs);
char *vfs_file_read(const char *filename, VirtualFileSystem *vfs);
int vfs_file_delete(const char *filename, VirtualFileSystem *vfs);

int vfs_file_get_index(const char *filename, VirtualFileSystem *vfs);
long vfs_file_get_size(int file_index, VirtualFileSystem *vfs);

char *vfs_file_get_metadata(const char *filename, VirtualFileSystem *vfs);
char *vfs_file_get_name(int file_index, VirtualFileSystem *vfs);
char *vfs_file_get_content(int file_index, VirtualFileSystem *vfs);

void vfs_free_memory(VirtualFileSystem *vfs);


// user

int vfs_user_authenticate(const char *username, const char *password, VirtualFileSystem *vfs);
int vfs_user_get_permission(const char *username, VirtualFileSystem *vfs);
int vfs_user_create(const char *username, const char *password, int permission, VirtualFileSystem *vfs);
int vfs_user_get_index(const char *username, VirtualFileSystem *vfs);

void vfs_set_main_user(int user_index);
int vfs_save(VirtualFileSystem *vfs, const char *filename);
int vfs_load(VirtualFileSystem *vfs, const char *filename);
void vfs_display_all_data(VirtualFileSystem *vfs);