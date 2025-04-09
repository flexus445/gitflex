#ifndef GITFLEX_H
#define GITFLEX_H

void runInit();
void runHashObject(const char *fileName);
void runCatFile(const char *hash);
char* runWriteTree(const char *dir);

#endif // GITFLEX_H 