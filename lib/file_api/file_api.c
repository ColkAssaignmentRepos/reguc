//
// Created by Manaki ITO on 2021/06/17.
//
#ifndef INCLUDED_FILE_API
#define INCLUDED_FILE_API

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../str_utils/str_utils.h"
#include "../print_utils.h"

#define DEFAULT_STR_BUFFER_LENGTH 512
#define DEFAULT_BUFFER_BYTES DEFAULT_STR_BUFFER_LENGTH * sizeof(char)

char **malloc_read_file_lines(char *filename);

char *malloc_read_file_in_one_array(char *filename);

long long int line_number(char *filename);


char **malloc_read_file_lines(char *filename) {
    FILE *fp = NULL;
    char **result = NULL;
    char *current_line = NULL;
    char *buf = NULL;
    long long int lines;
    long long int line_count = 0;
    long long int current_buffer_bytes = -1;
    int reached_eof = 0;

    lines = line_number(filename);

    fp = fopen(filename, "r");
    if (fp == NULL) {
        fatal_with_message(1,
                           "[X] Error: Failed to open \"%s\".\n"
                           "Make sure the file name is correct and not accessed by any other process.\n",
                           filename);
    }

    result = malloc_char_matrix(lines);
    if (result == NULL) {
        return NULL;
    }

    buf = (char *) malloc(sizeof(char) * DEFAULT_BUFFER_BYTES);
    if (buf == NULL) {
        fatal_with_message(1,
                           "[X] Unexpected error in %s(): Failed to allocate memory.\n",
                           __func__);
    }

    while (1) {
        current_buffer_bytes = DEFAULT_BUFFER_BYTES;

        current_line = (char *) malloc(current_buffer_bytes);
        if (current_line == NULL) {
            fatal_with_message(1,
                               "[X] Unexpected error in %s(): Failed to allocate memory.\n",
                               __func__);
        }

        if (fgets(current_line, DEFAULT_BUFFER_BYTES, fp) == NULL) {
            reached_eof = 1;
        }

        while (1) {
            int continue_required = !(
                    find_char(current_line, current_buffer_bytes, '\n') ||
                    find_char(current_line, current_buffer_bytes, EOF)) && (!reached_eof);
            if (!continue_required) {
                break;
            }

            if (fgets(buf, DEFAULT_BUFFER_BYTES, fp) == NULL) {
                reached_eof = 1;
            }

            current_buffer_bytes += DEFAULT_STR_BUFFER_LENGTH;
            current_line = realloc(current_line, current_buffer_bytes);
            if (current_line == NULL) {
                fatal_with_message(1,
                                   "[X] Unexpected error in %s(): Failed to allocate memory.\n",
                                   __func__);
            }

            strcat(current_line, buf);
            memset(buf, '\0', DEFAULT_BUFFER_BYTES);
        }

        result[line_count] = (char *) malloc(current_buffer_bytes);
        memcpy(result[line_count], current_line, current_buffer_bytes);

        free(current_line);

        if (reached_eof) {
            break;
        }

        line_count++;
    }

    fclose(fp);

    return result;
}


char *malloc_read_file_in_one_array(char *filename) {
    char **read_lines = NULL;
    char *result = NULL;
    long long int line_num = line_number(filename);

    read_lines = malloc_read_file_lines(filename);
    result = malloc_expanded_char_matrix(read_lines, line_num);

    return result;
}


long long int line_number(char *filename) {
    FILE *fp;
    size_t read_size;
    char buf[DEFAULT_BUFFER_BYTES];
    long long int lines = 0;

    if ((fp = fopen(filename, "r")) == NULL) {
        fatal_with_message(1,
                           "[X] Unexpected error: Failed to allocate memory.\n"
                           "Make sure the file name is correct and not accessed by any other process.\n"
                           "Aborting")
    }

    while ((read_size = fread(buf, 1, DEFAULT_BUFFER_BYTES, fp)) > 0) {

        for (size_t i = 0; i < read_size; i++) {

            if (buf[i] == '\n') {
                lines++;
            }

        }

    }

    lines++;

    fclose(fp);

    return lines;
}

#endif // INCLUDED_FILE_API
