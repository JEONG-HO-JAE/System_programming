#ifndef MYSTDIO_H
#define MYSTDIO_H

#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h>
#include <stdbool.h>

#define BUFSIZE 1024
#define EOF -1
#define stdin 0
#define stdout 1
#define stderr 2
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

typedef struct myFile {
    int fd;
    int pos;
    int size;
    int mode;
    int flag;
    char lastop;
    bool eof;
    char buffer[BUFSIZE];
} FILE;

FILE *fopen(const char *pathname, const char *mode);
int fread(void *ptr, int size, int nmemb, FILE *stream);
int fwrite(const void *ptr, int size, int nmemb, FILE *stream);
int fflush(FILE *stream);
int fseek(FILE *stream, int offset, int whence);
int feof(FILE *stream);
int fclose(FILE *stream);
void printf(const char *format, ...);
void int_to_string(int num, char *buffer);
char *fgets(char *str, int n, FILE *stream);

FILE *fopen(const char *pathname, const char *mode) {
    FILE *file = (FILE *)malloc(sizeof(FILE));
    if (!file) return NULL;

    int flags;
    if (strcmp(mode, "r") == 0) {
        flags = O_RDONLY;
    } else if (strcmp(mode, "r+") == 0) {
        flags = O_RDWR;
    } else if (strcmp(mode, "w") == 0) {
        flags = O_WRONLY | O_CREAT | O_TRUNC;
    } else if (strcmp(mode, "w+") == 0) {
        flags = O_RDWR | O_CREAT | O_TRUNC;
    } else if (strcmp(mode, "a") == 0) {
        flags = O_WRONLY | O_CREAT | O_APPEND;
    } else if (strcmp(mode, "a+") == 0) {
        flags = O_RDWR | O_CREAT | O_APPEND;
    } else {
        free(file);
        return NULL;
    }

    int fd = open(pathname, flags, 0666);
    if (fd == -1) {
        free(file);
        return NULL;
    }

    file->fd = fd;
    file->pos = 0;
    file->size = 0;
    file->mode = flags;
    file->flag = 0;
    file->lastop = 0;
    file->eof = false;
    memset(file->buffer, 0, BUFSIZE);

    return file;
}

int fread(void *ptr, int size, int nmemb, FILE *stream) {
    if (stream->mode & O_WRONLY) return 0;

    int bytes_to_read = size * nmemb;
    int bytes_read = read(stream->fd, ptr, bytes_to_read);

    if (bytes_read <= 0) {
        stream->eof = true;
    }

    return bytes_read / size;
}

int fwrite(const void *ptr, int size, int nmemb, FILE *stream) {
    if (stream->mode & O_RDONLY) return 0;

    int bytes_to_write = size * nmemb;
    int bytes_written = write(stream->fd, ptr, bytes_to_write);

    return bytes_written / size;
}

int fflush(FILE *stream) {
    if (stream->mode & O_RDONLY) return 0;

    // 현재는 버퍼링을 지원하지 않으므로, fsync를 사용하지 않음
    return 0;
}

int fseek(FILE *stream, int offset, int whence) {
    off_t result = lseek(stream->fd, offset, whence);
    return result == (off_t)-1 ? -1 : 0;
}

int feof(FILE *stream) {
    return stream->eof ? 1 : 0;
}

int fclose(FILE *stream) {
    int result = close(stream->fd);
    free(stream);
    return result == 0 ? 0 : EOF;
}

void int_to_string(int num, char *buffer) {
    int i = 0;
    int is_negative = 0;

    // Handle 0 explicitly
    if (num == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }

    // Handle negative numbers
    if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    // Convert integer to string
    while (num != 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }

    // Add minus sign for negative numbers
    if (is_negative) {
        buffer[i++] = '-';
    }

    // Add null terminator
    buffer[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    char temp;
    while (start < end) {
        temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
}

void printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format != '\0') {
        if (*format == '%') {
            format++;
            if (*format == 'd') {
                // Handle integer output
                int num = va_arg(args, int);
                char buffer[12]; // Maximum digits for an integer + sign + null terminator
                int_to_string(num, buffer);
                int i = 0;
                while (buffer[i] != '\0') {
                    write(stdout, &buffer[i], 1);
                    i++;
                }
            } else if (*format == 'c') {
                // Handle character output
                char ch = (char)va_arg(args, int);
                write(stdout, &ch, 1);
            } else if (*format == 's') {
                // Handle string output
                char *str = va_arg(args, char*);
                int i = 0;
                while (str[i] != '\0') {
                    write(stdout, &str[i], 1);
                    i++;
                }
            }
        } else {
            // Handle regular character output
            write(stdout, format, 1);
        }
        format++;
    }

    va_end(args);
}

char *fgets(char *str, int n, FILE *stream) {
    if (stream->mode & O_WRONLY) {
        // Cannot read from a write-only stream
        return NULL;
    }

    int i = 0;
    char c;
    while (i < n - 1) {
        int res = read(stream->fd, &c, 1);
        if (res == 1) {
            if (c == '\n') {
                break;
            }
            else str[i++] = c;
        } else if (res == 0) {
            // End of file
            stream->eof = true;
            break;
        } else {
            // Error
            return NULL;
        }
    }

    if (i == 0) {
        // No characters read
        return NULL;
    }

    str[i] = '\0';
    return str;
}

#endif // MYSTDIO_H
