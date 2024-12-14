#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "include/vfs.h"


int main()
{
    VirtualFileSystem vfs1;
    vfs_load(&vfs1, "out/vfs_img.bin");
    vfs_display_all_data(&vfs1);
}