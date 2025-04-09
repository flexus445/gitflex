#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <openssl/sha.h>
#include <zlib.h>
#include <dirent.h>
#include "gitflex.h"

struct tree_entry {
    char mode[8];       // File mode (e.g., "100644" for regular file)
    char name[256];     // Entry name
    char hash[41];      // SHA-1 hash (40 hex chars + null terminator)
};

//int compare_entries(const void *a, const void *b) {
//    return strcmp(((struct tree_entry *)a)->name, ((struct tree_entry *)b)->name);
//}
//
//void runInit() {
//    struct stat st;
//
//    if (stat(".gitflex", &st) == 0) {
//        printf("Error: .gitflex already initialized.\n");
//        return;
//    }
//
//    if (mkdir(".gitflex", 0777) == -1) {
//        perror("Error creating .gitflex");
//        return;
//    }
//
//    mkdir(".gitflex/objects", 0777);
//    mkdir(".gitflex/refs", 0777);
//    mkdir(".gitflex/refs/heads", 0777);
//
//    FILE *headFile = fopen(".gitflex/HEAD", "w");
//    if (headFile) {
//        fprintf(headFile, "ref: refs/heads/main\n");
//        fclose(headFile);
//    } else {
//        perror("Error creating .gitflex/HEAD");
//    }
//
//    printf("Initialized empty GitFlex repository in .gitflex/\n");
//}
//
//void runHashObject(const char *fileName) {
//    if(fileName == NULL)
//    {
//        printf("Provide a file name.\n");
//        return;
//    }
//
//    struct stat st;
//
//    if (stat(fileName, &st) == -1) {
//        printf("Error: file doesn\'t exist.\n");
//        return;
//    }
//
//    int fileSize = st.st_size;
//    if (fileSize == 0) {
//        printf("Error: file is empty.\n");
//        return;
//    }
//
//    FILE *file = fopen(fileName, "rb");
//    if (file) {
//        char *buffer = malloc(fileSize);
//        if (buffer == NULL) {
//            perror("Error allocating memory");
//            fclose(file);
//            return;
//        }
//        fread(buffer, sizeof(char), fileSize, file);
//        fclose(file);
//
//        unsigned char hash[SHA_DIGEST_LENGTH];
//        SHA1((unsigned char *)buffer, fileSize, hash);
//        unsigned char hexHash[SHA_DIGEST_LENGTH * 2 + 1];
//        for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
//            sprintf((char*)hexHash + (i * 2), "%02x", hash[i]);
//        }
//        hexHash[SHA_DIGEST_LENGTH * 2] = '\0';
//
////        printf("%s", hexHash);
//
//        char dirPath[256];
//        snprintf(dirPath, sizeof(dirPath), ".gitflex/objects/%c%c/", hexHash[0], hexHash[1]);
//        if (mkdir(dirPath, 0777) == -1) {
////            perror("Error creating directory");
//        } else {
//            printf("Directory %s created successfully\n", dirPath);
//        }
//
//        char filePath[256];
//        snprintf(filePath, sizeof(filePath), ".gitflex/objects/%c%c/%s", hexHash[0], hexHash[1], hexHash + 2);
//
//
//        char header[64];
//        snprintf(header, sizeof(header), "blob %d", fileSize);
//        size_t headerLen = strlen(header) + 1;
//        char *blob = malloc(headerLen + fileSize);
//        if (!blob) {
//            perror("Error allocating memory");
//            return;
//        }
//        memcpy(blob, header, headerLen);
//        memcpy(blob + headerLen, buffer, fileSize);
//
//
//        uLong compressedSize = compressBound(headerLen + fileSize);
//        char *compressedBlob = malloc(compressedSize);
//        if (!compressedBlob) {
//            perror("Error allocating memory for compression");
//            free(blob);
//            return;
//        }
//        if (compress((Bytef *)compressedBlob, &compressedSize, (Bytef *)blob, headerLen + fileSize) != Z_OK) {
//            perror("Error compressing blob");
//            free(blob);
//            free(compressedBlob);
//            return;
//        }
//
//
//        FILE *outFile = fopen(filePath, "wb");
//        if (outFile) {
//            fwrite(compressedBlob, sizeof(unsigned char), compressedSize, outFile);
//            fclose(outFile);
//            printf("File %s created successfully\n", filePath);
//        } else {
//            perror("Error creating file");
//        }
//
//        free(buffer);
//        free(blob);
//        free(compressedBlob);
//    } else {
//        perror("Error reding file.\n");
//    }
//
//
//}
//
//void runCatFile(const char *hash) {
//    if(hash == NULL)
//    {
//        printf("Provide a hash.\n");
//        return;
//    }
//
//    struct stat st;
//
//    char dirPath[256];
//    snprintf(dirPath, sizeof(dirPath), ".gitflex/objects/%c%c/%s", hash[0], hash[1], hash+2);
//    if (stat(dirPath, &st) == -1) {
//        perror("Error finding directory");
//    }
//
//    FILE *file = fopen(dirPath, "rb");
//    int fileSize = st.st_size;
//    if (file) {
//        char *buffer = malloc(fileSize);
//        if (buffer == NULL) {
//            perror("Error allocating memory");
//            fclose(file);
//            return;
//        }
//        fread(buffer, sizeof(char), fileSize, file);
//        fclose(file);
//
//
////        uLong ucompSize = strlen(buffer)+1;
//        uLong ucompSize = fileSize * 10;
//        uLong compSize = compressBound(ucompSize);
//        char *outBuffer = malloc(compSize);
//
//        if (outBuffer == NULL) {
//            perror("Error allocating memory for decompressed content");
//            free(buffer);
//            return;
//        }
//
//        int ret = uncompress((Bytef *)outBuffer, &ucompSize, (Bytef *)buffer, compSize);
//        if (ret != Z_OK) {
//            printf("Error decompressing data.\n");
//            free(buffer);
//            free(outBuffer);
//            return;
//        }
//        char *content = outBuffer;
//        if (strncmp(content, "blob ", 5) != 0) {
//            content += 5;
//            int contentSize = 0;
//            while (*content >= '0' && *content <= '9') {
//                contentSize = contentSize * 10 + (*content - '0');
//                content++;
//            }
//
//            if (*content == '\0') {
//                content++;
//            } else {
//                printf("Error: Expected null byte after size.\n");
//                free(buffer);
//                free(outBuffer);
//                return;
//            }
//
//            printf("Content size: %d bytes\n", contentSize);
//            printf("Actual content: %s\n", content);
//        }
//        else {
//            printf("Error: Invalid object format.\n");
//        }
//
//        free(buffer);
//        free(outBuffer);
//    }
//}
//
///*
//char* runWriteTree(const char* dir) {
//    DIR *dirp;
//    struct dirent *dp;
//    struct stat st;
//    struct tree_entry *entries = NULL;
//    int entry_count = 0;
//    int entries_capacity = 10;
//
//    entries = malloc(entries_capacity * sizeof(struct tree_entry));
//    if (!entries) {
//        perror("Failed to allocate memory for tree entries");
//        return NULL;
//    }
//
//    if ((dirp = opendir(dir)) == NULL) {
//        fprintf(stderr, "Couldn't open %s: ", dir);
//        perror("");
//        return NULL;
//    }
//
//    while ((dp = readdir(dirp)) != NULL) {
//        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0 || strcmp(dp->d_name, ".gitflex") == 0)
//            continue;
//
//        char path[512];
//        snprintf(path, sizeof(path), "%s/%s", dir, dp->d_name);
//
//        if (stat(path, &st) != 0)
//        {
//            perror("stat failed");
//            continue;
//        }
//
//        if (entry_count >= entries_capacity) {
//            entries_capacity *= 2;
//            struct tree_entry *new_entries = realloc(entries,
//                                                     entries_capacity * sizeof(struct tree_entry));
//            if (!new_entries) {
//                perror("Failed to resize entries array");
//                free(entries);
//                closedir(dirp);
//                return NULL;
//            }
//            entries = new_entries;
//        }
//
//        if (S_ISDIR(st.st_mode)) {
//            printf("%s is a directory\n", path);
//
//            char *subdir_hash = runWriteTree(path);
//            if (subdir_hash) {
//                // Add tree entry
//                strcpy(entries[entry_count].mode, "40000");
//                strcpy(entries[entry_count].name, dp->d_name);
//                strcpy(entries[entry_count].hash, subdir_hash);
//                entry_count++;
//                free(subdir_hash);
//            }
//
//        } else if (S_ISREG(st.st_mode)) {
//            printf("%s is a file\n", path);
//
//            // Hash the file
//            char *file_hash = hashTree(path);
//            if (file_hash) {
//                // Set the file mode based on permissions
//                if (st.st_mode & S_IXUSR) {
//                    strcpy(entries[entry_count].mode, "100755");  // Executable file
//                } else {
//                    strcpy(entries[entry_count].mode, "100644");  // Regular file
//                }
//                strcpy(entries[entry_count].name, dp->d_name);
//                strcpy(entries[entry_count].hash, file_hash);
//                entry_count++;
//                free(file_hash);  // Free the allocated hash
//            }
//        }
//
//    }
//    closedir(dirp);
//    if (entry_count == 0) {
//        free(entries);
//        return NULL;
//    }
//
//    // Sort entries by name (Git requirement)
//    qsort(entries, entry_count, sizeof(struct tree_entry), compare_entries);
//
//    // Create and store the tree object
//    char *tree_hash = hashTree(entries, entry_count);
//
//    // Clean up
//    free(entries);
//
//    // Return the hash of the created tree
//    return tree_hash;
//}
//*/
//
//char* getHashFromFile(const char *path) {
//    struct stat st;
//
//    if (stat(path, &st) == -1) {
//        printf("Error: file doesn't exist.\n");
//        return NULL;
//    }
//
//    int fileSize = st.st_size;
//    if (fileSize == 0) {
//        printf("Error: file is empty.\n");
//        return NULL;
//    }
//
//    FILE *file = fopen(path, "rb");
//    if (!file) {
//        perror("Error reading file");
//        return NULL;
//    }
//
//    char *buffer = malloc(fileSize);
//    if (buffer == NULL) {
//        perror("Error allocating memory");
//        fclose(file);
//        return NULL;
//    }
//    fread(buffer, sizeof(char), fileSize, file);
//    fclose(file);
//
//    unsigned char hash[SHA_DIGEST_LENGTH];
//    SHA1((unsigned char *)buffer, fileSize, hash);
//
//    char *hexHash = malloc(SHA_DIGEST_LENGTH * 2 + 1);
//    if (hexHash == NULL) {
//        perror("Error allocating memory for hash");
//        free(buffer);
//        return NULL;
//    }
//
//    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
//        sprintf((char*)hexHash + (i * 2), "%02x", hash[i]);
//    }
//    hexHash[SHA_DIGEST_LENGTH * 2] = '\0';
//
//    // Also store the object using your runHashObject function
//    runHashObject(path);
//
//    free(buffer);
//    return hexHash;
//}
//
//char* runWriteTree(const char* dir) {
//    DIR *dirp;
//    struct dirent *dp;
//    struct stat st;
//
//    // Create an array to store entries
//    struct tree_entry *entries = malloc(100 * sizeof(struct tree_entry)); // Start with space for 100 entries
//    if (!entries) {
//        perror("Failed to allocate memory for tree entries");
//        return NULL;
//    }
//    int entry_count = 0;
//
//    if ((dirp = opendir(dir)) == NULL) {
//        fprintf(stderr, "Couldn't open %s: ", dir);
//        perror("");
//        free(entries);
//        return NULL;
//    }
//
//    while ((dp = readdir(dirp)) != NULL) {
//        // Skip . and .. directories
//        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
//            continue;
//
//        // Skip .gitflex directory
//        if (strcmp(dp->d_name, ".gitflex") == 0)
//            continue;
//
//        char path[512];
//        snprintf(path, sizeof(path), "%s/%s", dir, dp->d_name);
//
//        if (stat(path, &st) == 0) {
//            if (S_ISDIR(st.st_mode)) {
//                printf("%s is a directory\n", path);
//
//                // Recursively process subdirectory and get its tree hash
//                char *subdir_hash = runWriteTree(path);
//                if (subdir_hash) {
//                    // Store directory entry
//                    strcpy(entries[entry_count].mode, "40000");
//                    strcpy(entries[entry_count].name, dp->d_name);
//                    strcpy(entries[entry_count].hash, subdir_hash);
//                    entry_count++;
//                    free(subdir_hash);
//                }
//            } else if (S_ISREG(st.st_mode)) {
//                printf("%s is a file\n", path);
//
//                // Get file hash and store the blob
//                char *file_hash = getHashFromFile(path);
//                if (file_hash) {
//                    // Determine file mode (executable or regular)
//                    if (st.st_mode & S_IXUSR) {
//                        strcpy(entries[entry_count].mode, "100755"); // Executable
//                    } else {
//                        strcpy(entries[entry_count].mode, "100644"); // Regular file
//                    }
//                    strcpy(entries[entry_count].name, dp->d_name);
//                    strcpy(entries[entry_count].hash, file_hash);
//                    entry_count++;
//                    free(file_hash);
//                }
//            }
//        } else {
//            perror("stat failed");
//        }
//    }
//    closedir(dirp);
//
//    // If no entries, return NULL
//    if (entry_count == 0) {
//        free(entries);
//        return NULL;
//    }
//
//    // Sort entries by name (Git requirement)
//    qsort(entries, entry_count, sizeof(struct tree_entry), compare_entries);
//
//    // Now build the tree content
//    // First calculate the size
//    size_t total_size = 0;
//    for (int i = 0; i < entry_count; i++) {
//        // Format: [mode] [space] [name] [null byte] [20-byte SHA-1 binary hash]
//        total_size += strlen(entries[i].mode) + 1 + strlen(entries[i].name) + 1 + 20;
//    }
//
//    // Allocate memory for tree content
//    unsigned char *content = malloc(total_size);
//    if (!content) {
//        perror("Failed to allocate memory for tree content");
//        free(entries);
//        return NULL;
//    }
//
//    // Build tree content
//    unsigned char *p = content;
//    for (int i = 0; i < entry_count; i++) {
//        // Add mode and space
//        size_t mode_len = strlen(entries[i].mode);
//        memcpy(p, entries[i].mode, mode_len);
//        p += mode_len;
//        *p++ = ' ';
//
//        // Add name and null byte
//        size_t name_len = strlen(entries[i].name);
//        memcpy(p, entries[i].name, name_len);
//        p += name_len;
//        *p++ = '\0';
//
//        // Convert hash from hex to binary and add
//        unsigned char binary_hash[20];
//        for (int j = 0; j < 20; j++) {
//            sscanf(&entries[i].hash[j*2], "%2hhx", &binary_hash[j]);
//        }
//        memcpy(p, binary_hash, 20);
//        p += 20;
//    }
//
//    // Create header for tree object (like your blob header)
//    char header[64];
//    snprintf(header, sizeof(header), "tree %zu", total_size);
//    size_t headerLen = strlen(header) + 1; // +1 for null byte
//
//    // Create the full object with header + content
//    unsigned char *tree_obj = malloc(headerLen + total_size);
//    if (!tree_obj) {
//        perror("Failed to allocate memory for tree object");
//        free(content);
//        free(entries);
//        return NULL;
//    }
//    memcpy(tree_obj, header, headerLen);
//    memcpy(tree_obj + headerLen, content, total_size);
//
//    // Hash the tree object
//    unsigned char hash[SHA_DIGEST_LENGTH];
//    SHA1(tree_obj, headerLen + total_size, hash);
//
//    // Convert hash to hex
//    char *hexHash = malloc(SHA_DIGEST_LENGTH * 2 + 1);
//    if (!hexHash) {
//        perror("Failed to allocate memory for hash");
//        free(content);
//        free(tree_obj);
//        free(entries);
//        return NULL;
//    }
//
//    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
//        sprintf((char*)hexHash + (i * 2), "%02x", hash[i]);
//    }
//    hexHash[SHA_DIGEST_LENGTH * 2] = '\0';
//
//    // Create directory for the object
//    char dirPath[256];
//    snprintf(dirPath, sizeof(dirPath), ".gitflex/objects/%c%c/", hexHash[0], hexHash[1]);
//    if (mkdir(dirPath, 0777) == -1 /*&& errno != EEXIST*/) {
////        perror("Error creating directoryy");
//    } else {
//        printf("Directory %s created successfully\n", dirPath);
//    }
//
//    // Create file path
//    char filePath[256];
//    snprintf(filePath, sizeof(filePath), ".gitflex/objects/%c%c/%s", hexHash[0], hexHash[1], hexHash + 2);
//
//    // Compress the tree object
//    uLong compressedSize = compressBound(headerLen + total_size);
//    unsigned char *compressedTree = malloc(compressedSize);
//    if (!compressedTree) {
//        perror("Error allocating memory for compression");
//        free(content);
//        free(tree_obj);
//        free(hexHash);
//        free(entries);
//        return NULL;
//    }
//
//    if (compress((Bytef *)compressedTree, &compressedSize, (Bytef *)tree_obj, headerLen + total_size) != Z_OK) {
//        perror("Error compressing tree");
//        free(content);
//        free(tree_obj);
//        free(compressedTree);
//        free(hexHash);
//        free(entries);
//        return NULL;
//    }
//
//    // Write compressed tree object to file
//    FILE *outFile = fopen(filePath, "wb");
//    if (outFile) {
//        fwrite(compressedTree, sizeof(unsigned char), compressedSize, outFile);
//        fclose(outFile);
//        printf("Tree object stored: %s\n", hexHash);
//    } else {
//        perror("Error creating file");
//        free(content);
//        free(tree_obj);
//        free(compressedTree);
//        free(hexHash);
//        free(entries);
//        return NULL;
//    }
//
//    // Clean up
//    free(content);
//    free(tree_obj);
//    free(compressedTree);
//    free(entries);
//
//    return hexHash;
//}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        char* command = argv[1];
        
        if (!strcmp(command, "init")) {
            runInit();
        }
        else if (!strcmp(command, "hash-object")) {
            if (argc > 2) {
                runHashObject(argv[2]);
            } else {
                printf("Error: Please provide a file name.\n");
            }
        }
        else if (!strcmp(command, "cat-file")) {
            if (argc > 2) {
                runCatFile(argv[2]);
            } else {
                printf("Error: Please provide a hash.\n");
            }
        }
        else if (!strcmp(command, "write-tree")) {
            char *tree_hash = runWriteTree(".");
            if (tree_hash) {
                printf("%s\n", tree_hash);
                free(tree_hash);
            }
        }
        else {
            printf("Error: Unknown command '%s'\n", command);
        }
    } else {
        printf("Usage: gitflex <command> [args]\n");
        printf("Commands:\n");
        printf("  init                 Initialize a new repository\n");
        printf("  hash-object <file>   Hash a file\n");
        printf("  cat-file <hash>      Show contents of a file\n");
        printf("  write-tree           Write a tree object\n");
    }

    return 0;
}
