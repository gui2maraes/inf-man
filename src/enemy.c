#include "enemy.h"
#include "animator.h"
#include "collision.h"
#include "config.h"
#include "game.h"
#include "player.h"
#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>

static void Enemy_Animator_init(Animator *animator, TextureManager *tex) {
  Animator_init(animator, tex->enemy);
  Animator_add(animator, 0, Animation_new((Rectangle){0, 0, 48, 24}, 2, 0.1));
  Animator_change(animator, 0);
}

void EnemyManager_init(EnemyManager *enemy_mgr, TextureManager *tex) {
  enemy_mgr->enemies_len = 0;
  Enemy_Animator_init(&enemy_mgr->animator, tex);
}

void EnemyManager_add(EnemyManager *enemy_mgr, Vector2 pos) {
  if (enemy_mgr->enemies_len >= MAX_ENEMIES) {
    error_out("Max enemy number reached");
    return;
  }
  enemy_mgr->spawn_positions[enemy_mgr->enemies_len++] = pos;
}
void EnemyManager_spawn(EnemyManager *enemy_mgr) {
  for (int i = 0; i < enemy_mgr->enemies_len; ++i) {
    Enemy *enemy = &enemy_mgr->enemies[i];
    enemy->alive = true;
    enemy->dir = DIR_LEFT;
    enemy->pos = enemy_mgr->spawn_positions[i];
    enemy->velocity = (Vector2){0};
  }
}

static Hitbox Enemy_hitbox(Enemy *enemy) {
  return (Hitbox){enemy->pos, ENEMY_SIZE / 2.0};
}

static void Enemy_update(Enemy *enemy, Player *player, float delta) {
  if (!enemy->alive) {
    return;
  }
  Vector2 dist = Vector2Subtract(player->pos, enemy->pos);
  if (fabsf(dist.x) > (float)WINDOW_WIDTH) {
    return;
  }
  enemy->velocity = Vector2Normalize(dist);
  // enemy->velocity.x =
  //     Clamp(enemy->velocity.x, -ENEMY_MAX_SPEED, ENEMY_MAX_SPEED);
  enemy->pos.x += enemy->velocity.x * delta * ENEMY_MAX_SPEED;
  enemy->pos.y += enemy->velocity.y * delta * ENEMY_MAX_SPEED;
}

void EnemyManager_update(Game *game, float delta) {
  EnemyManager *enemy_mgr = &game->enemy_mgr;
  for (int i = 0; i < enemy_mgr->enemies_len; ++i) {
    Enemy_update(&enemy_mgr->enemies[i], &game->player, delta);
  }
  Animator_update(&enemy_mgr->animator, delta);
}

void EnemyManager_draw(EnemyManager *enemy_mgr) {
  for (int i = 0; i < enemy_mgr->enemies_len; ++i) {
    Enemy *enemy = &enemy_mgr->enemies[i];
    if (!enemy->alive) {
      continue;
    }
    Animator_draw(&enemy_mgr->animator, enemy->pos, enemy->dir);
    if (DEBUG_MODE) {
      Hitbox hb = Enemy_hitbox(enemy);
      DrawCircleV(hb.pos, hb.radius, RED);
    }
  }
}

Enemy *EnemyManager_colliding_with(EnemyManager *enemy_mgr, Hitbox hitbox) {
  for (int i = 0; i < enemy_mgr->enemies_len; ++i) {
    Enemy *enemy = &enemy_mgr->enemies[i];
    if (!enemy->alive) {
      continue;
    }
    Hitbox hb = Enemy_hitbox(enemy);
    if (CheckCollisionCircles(hb.pos, hb.radius, hitbox.pos, hitbox.radius)) {
      return enemy;
    }
  }
  return 0;
}