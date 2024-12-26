#include "level.h"
#include "config.h"
#include "raylib.h"
#include "worldfile.h"
#include <math.h>
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
  float scale = TILE_SIZE;
  Vector2 v = {(column * scale) + TILE_SIZE / 2.0,
               (line * scale) + TILE_SIZE / 2.0};
  return v;
}

int Level_world_to_matrix(Vector2 v, int *line, int *column) {
  int x = v.x / TILE_SIZE; // Converte posição em coordenada de tile
  int y = v.y / TILE_SIZE;

  // Verifica se está fora dos limites do nível
  if (x < 0 || x >= LEVEL_WIDTH || y < 0 || y >= LEVEL_HEIGHT) {
    return 0; // Fora dos limites, não é sólido
  }
  *line = y;
  *column = x;
  return 1;
}
Vector2 Level_align_coord_tile_center(Vector2 coords) {
  int line, column;
  if (Level_world_to_matrix(coords, &line, &column)) {
    return Level_matrix_to_world(line, column);
  }
  return (Vector2){0};
  // coords.x = floorf(coords.x / TILE_SIZE) + TILE_SIZE / 2.0;
  // coords.y = floorf(coords.y / TILE_SIZE) + TILE_SIZE / 2.0;
}

int Level_coord_is_tile(Level *l, Vector2 v, Tile tile) {
  int line, column;
  if (!Level_world_to_matrix(v, &line, &column)) {
    return 0;
  }
  return l->tiles[column][line] == tile;
}
void Level_draw(Level *level) { DrawTexture(level->sprite, 0, 0, WHITE); }
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
