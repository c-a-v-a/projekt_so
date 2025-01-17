// Module that provides utility string functions

#ifndef STR_CREATOR_H
#define STR_CREATOR_H

#include <sys/types.h>

char* int_to_str(int x);
char* int_arr_to_str(int* xs, ssize_t n);

#endif
