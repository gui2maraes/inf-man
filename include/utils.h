#pragma once
#include <stdlib.h>
#include <stdio.h>

static inline void error_out(char *msg) {
    fprintf(stderr, "error: %s", msg);
    exit(1);
}