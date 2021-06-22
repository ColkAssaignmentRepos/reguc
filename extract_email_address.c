#include <stdio.h>
#include <regex.h>
#include <stdlib.h>

#include "./lib/file_api/file_api.h"
#include "./lib/regex_api/regex_api.h"
#include "./lib/print_utils.h"

#define PATTERN_STRING "[a-zA-Z0-9_+-]+(.[a-zA-Z0-9_+-]+)*@([a-zA-Z0-9][a-zA-Z0-9-]*[a-zA-Z0-9]*\\.)+[a-zA-Z]{2,}"


int main(int argc, char **argv) {
    char *filename = NULL;
    char *file_content = NULL;

    regmatchn_result_t regmatch_result;
    char **extracted_result = NULL;

    if (argc != 2) {
        fatal_with_message(1,
                           "[X] Unexpected error in %s(): Unexpected parameter has been passed.\n"
                           "Expected only one 2 (including executable file itself), but got %d arguments.",
                           __func__ , argc)
    }

    filename = argv[1];

    file_content = malloc_read_file_in_one_array(filename);
    regmatch_result = malloc_regmatchn_regex_str(PATTERN_STRING, file_content);
    extracted_result = malloc_extract_regmatchn_result(file_content, regmatch_result);

    if (regmatch_result.match_num == 0) {
        print_green("[i] No match found.");
        return 0;
    }

    print_green("[i] %lld match(es) found in \"%s\".",
                regmatch_result.match_num, filename);

    for (long long int i = 0; i < regmatch_result.match_num; i++) {
        printf("%s\n", extracted_result[i]);
    }

    return 0;
}
