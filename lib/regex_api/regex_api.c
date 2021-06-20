//
// Created by Manaki ITO on 2021/06/19.
//
#ifndef INCLUDED_REGEX_API
#define INCLUDED_REGEX_API

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "../str_utils/str_utils.h"
#include "../print_utils.h"

#define SUCCESS 0
#define NO_FLAGS 0
#define ERROR_MESSAGE_BUFFER_SIZE 128
#define REGEX_TEMP_REPLACE_WITH '\r'

typedef struct {
    long long int start;
    long long int end;
} se_index_t;

typedef struct {
    se_index_t *index_array;
    long long int match_num;
} regmatchn_result_t;

void __regcomp(char *pattern_str, regex_t *target);

regmatchn_result_t malloc_regmatchn(regex_t *regex, char *string);

regmatchn_result_t malloc_regmatchn_regex_str(char pattern_str[], char *string);

char **malloc_extract_regmatchn_result(char *source, regmatchn_result_t indexes);

void regmatchn_free(regmatchn_result_t *target);


void __regcomp(char *pattern_str, regex_t *target) {
    if (regcomp(target, pattern_str, REG_EXTENDED | REG_NEWLINE) != 0) {
        fatal_with_message(1,
                           "[X] Unexpected error in %s(): External function returned an error.\n"
                           "Regex is unavailable. Make sure the pattern string is correct.\n",
                           __func__);
    }
}


regmatchn_result_t malloc_regmatchn(regex_t *regex, char *string) {
    long long int i = 0;
    long long int regex_groups_num = regex->re_nsub;
    char *temp = NULL;
    int err_number;

    long long int str_length = strlen(string);

    regmatch_t *match;
    se_index_t *match_indexes = NULL;
    regmatchn_result_t result;

    temp = (char *) malloc(sizeof(char) * (str_length + 1));
    if (temp == NULL) {
        fatal_with_message(1,
                           "[X] Unexpected error in %s(): Failed to allocate memory.\n",
                           __func__);
    }

    match = (regmatch_t *) malloc(sizeof(regex_t) * (regex_groups_num + 1));
    if (match == NULL) {
        fatal_with_message(1,
                           "[X] Unexpected error in %s(): Failed to allocate memory.\n",
                           __func__);
    }

    strcpy(temp, string);

    while (1) {
        err_number = regexec(regex, temp, regex_groups_num + 1, match, NO_FLAGS);
        if (err_number == REG_NOMATCH) {
            break;
        }

        if (err_number != SUCCESS) {
            char err_buf[ERROR_MESSAGE_BUFFER_SIZE];

            regerror(err_number, regex, err_buf, ERROR_MESSAGE_BUFFER_SIZE);
            fatal_with_message(1, "[X] Unexpected error in %s(): External function returned an error.\n"
                                  "%s() says: %s",
                                  __func__, "regerror()", err_buf);
        }

        replace_char(temp, match[0].rm_so, match[0].rm_eo, REGEX_TEMP_REPLACE_WITH);

        i++;

        if (match_indexes == NULL) {
            match_indexes = (se_index_t *) malloc(sizeof(se_index_t) * i);

            if (match_indexes == NULL) {
                fatal_with_message(1,
                                   "[X] Unexpected error in %s(): Failed to allocate memory.\n",
                                   __func__);
            }
        }

        match_indexes = (se_index_t *) realloc(match_indexes, sizeof(se_index_t) * i);

        match_indexes[i - 1].start = match[0].rm_so;
        match_indexes[i - 1].end = match[0].rm_eo;
    }

    result.index_array = match_indexes;
    result.match_num = i;

    free(temp);

    return result;
}


regmatchn_result_t malloc_regmatchn_regex_str(char pattern_str[], char *string) {
    regex_t compiled_regex;
    regmatchn_result_t result;

    __regcomp(pattern_str, &compiled_regex);
    result = malloc_regmatchn(&compiled_regex, string);

    regfree(&compiled_regex);

    return result;
}


char **malloc_extract_regmatchn_result(char *source, regmatchn_result_t indexes) {
    char **result = NULL;

    result = (char **) malloc(sizeof(char *) * indexes.match_num);
    if (result == NULL) {
        fatal_with_message(1,
                           "[X] Unexpected error in %s(): Failed to allocate memory.\n",
                           __func__);
    }

    for (long long int i = 0; i < indexes.match_num; i++) {
        result[i] = malloc_extract_string(source, indexes.index_array[i].start, indexes.index_array[i].end);
    }

    return result;
}


void regmatchn_free(regmatchn_result_t *target) {
    free(target->index_array);
}

#endif // INCLUDED_REGEX_API
