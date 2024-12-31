#include "bullet.h"
#include "collision.h"
#include "config.h"
#include "enemy.h"
#include "game.h"
#include "player.h"
#include <math.h>

Hitbox Bullet_hitbox(Bullet *bullet) {
  return (Hitbox){bullet->pos, BULLET_HEIGHT};
}

void Bullet_update(Bullet *bullet, Player *p, EnemyManager *enemy_mgr,
                   float cutoff_distance, float delta) {
  if (fabs(bullet->pos.x - p->pos.x) > cutoff_distance) {
    bullet->alive = false;
  }
  if (!bullet->alive) {
    return;
  }
  bullet->pos.x += bullet->velocity * delta;
  Enemy *enemy = EnemyManager_colliding_with(enemy_mgr, Bullet_hitbox(bullet));
  if (enemy) {
    enemy->alive = false;
    bullet->alive = false;
    p->record.score += ENEMY_PTS;
  }
}
void Bullets_update(Game *game, float delta) {
  float cutoff = WINDOW_WIDTH / game->camera.zoom / 2.0;
  for (int i = 0; i < MAX_BULLETS; ++i) {
    Bullet_update(&game->bullets[i], &game->player, &game->enemy_mgr, cutoff,
                  delta);
  }
}

void Bullet_draw(Bullet *bullet) {
  // Rectangle rect = Bullet_hitbox(bullet);
  // rect.x -= rect.width / 2;
  // rect.y -= rect.height / 2;
  if (bullet->alive) {
    // DrawRectangleRec(rect, YELLOW);
    Hitbox hb = Bullet_hitbox(bullet);
    DrawCircleV(hb.pos, hb.radius, YELLOW);
    // if (DEBUG_MODE) {
    //   DrawRectangleRec(Bullet_hitbox(bullet), ColorAlpha(RED, 0.5));
    // }
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