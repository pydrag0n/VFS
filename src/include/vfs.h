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

typedef struct VFSAPI {
    user user[MAX_USERS];
    vfsFile file[MAX_FILES];
    unsigned int count;
    unsigned int user_count;
} VirtualFileSystem;

extern int vfs_main_user;

// ## functions
// file

int vfs_create_file(const char *filename, VirtualFileSystem *vfs);
int vfs_write_file(const char *filename, const char *content, char mode, VirtualFileSystem *vfs);
char *vfs_read_file(const char *filename, VirtualFileSystem *vfs);

int vfs_get_file_index(const char *filename, VirtualFileSystem *vfs);
long vfs_get_file_size(int file_index, VirtualFileSystem *vfs);

char *vfs_get_file_metadata(const char *filename, VirtualFileSystem *vfs);
char *vfs_get_file_index_metadata(int file_index, VirtualFileSystem *vfs);
char *vfs_get_file_name(int file_index, VirtualFileSystem *vfs);
char *vfs_get_file_content(int file_index, VirtualFileSystem *vfs);

void vfs_free_memory(VirtualFileSystem *vfs);


// user

void vfs_set_main_user(int user_index);
int vfs_authenticate_user(const char *username, const char *password, VirtualFileSystem *vfs);
int vfs_get_user_permission(const char *username, VirtualFileSystem *vfs);
int vfs_create_user(const char *username, const char *password, int permission, VirtualFileSystem *vfs);
int vfs_user_get_index(const char *username, VirtualFileSystem *vfs);
void vfs_user_get_list(VirtualFileSystem *vfs);