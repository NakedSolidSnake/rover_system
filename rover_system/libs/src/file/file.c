#include <stdlib.h>
#include <file/file.h>

FILE *openWrite(const char *filename)
{
  FILE *fd = fopen(filename, "w");
  return fd;
}

FILE *openRead(const char *filename)
{
  FILE *fd = fopen(filename, "r");
  return fd;
}

size_t getFileSize(FILE *file)
{
  size_t len = 0;
  if(!file)
    return -1;
  fseek(file, 0, SEEK_END);
  len = ftell(file);
  rewind(file);
  return len;
}
