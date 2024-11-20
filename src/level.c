#include "level.h"
#include "config.h"
#include "raylib.h"
#include "worldfile.h"
#include <stdlib.h>

int Level_init(Level *level, char *worldfile) {
  *level = (Level){0};
  if (!read_worldfile(worldfile, level)) {
    return 0;
  }
  Level_gen_texture(level);
  return 1;
}
Vector2 Level_matrix_to_world(int line, int column) {
  Vector2 v = {column, LEVEL_HEIGHT - line - 1};
  return v;
}
void Level_gen_texture(Level *level) {
  Image dirt = LoadImage("assets/dirt.png");
  Image sky = LoadImage("assets/sky.png");
  Image lava = LoadImage("assets/lava.png");
  Image lvl =
      GenImageColor(LEVEL_WIDTH * TILE_SIZE, LEVEL_HEIGHT * TILE_SIZE, WHITE);

  Image *curr;
  Rectangle src_rec = {0.0, 0.0, TILE_SIZE, TILE_SIZE};
  for (int x = 0; x < LEVEL_WIDTH; ++x) {
    for (int y = 0; y < LEVEL_HEIGHT; ++y) {
      switch (level->tiles[x][y]) {
      case TILE_SPACE:
      case TILE_SPAWN:
        curr = &sky;
        break;
      case TILE_BLOCK:
        curr = &dirt;
        break;
      case TILE_OBSTACLE:
        curr = &lava;
        break;
      }
      Rectangle src_rec = {0, 0, curr->width, curr->height};
      Rectangle dst_rec = {(float)x * TILE_SIZE, (float)y * TILE_SIZE,
                           TILE_SIZE, TILE_SIZE};
      ImageDraw(&lvl, *curr, src_rec, dst_rec, WHITE);
    }
  }
  if (!ExportImage(lvl, "level.bmp")) {
    exit(3);
  }
  level->sprite = LoadTextureFromImage(lvl);
}