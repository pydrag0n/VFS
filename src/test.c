#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/vfs.h"

int main() {
    VirtualFileSystem vfs;
    vfs.count = 0;

    // Test vfs_create_file
    const char* filename1 = "testfile1.txt";
    const char* filename2 = "testfile2.txt";
    int result = vfs_create_file(filename1, &vfs);

    if (result != 0)
    {
        printf("Failed to create file %s\n", filename1);
        return 1;
    }
    result = vfs_create_file(filename2, &vfs);

    if (result != 0)
    {
        printf("Failed to create file %s\n", filename2);
        return 1;
    }

    // Test vfs_get_file_index
    int file_index1 = vfs_get_file_index(filename1, &vfs);
    int file_index2 = vfs_get_file_index(filename2, &vfs);

    if (file_index1 < 0 || file_index2 < 0)
    {
        printf("Failed to get file index\n");
        return 1;
    }

    // Test vfs_get_file_name
    char* name1 = vfs_get_file_name(file_index1, &vfs);
    char* name2 = vfs_get_file_name(file_index2, &vfs);

    if (strcmp(name1, filename1) != 0 || strcmp(name2, filename2) != 0)
    {
        printf("Failed to get file name\n");
        return 1;
    }

    // Test vfs_get_file_size
    long size1 = vfs_get_file_size(file_index1, &vfs);
    long size2 = vfs_get_file_size(file_index2, &vfs);

    if (size1 != DEFAULT_FILE_SIZE || size2 != DEFAULT_FILE_SIZE)
    {
        printf("Failed to get file size\n");
        return 1;
    }

    // Test vfs_write_file
    const char* content1 = "This is the content of testfile1.txt";
    const char* content2 = "This is the content of testfile2.txt";
    result = vfs_write_file(filename1, content1, 'w', &vfs);

    if (result != 0)
    {
        printf("Failed to write to file %s\n", filename1);
        return 1;
    }
    result = vfs_write_file(filename2, content2, 'w', &vfs);

    if (result != 0)
    {
        printf("Failed to write to file %s\n", filename2);
        return 1;
    }

    // Test vfs_get_file_content
    char* content_read1 = vfs_get_file_content(file_index1, &vfs);
    char* content_read2 = vfs_get_file_content(file_index2, &vfs);

    if (strcmp(content_read1, content1) != 0 || strcmp(content_read2, content2) != 0)
    {
        printf("Failed to read file content\n");
        return 1;
    }

    // Test vfs_get_file_metadata
    char* metadata1 = vfs_get_file_metadata(filename1, &vfs);
    char* metadata2 = vfs_get_file_metadata(filename2, &vfs);

    if (metadata1 == NULL || metadata2 == NULL)
    {
        printf("Failed to get file metadata\n");
        return 1;
    }
    printf("%s\n", metadata1);
    printf("%s\n", metadata2);
    free(metadata1);
    free(metadata2);

    // Test vfs_get_file_index_metadata
    metadata1 = vfs_get_file_index_metadata(file_index1, &vfs);
    metadata2 = vfs_get_file_index_metadata(file_index2, &vfs);

    if (metadata1 == NULL || metadata2 == NULL)
    {
        printf("Failed to get file index metadata\n");
        return 1;
    }
    printf("%s\n", metadata1);
    printf("%s\n", metadata2);
    free(metadata1);
    free(metadata2);

    // Test vfs_read_file
    content_read1 = vfs_read_file(filename1, &vfs);
    content_read2 = vfs_read_file(filename2, &vfs);

    if (content_read1 == NULL || content_read2 == NULL)
    {
        printf("Failed to read file\n");
        return 1;
    }
    printf("Content of %s: %s\n", filename1, content_read1);
    printf("Content of %s: %s\n", filename2, content_read2);

    // Test vfs_write_file in append mode
    const char* append_content = " Appended content";
    result = vfs_write_file(filename1, append_content, 'a', &vfs);

    if (result != 0)
    {
        printf("Failed to append to file %s\n", filename1);
        return 1;
    }
    content_read1 = vfs_get_file_content(file_index1, &vfs);

    if (strstr(content_read1, append_content) == NULL)
    {
        printf("Failed to append content\n");
        return 1;
    }

    if (strlen(content_read1)+1 != strlen(content1) +
                                    strlen(append_content) + 1)
    {
        printf("Failed to append content correctly\n");
        printf("%i - %i", strlen(content_read1), strlen(content1) +
                                                    strlen(append_content) + 1);
        return 1;
    }

    // Test vfs_free_memory
    vfs_free_memory(&vfs);

    printf("All tests passed successfully\n");
    return 0;
}