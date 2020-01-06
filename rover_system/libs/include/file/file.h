#ifndef __FILE_H
#define __FILE_H

#include <stdio.h>

FILE *openWrite(const char *filename);
FILE *openRead(const char *filename);
size_t getFileSize(FILE *file);

#endif
