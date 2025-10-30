#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include "lexer/lexer.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "required argument missing!\n");
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        return 1;
    }

    char *pathname = argv[1];
    int fd = open(pathname, O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) /* To obtain file size */
    {
        perror("fstat");
        close(fd);
        return 1;
    }

    char *src = malloc(sb.st_size + 1);
    if (!src)
    {
        fprintf(stderr, "Error: memory allocation failed.\n\n");
        close(fd);
        return 1;
    }

    ssize_t bytes_read = read(fd, src, sb.st_size);
    close(fd);
    if (bytes_read == -1)
    {
        perror("read");
        free(src);
        return 1;
    }
    if (bytes_read < sb.st_size)
    {
        fprintf(stderr, "Error: partial read (%ld/%ld bytes)\n", bytes_read, (long)sb.st_size);
        free(src);
        return 1;
    }
    src[sb.st_size] = '\0';

    fprintf(stdout, "Tokens:\n");
    Token tok;
    fprintf(stdout, "Line\t\tColumn\t\tToken\t\tvalue\n");
    while ((tok = get_token(src)).type != NULLP)
    {
        fprintf(stdout, "%d\t\t%d\t\t%s\t\t%s\n", tok.line, tok.column, type_name[tok.type], tok.value);
    }

    free(src);
    return 0;
}