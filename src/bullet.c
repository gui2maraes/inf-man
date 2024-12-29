#include "bullet.h"
#include "config.h"
#include "game.h"
#include "player.h"
#include <math.h>

Rectangle Bullet_hitbox(Bullet *bullet) {
  Rectangle rec;
  rec.x = bullet->pos.x;
  rec.y = bullet->pos.y;
  rec.width = BULLET_WIDTH;
  rec.height = BULLET_HEIGHT;
  return rec;
}

void Bullet_update(Bullet *bullet, Player *p, float delta) {
  if (fabs(bullet->pos.x - p->pos.x) > WINDOW_WIDTH / GAME_SCALE / 2.0) {
    bullet->alive = false;
  }
  if (!bullet->alive) {
    return;
  }
  bullet->pos.x += bullet->velocity * delta;
}
void Bullets_update(Game *game, float delta) {
  for (int i = 0; i < MAX_BULLETS; ++i) {
    Bullet_update(&game->bullets[i], &game->player, delta);
  }
}

void Bullet_draw(Bullet *bullet) {
  if (bullet->alive) {
    DrawRectangleRec(Bullet_hitbox(bullet), YELLOW);
  }
}
void Bullets_draw(Game *game) {
  for (int i = 0; i < MAX_BULLETS; ++i) {
    Bullet_draw(&game->bullets[i]);
  }
}
void Bullet_spawn(Game *game) {
  Bullet *b = 0;
  for (int i = 0; i < MAX_BULLETS; ++i) {
    if (!game->bullets[i].alive) {
      b = &game->bullets[i];
      break;
    }
  }
  if (b) {
    Player *p = &game->player;
    b->alive = true;
    b->pos = p->pos;
    b->pos.x += PLAYER_SIZE * p->direction / 2.0;
    b->velocity = BULLET_SPEED * p->direction;
  }
}