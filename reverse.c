#reverse c program

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE *in = stdin, *out = stdout;

    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        exit(1);
    }

    if (argc >= 2) {
        in = fopen(argv[1], "r");
        if (!in) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[1]);
            exit(1);
        }
    }

    if (argc == 3) {
        if (strcmp(argv[1], argv[2]) == 0) {
            fprintf(stderr, "Input and output file must differ\n");
            exit(1);
        }
        out = fopen(argv[2], "w");
        if (!out) {
            fprintf(stderr, "error: cannot open file '%s'\n", argv[2]);
            exit(1);
        }
    }

    char **lines = NULL;
    size_t n = 0, cap = 0;
    char *line = NULL;
    size_t len = 0;

    while (getline(&line, &len, in) != -1) {
        if (n == cap) {
            cap = cap ? cap * 2 : 16;
            lines = realloc(lines, cap * sizeof(char *));
            if (!lines) {
                fprintf(stderr, "malloc failed\n");
                exit(1);
            }
        }
        lines[n++] = strdup(line);
    }

    free(line);

    for (ssize_t i = n - 1; i >= 0; i--) {
        fprintf(out, "%s", lines[i]);
        free(lines[i]);
    }

    free(lines);
    if (in != stdin) fclose(in);
    if (out != stdout) fclose(out);
    return 0;
}
