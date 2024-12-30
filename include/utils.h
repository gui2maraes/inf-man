#pragma once
#include <stdio.h>
#include <stdlib.h>

static inline void error_out(char *msg) {
  fprintf(stderr, "error: %s", msg);
  exit(1);
}

typedef enum Direction {
  DIR_LEFT = -1,
  DIR_RIGHT = 1,
} Direction;