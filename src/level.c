#include "level.h"
#include "config.h"
#include "raylib.h"
#include "worldfile.h"
#include <math.h>
#include <stdio.h>
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
void Level_draw(Level *level) {
  DrawTexture(level->sprite, -level->sprite_offset.x, -level->sprite_offset.y,
              WHITE);
}

/// Builds a background from the background sprite with the full level width.
/// bg_rect is filled with the playable background area (without the padding, if
/// any).
static Image build_background(Image bg_tile, Rectangle *bg_rect) {
  int lvl_height_pixels = LEVEL_HEIGHT * TILE_SIZE;

  int full_bg_height =
      lvl_height_pixels > bg_tile.height ? lvl_height_pixels : bg_tile.height;
  Image full_bg = GenImageColor(LEVEL_WIDTH * TILE_SIZE + WINDOW_WIDTH,
                                full_bg_height, WHITE);
  int amount = full_bg.width / bg_tile.width + 1;
  printf("amount: %d", amount);
  Rectangle src_rec = {0.0, 0.0, bg_tile.width, bg_tile.height};
  for (int i = 0; i < amount; ++i) {
    Rectangle dst_rec = {i * bg_tile.width, 0, bg_tile.width, bg_tile.height};
    ImageDraw(&full_bg, bg_tile, src_rec, dst_rec, WHITE);
  }
  if (bg_rect) {
    bg_rect->x = WINDOW_WIDTH / 2.0;
    bg_rect->width = LEVEL_WIDTH * TILE_SIZE;
    bg_rect->y = (full_bg_height - lvl_height_pixels) / 2.0;
    bg_rect->height = lvl_height_pixels;
  }
  return full_bg;
}
void Level_gen_texture(Level *level) {
  Image background_sprite = LoadImage("assets/background.png");
  Image dirt = LoadImage("assets/dirt.png");
  Image spike = LoadImage("assets/spike.png");
  Rectangle usable_bg = {0};
  Image background = build_background(background_sprite, &usable_bg);
  // ImageDrawRectangleRec(&background, usable_bg, WHITE);

  Image *curr;
  Rectangle src_rec = {0.0, 0.0, TILE_SIZE, TILE_SIZE};
  for (int x = 0; x < LEVEL_WIDTH; ++x) {
    for (int y = 0; y < LEVEL_HEIGHT; ++y) {
      switch (level->tiles[x][y]) {
      case TILE_SPACE:
      case TILE_SPAWN:
        continue;
        break;
      case TILE_BLOCK:
        curr = &dirt;
        break;
      case TILE_OBSTACLE:
        curr = &spike;
        break;
      }
      Rectangle src_rec = {0, 0, curr->width, curr->height};
      Rectangle dst_rec = {(float)x * TILE_SIZE + usable_bg.x,
                           (float)y * TILE_SIZE + usable_bg.y, TILE_SIZE,
                           TILE_SIZE};
      ImageDraw(&background, *curr, src_rec, dst_rec, WHITE);
    }
  }
  if (!ExportImage(background, "level.bmp")) {
    exit(3);
  }
  level->sprite = LoadTextureFromImage(background);
  level->sprite_offset = (Vector2){usable_bg.x, usable_bg.y};
  UnloadImage(background_sprite);
}
