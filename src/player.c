#include "player.h"
#include "config.h"
#include "level.h"
#include "raylib.h"
#include <stdio.h>

void Player_draw(Player *p) {
  DrawTextureEx(p->sprite, p->pos, 0.0, 1.0, WHITE);
}

static int Player_can_jump(Player *p, Level *level) {
  return p->pos.y > LEVEL_HEIGHT * TILE_SPACE;
}

void Player_update(Player *p, Level *level, float delta) {
  p->velocity.x = 0;
  if (IsKeyDown(KEY_A)) {
    p->velocity.x = -PLAYER_SPEED * delta;
  }
  if (IsKeyDown(KEY_D)) {
    p->velocity.x = PLAYER_SPEED * delta;
  }
  p->pos.x += p->velocity.x;
  if (Player_can_jump(p, level)) {
    p->velocity.y = 0;
    if (IsKeyDown(KEY_W)) {
      p->velocity.y = PLAYER_SPEED * 2.0;
    }
  } else {
    p->velocity.y -= GRAVITY * delta;
  }
  p->pos.y -= p->velocity.y * delta;
}