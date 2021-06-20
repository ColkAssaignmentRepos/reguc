//
// Created by Manaki ITO on 2021/06/20.
//

#ifndef REGUC_PRINT_UTILS_H
#define REGUC_PRINT_UTILS_H

#define print_green(...) {printf("\x1b[32m"); printf(__VA_ARGS__); printf("\x1b[39m\n");}
#define print_error(...) {fprintf(stderr, "\x1b[31m"); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\x1b[39m\n");}
#define fatal_with_message(exit_code, ...) {print_error(__VA_ARGS__); exit(exit_code);}
#define blank() {printf("\n");}

#endif // REGUC_PRINT_UTILS_H
