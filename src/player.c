#include "player.h"
#include "collision.h"
#include "config.h"
#include "level.h"
#include "raylib.h"

Rectangle Player_hitbox(Player *p, Tile tile) {
  float side = PLAYER_SIZE;
  if (tile == TILE_OBSTACLE) {
    side *= 0.8;
  }
  return (Rectangle){
      .x = p->pos.x, .y = p->pos.y, .width = side, .height = side};
}
int Player_is_dead(Player *p) { return p->health <= 0; }

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

void Player_update(Player *p, Level *level, float delta) {
  if (p->pos.y > LEVEL_HEIGHT * TILE_SIZE + PLAYER_SIZE) {
    p->health = 0;
  }
  if (Player_is_dead(p)) {
    return;
  }
  p->velocity.x = 0;
  if (IsKeyDown(KEY_A)) {
    p->velocity.x = -PLAYER_SPEED * delta;
  }
  if (IsKeyDown(KEY_D)) {
    p->velocity.x = PLAYER_SPEED * delta;
  }
  p->pos.x += p->velocity.x;
  p->pos.y -= p->velocity.y * delta;

  CollisionMap collisions =
      resolve_collisions(level, Player_hitbox(p, TILE_BLOCK), TILE_BLOCK);
  Vector2 aligned = Level_align_coord_tile_center(p->pos);
  if (collisions & COLLISION_LEFT) {
    p->pos.x = aligned.x - TILE_SIZE / 2.0 + PLAYER_SIZE / 2.0;
  }
  if (collisions & COLLISION_RIGHT) {
    p->pos.x = aligned.x + TILE_SIZE / 2.0 - PLAYER_SIZE / 2.0;
  }
  if (collisions & COLLISION_UP) {
    p->pos.y = aligned.y - TILE_SIZE / 2.0 + PLAYER_SIZE / 2.0;
    p->velocity.y = 0;
  }
  if (p->pos.x < PLAYER_SIZE / 2.0) {
    p->pos.x = PLAYER_SIZE / 2.0;
  }

  if (collisions & COLLISION_DOWN) {
    p->pos.y = aligned.y + TILE_SIZE / 2.0 - PLAYER_SIZE / 2.0;
    p->velocity.y = 0;
    if (IsKeyPressed(KEY_W)) {
      p->velocity.y = PLAYER_SPEED * 2.5;
    }
  } else {
    p->velocity.y -= GRAVITY * delta;
  }
}