//macros
#define MAX_FILES 1024

// struct / typedef
typedef struct main
{
    char *name;
    char *content;
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
