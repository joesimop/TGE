#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


//HELPERS
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define TYPEOF(_t) __typeof__(_t)

#define MIN(_a, _b) ({ __typeof__(_a) __a = (_a), __b = (_b); __a < __b ? __a : __b; })
#define MAX(_a, _b) ({ __typeof__(_a) __a = (_a), __b = (_b); __a > __b ? __a : __b; })
#define CLAMP(_x, _mi, _ma) (MIN(MAX(_x, _mi), _ma))

//Chekcs if variadic macros are empty or not
#define is_empty(...) ( sizeof( (char[]){#__VA_ARGS__} ) == 1 )

//Assert
#define ASSERT(condition, ...)                                                                                 \
    do {                                                                                                       \
        if (!(condition)) {                                                                                    \
            fprintf(stderr, "\n**********************************************************\n");                 \
            fprintf(stderr, "Assertion failed: %s\n", #condition);                                             \
            fprintf(stderr, "Error: %s\n", is_empty(__VA_ARGS__) ? "No message" : #__VA_ARGS__);               \
            fprintf(stderr, "File: %s, Line: %d\n", __FILE__, __LINE__);                                       \
            fprintf(stderr, "**********************************************************\n");                   \
            exit(EXIT_FAILURE);                                                                                \
        }                                                                                                      \
    } while (0)



