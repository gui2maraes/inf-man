#include "bullet.h"
#include "collision.h"
#include "config.h"
#include "enemy.h"
#include "game.h"
#include "player.h"

Hitbox Bullet_hitbox(Bullet *bullet) {
  return (Hitbox){bullet->pos, BULLET_HEIGHT};
}
bool Bullet_alive(Bullet *bullet) { return bullet->alive_time > 0.0; }

void Bullet_update(Bullet *bullet, Player *p, EnemyManager *enemy_mgr,
                   float delta) {
  if (Bullet_alive(bullet)) {
    bullet->alive_time -= delta;
  } else {
    return;
  }
  bullet->pos.x += bullet->velocity * delta;
  Enemy *enemy = EnemyManager_colliding_with(enemy_mgr, Bullet_hitbox(bullet));
  if (enemy) {
    enemy->alive = false;
    bullet->alive_time = 0;
    p->record.score += ENEMY_PTS;
  }
}
void Bullets_update(Game *game, float delta) {
  for (int i = 0; i < MAX_BULLETS; ++i) {
    Bullet_update(&game->bullets[i], &game->player, &game->enemy_mgr, delta);
  }
}

void Bullet_draw(Bullet *bullet) {
  if (Bullet_alive(bullet)) {
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
    if (!Bullet_alive(&game->bullets[i])) {
      b = &game->bullets[i];
      break;
    }
  }
  if (b) {
    Player *p = &game->player;
    b->alive_time = BULLET_ALIVE_TIME;
    b->pos = p->pos;
    b->pos.x += PLAYER_SIZE * p->direction / 2.0;
    b->velocity = BULLET_SPEED * p->direction;
  }
}