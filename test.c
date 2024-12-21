#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "include/vfs.h"

int main() {
    clock_t start_time, end_time;

    start_time = clock();
    VirtualFileSystem vfs;
    vfs_init("out/logs.log", &vfs);

    // Test vfs_user_create
    vfs_user_create("user1", "1234qwerty", READ_WRITE, &vfs);
    vfs_user_create("user2", "1234qwerty", READ_WRITE, &vfs);
    vfs_user_create("user3", "1234qwerty", READ_WRITE_DEL, &vfs);

    // Test vfs_set_main_user
    vfs_set_main_user(vfs_user_get_index("user1", &vfs));

    // Test vfs_user_get_list

    // Test vfs_user_authenticate
    assert(vfs_user_authenticate("user2", "1234qwerty", &vfs) >= 0);
    assert(vfs_user_authenticate("user3", "1234qwerty", &vfs) >= 0);

    // Test vfs_file_create
    const char* filename1 = "testfile1.txt";
    const char* filename2 = "testfile2.txt";
    const char* filename3 = "testfile3.txt";
    int result = vfs_file_create(filename1, &vfs);

    if (result != 0) {
        printf("Failed to create file %s\n", filename1);
        return -1;
    }
    result = vfs_file_create(filename2, &vfs);

    if (result != 0) {
        printf("Failed to create file %s\n", filename2);
        return -1;
    }
    result = vfs_file_create(filename3, &vfs);
    if (result != 0) {
        printf("Failed to create file %s\n", filename3);
        return -1;
    }

    // Test vfs_file_get_index
    int file_index1 = vfs_file_get_index(filename1, &vfs);
    int file_index2 = vfs_file_get_index(filename2, &vfs);
    int file_index3 = vfs_file_get_index(filename3, &vfs);

    if (file_index1 < 0 || file_index2 < 0 || file_index3 < 0) {
        printf("Failed to get file index\n");
        return -1;
    }

    // Test vfs_file_get_name
    char* name1 = vfs_file_get_name(file_index1, &vfs);
    char* name2 = vfs_file_get_name(file_index2, &vfs);
    char* name3 = vfs_file_get_name(file_index3, &vfs);

    if (strcmp(name1, filename1) != 0 || strcmp(name2, filename2) != 0 || strcmp(name3, filename3) != 0) {
        printf("Failed to get file name\n");
        return -1;
    }

    // Test vfs_file_get_size
    long size1 = vfs_file_get_size(file_index1, &vfs);
    long size2 = vfs_file_get_size(file_index2, &vfs);

    if (size1 != DEFAULT_FILE_SIZE || size2 != DEFAULT_FILE_SIZE) {
        printf("Failed to get file size\n");
        return -1;
    }

    // Test vfs_file_write
    const char* content1 = "This is the content of testfile1.txt";
    const char* content2 = "This is the content of testfile2.txt";
    result = vfs_file_write(filename1, content1, 'w', &vfs);

    if (result != 0) {
        printf("Failed to write to file %s\n", filename1);
        return -1;
    }
    result = vfs_file_write(filename2, content2, 'w', &vfs);

    if (result != 0) {
        printf("Failed to write to file %s\n", filename2);
        return -1;
    }

    // Test vfs_file_get_content
    char* content_read1 = vfs_file_get_content(file_index1, &vfs);
    char* content_read2 = vfs_file_get_content(file_index2, &vfs);

    if (strcmp(content_read1, content1) != 0 || strcmp(content_read2, content2) != 0) {
        printf("Failed to read file content\n");
        return -1;
    }

    // Test vfs_file_get_metadata
    char* metadata1 = vfs_file_get_metadata(filename1, &vfs);
    char* metadata2 = vfs_file_get_metadata(filename2, &vfs);

    if (metadata1 == NULL || metadata2 == NULL) {
        printf("Failed to get file metadata\n");
        return -1;
    }
    printf("%s\n", metadata1);
    printf("%s\n", metadata2);
    free(metadata1);
    free(metadata2);


    // Test vfs_file_read
    content_read1 = vfs_file_read(filename1, &vfs);
    content_read2 = vfs_file_read(filename2, &vfs);

    if (content_read1 == NULL || content_read2 == NULL) {
        printf("Failed to read file\n");
        return -1;
    }
    printf("Content of %s: %s\n", filename1, content_read1);
    printf("Content of %s: %s\n", filename2, content_read2);

    // Test vfs_file_change_name
    vfs_file_change_name(filename3, "f3", &vfs);
    char* changed_name3 = vfs_file_get_name(vfs_file_get_index("f3", &vfs), &vfs);

    if (strcmp(changed_name3, "f3") != 0) {
        printf("Failed to get file name\n");
        return -1;
    }


    // Test vfs_file_write in append mode
    const char* append_content = " Appended content";
    result = vfs_file_write(filename1, append_content, 'a', &vfs);

    if (result != 0) {
        printf("Failed to append to file %s\n", filename1);
        return -1;
    }
    content_read1 = vfs_file_get_content(file_index1, &vfs);

    if (strstr(content_read1, append_content) == NULL) {
        printf("Failed to append content\n");
        return -1;
    }

    if (strlen(content_read1)+1 != strlen(content1) + strlen(append_content) + 1) {
        printf("Failed to append content correctly\n");
        printf("%lli - %lli", strlen(content_read1), strlen(content1) + strlen(append_content) + 1);
        return -1;
    }
    vfs_set_main_user(vfs_user_get_index("user3", &vfs));

    int isdel = vfs_file_delete(filename2, &vfs);
    if (isdel < 0) {
        printf("permission danied");
    }

    else {
        printf("file deleted %s\n", filename2);
        char* metadata1 = vfs_file_get_metadata(filename1, &vfs);
        char* metadata2 = vfs_file_get_metadata(filename2, &vfs);

        if (metadata1 == NULL || metadata2 == NULL) {
            printf("Failed to get file metadata\n");
        }
    }
    vfs_save(&vfs, "out/vfs_img.bin");

    // Test vfs_free_memory
    vfs_free_memory(&vfs);

    end_time = clock();

    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("\n\nTime taken: %f seconds\n", time_taken);

    printf("All tests passed successfully\n");
    return 0;
}