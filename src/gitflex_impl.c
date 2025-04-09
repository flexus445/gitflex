#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <openssl/sha.h>
#include <zlib.h>
#include <dirent.h>
#include "../include/gitflex.h"

struct tree_entry {
    char mode[8];       // File mode (e.g., "100644" for regular file)
    char name[256];     // Entry name
    char hash[41];      // SHA-1 hash (40 hex chars + null terminator)
};

int compare_entries(const void *a, const void *b) {
    return strcmp(((struct tree_entry *)a)->name, ((struct tree_entry *)b)->name);
}

void runInit() {
    struct stat st;

    if (stat(".gitflex", &st) == 0) {
        printf("Error: .gitflex already initialized.\n");
        return;
    }

    if (mkdir(".gitflex", 0777) == -1) {
        perror("Error creating .gitflex");
        return;
    }

    mkdir(".gitflex/objects", 0777);
    mkdir(".gitflex/refs", 0777);
    mkdir(".gitflex/refs/heads", 0777);

    FILE *headFile = fopen(".gitflex/HEAD", "w");
    if (headFile) {
        fprintf(headFile, "ref: refs/heads/main\n");
        fclose(headFile);
    } else {
        perror("Error creating .gitflex/HEAD");
    }

    printf("Initialized empty GitFlex repository in .gitflex/\n");
}

void runHashObject(const char *fileName) {
    // ... rest of the implementation ...
}

void runCatFile(const char *hash) {
    // ... rest of the implementation ...
}

char* runWriteTree(const char* dir) {
    // ... rest of the implementation ...
} 