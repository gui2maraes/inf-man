#include "player.h"
#include "config.h"
#include "level.h"
#include "raylib.h"

static Vector2 Player_feet(Player *p) {
  Vector2 translated = p->pos;
  translated.y += PLAYER_SIZE / 2.0;
  return translated;
}
static Vector2 Player_edge(Player *p, int x_sign) {
  Vector2 translated = p->pos;
  translated.x += (PLAYER_SIZE / 2.0) * x_sign;
  return translated;
}
void Player_draw(Player *p) {
  Vector2 translated = p->pos;
  translated.x -= p->sprite.width / 2.0;
  translated.y -= p->sprite.height / 2.0;
  DrawTextureEx(p->sprite, translated, 0.0, 1.0, WHITE);
  if (DEBUG_MODE) {
    DrawCircleV(p->pos, PLAYER_SIZE / 10.0, RED);
    DrawCircleV(Player_feet(p), PLAYER_SIZE / 20.0, GREEN);
    DrawCircleV(Player_edge(p, -1), PLAYER_SIZE / 20.0, BLUE);
    DrawCircleV(Player_edge(p, 1), PLAYER_SIZE / 20.0, BLUE);
  }
}

static int Player_can_jump(Player *p, Level *level) {
  return Level_is_tile(level, Player_feet(p), TILE_BLOCK);
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
    if (IsKeyPressed(KEY_W)) {
      p->velocity.y = PLAYER_SPEED * 2.5;
    }
  } else {
    p->velocity.y -= GRAVITY * delta;
  }
  p->pos.y -= p->velocity.y * delta;
}