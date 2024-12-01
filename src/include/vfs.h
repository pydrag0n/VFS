//macros
#define MAX_FILES 1024

// struct / typedef

typedef struct
{
    char *create_date;
    char *change_date;
    char *open_date;
    long size;
} metadata;

typedef struct
{
    metadata metadata;
    char    *name;
    char    *content;
} vfsFile;

typedef struct
{
    vfsFile      file[MAX_FILES];
    unsigned int count;
} VirtualFileSystem;

// ## functions
// file_func

int vfs_create_file(const char *filename, VirtualFileSystem *vfs);
char *vfs_read_file(const char *filename, VirtualFileSystem *vfs);
int vfs_get_file_index(const char *filename, VirtualFileSystem *vfs);
int vfs_write_file(const char *filename, const char *content, int mode, VirtualFileSystem *vfs);
int get_file_info(const char *filename, VirtualFileSystem *vfs);
char *get_file_index_info(int file_index, VirtualFileSystem *vfs);
