#include "worldfile.h"
#include "config.h"
#include "level.h"
#include "utils.h"
#include <stdio.h>

#define LINE_WIDTH (LEVEL_WIDTH + 2)

static int proc_line(Level *level, int line_i, char *line, int *spawn_set) {
  int i = 0;
  for (i = 0; line[i] != '\n' && line[i] != '\0'; ++i) {
    char c = line[i];
    Tile t;

    switch (c) {
    case ' ':
      t = TILE_SPACE;
      break;
    case 'B':
      t = TILE_BLOCK;
      break;
    case 'O':
      t = TILE_OBSTACLE;
      break;
    case 'M':
      error_out("TODO");
      break;
    case 'P':
      t = TILE_SPACE;
      if (*spawn_set) {
        fprintf(stderr, "error: spawn point set twice in worldfile.");
        return 0;
      }
      level->spawn_point = Level_matrix_to_world(line_i, i);
      *spawn_set = 1;
      break;
    default:
      fprintf(stderr, "error: invalid worldfile character in line %d: `%c`",
              line_i, c);
      return 0;
    }
    level->tiles[i][line_i] = t;
  }
  if (i < LEVEL_WIDTH) {
    for (; i < LEVEL_WIDTH; ++i) {
      level->tiles[i][line_i] = TILE_SPACE;
    }
  }
  return 1;
}

/// Reads a worldfile into level data.
/// The worldfile should be a character matrix
/// with width and height compatible with the level
/// configuration.
/// Returns 0 on failure and non-zero on success.
int read_worldfile(char *filepath, Level *level) {

  FILE *f = fopen(filepath, "r");
  int spawn_point_set = 0;
  char line[LINE_WIDTH] = {0};

  if (!f) {
    perror("error opening worldfile");
    return 0;
  }

  for (int i = 0; i < LEVEL_HEIGHT; ++i) {
    if (!fgets(line, LINE_WIDTH, f)) {
      // if line is missing, report wrong file height and error out
      if (feof(f)) {
        fprintf(stderr, "error: invalid worldfile height. \
              Expected %d lines, got %d",
                LEVEL_HEIGHT, i);
      } else {
        // if another error, report it
        perror("error reading worldfile line");
      }
      return 0;
    }

    if (!proc_line(level, i, line, &spawn_point_set)) {
      return 0;
    }
  }
  if (!spawn_point_set) {
    fprintf(stderr, "error: spawn point was not set in worldfile.");
    return 0;
  }
  return 1;
}
