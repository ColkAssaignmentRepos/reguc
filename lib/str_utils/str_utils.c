//
// Created by Manaki ITO on 2021/06/17.
//
#ifndef INCLUDED_STR_UTILS
#define INCLUDED_STR_UTILS

#include <stdlib.h>
#include <string.h>

#include "../print_utils.h"

int find_char(char *string, long long int length, char query);

int delete_char(char *string, long long int length, char query);

int replace_char(char *string, long long int start_index, long long int end_index, char replace_with);

char **malloc_char_matrix(long long int row_length);

char *malloc_extract_string(char *source, long long int start_index, long long int end_index);

char *malloc_expanded_char_matrix(char **source, long long int row_length);

void char_matrix_free(char **target, long long int row_length);


int find_char(char *string, long long int length, char query) {
    long long int i = 0;

    while (i < length) {
        if (string[i] == query) {
            return 1;
        }
        i++;
    }

    return 0;
}


int delete_char(char *string, long long int length, char query) {
    char *temp;
    long long int size = length * sizeof(char);

    temp = (char *) malloc(size);
    if (temp == NULL) {
        fatal_with_message(1,
                           "[X] Unexpected error in %s(): Failed to allocate memory.\n",
                           __func__);
    }

    memset(temp, '\0', size);

    for (int i = 0; i < length; i++) {
        if (string[i] != query) {
            temp[i] = string[i];
        }
    }

    memcpy(string, temp, length * sizeof(char));

    free(temp);

    return 0;
}


int replace_char(char *string, long long int start_index, long long int end_index, char replace_with) {
    long long int length = strlen(string);
    long long int index_sub = end_index - start_index;

    if ((start_index < 0 || end_index < 0) || (start_index > length || end_index > length) || (index_sub < 0)) {
        return -1;
    }

    memset(&string[start_index], replace_with, sizeof(char) * index_sub);
    return 0;
}


char **malloc_char_matrix(long long int row_length) {
    char **result = NULL;
    result = (char **) malloc(sizeof(char *) * row_length);
    if (result == NULL) {
        fatal_with_message(1,
                           "[X] Unexpected error in %s(): Failed to allocate memory.\n",
                           __func__);
    }

    return result;
}


char *malloc_extract_string(char *source, long long int start_index, long long int end_index) {
    long long int index_sub = end_index - start_index;
    char *result = NULL;

    result = malloc(sizeof(char) * (index_sub + 1));
    if (result == NULL) {
        fatal_with_message(1,
                           "[X] Unexpected error in %s(): Failed to allocate memory.\n",
                           __func__);
    }

    memset(result, '\0', sizeof(char) * (index_sub + 1));

    strncpy(result, &source[start_index], sizeof(char) * index_sub);

    return result;
}


char *malloc_expanded_char_matrix(char **source, long long int row_length) {
    char *result = NULL;
    long long int length_in_total = 0;
    long long int character_count = 0;

    for (long long int i = 0; i < row_length; i++) {
        length_in_total += strlen(source[i]);
    }

    result = malloc(sizeof(char) * (length_in_total + 1));
    if (result == NULL) {
        fatal_with_message(1,
                           "[X] Unexpected error in %s(): Failed to allocate memory.\n",
                           __func__);
    }

    for (long long int i = 0; i < row_length; i++) {
        strcpy(&result[character_count], source[i]);
        character_count += strlen(source[i]);
    }

    char_matrix_free(source, row_length);

    return result;
}

void char_matrix_free(char **target, long long int row_length) {
    for (long long int i = 0; i < row_length; i++) {
        free(target[i]);
    }

    free(target);
}

#endif // INCLUDED_STR_UTILS
