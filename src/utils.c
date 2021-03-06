#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "utils.h"

extern void *
emalloc(size_t size) {
    void * p = malloc(size);
    if (!p) exit(1);
    return p;
}

static off_t
getFileSize(const char *path) {
    struct stat st;
    if (stat(path, &st)) die("Can't get file size: %s", strerror(errno));
    return st.st_size;
}

#define KB 1024
#define MB (1024 * KB)

#define MAX_FILE_SIZE (4 * MB)

extern char *
loadFile(const char *path) {
    off_t size = getFileSize(path);
    if (MAX_FILE_SIZE < size) die("File is too big.");
    FILE *f = fopen(path, "r");
    if (!f) die("Can't open file: %s", strerror(errno));
    char *content = emalloc(size + 1);
    if (1 != fread(content, size, 1, f)) {
        die("Can't read file: %s", strerror(errno));
    }
    fclose(f);
    content[size] = 0;
    return content;
}

extern void
die(const char *format, ...) {
    va_list vargs;
    va_start(vargs, format);
    vfprintf(stderr, format, vargs);
    fprintf(stderr, "\n");
    va_end(vargs);
    exit(1);
}

extern uintmax_t
stringHash(const char *str) {
    uintmax_t hash = 5381;
    int c;
    while ((c = *(const unsigned char *)str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}
