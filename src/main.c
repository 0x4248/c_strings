/* C Strings
 * The strings command written in C
 * Github: https://www.github.com/lewisevans2007/c_strings
 * Licence: GNU General Public License v3.0
 * By: Lewis Evans
 */

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "colour.h"

#define VERSION "1.1.0"

void print_help() {
    printf("Usage: strings [options] [file]\n");
    printf("Strings is a command line tool that prints all printable strings "
           "that are found in a file.\n");
    printf("Options:\n");
    printf("  -ln, --line-numbers\t\tShow line numbers\n");
    printf("  -c, --context\t\t\tShow context of each string\n");
    printf("  -h, --help\t\t\tShow this help message\n");
    printf("  -v, --version\t\t\tShow version\n");
}

int main(int argc, char *argv[]) {
    bool show_byte_position = false;
    bool show_line_numbers = false;
    bool show_context = false;
    if (argc == 1) {
        printf("Usage: strings [options] [file]\n");
        return 1;
    }

    if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
        printf("strings version %s\n", VERSION);
        return 0;
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        print_help();
        return 0;
    }

    if (argc > 2) {
        if (strcmp(argv[2], "-ln") == 0 ||
            strcmp(argv[2], "--line-numbers") == 0) {
            show_line_numbers = true;
        }
        if (strcmp(argv[2], "-c") == 0 || strcmp(argv[2], "--context") == 0) {
            show_context = true;
        }
        if (strcmp(argv[2], "-b") == 0) {
            show_byte_position = true;
        }
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("%sError%s: ", ANSI_COLOR_RED, ANSI_COLOR_RESET);
        if (errno == ENOENT) {
            printf("File not found\n");
        } else {
            printf("Unknown error\n");
        }
        return 1;
    }
    char *line = NULL;
    size_t len = 0;
    size_t read;
    int line_number = 0;
    int byte_position = 0;

    while ((read = getline(&line, &len, file)) != -1) {
        line_number++;
        for (int i = 0; i < read; i++) {
            int printable_chars = 0;
            int start_index = i;
            while (isprint(line[i])) {
                printable_chars++;
                i++;
            }

            if (printable_chars > 2) {
                if (show_line_numbers) {
                    printf("%s%d%s | ", ANSI_COLOR_CYAN, line_number,
                           ANSI_COLOR_RESET);
                }
                if (show_byte_position) {
                    printf("%s0x%08x%s | ", ANSI_COLOR_YELLOW, byte_position,
                           ANSI_COLOR_RESET);
                }

                if (show_context) {
                    int context_start = start_index - 5;
                    if (context_start < 0) {
                        context_start = 0;
                    }
                    int context_end = start_index + printable_chars + 5;
                    if (context_end > read) {
                        context_end = read;
                    }
                    for (int j = context_start; j < start_index; j++) {
                        if (isprint(line[j])) {
                            printf("%c", line[j]);
                        } else {
                            printf("%s", ANSI_COLOR_GRAY);
                            printf(".");
                            printf("%s", ANSI_COLOR_RESET);
                        }
                    }
                }

                for (int j = start_index; j < start_index + printable_chars;
                     j++) {
                    printf("%c", line[j]);
                }

                if (show_context) {
                    int context_start = start_index - 5;
                    if (context_start < 0) {
                        context_start = 0;
                    }
                    int context_end = start_index + printable_chars + 5;
                    if (context_end > read) {
                        context_end = read;
                    }
                    printf("%s", ANSI_COLOR_GRAY);
                    for (int j = start_index + printable_chars; j < context_end;
                         j++) {
                        if (isprint(line[j])) {
                            printf("%c", line[j]);
                        } else {
                            printf(".");
                        }
                    }
                    printf("%s", ANSI_COLOR_RESET);
                }

                printf("\n");
            }
            byte_position += printable_chars;
        }
    }
}