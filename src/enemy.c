#include "enemy.h"
#include "animator.h"
#include "config.h"
#include "game.h"
#include "player.h"
#include "raymath.h"
#include <stdio.h>

void Enemy_Animator_init(Animator *animator, TextureManager *tex) {
  animator->atlas = tex->enemy;
  animator->curr_sprite = 0;
  animator->state = 0;
  animator->map_len = 1;
  animator->map[0] = (Animation){{0, 0, 48, 24}, 2, 0.1};
}

void EnemyManager_init(EnemyManager *enemy_mgr, TextureManager *tex) {
  enemy_mgr->enemies_len = 0;
  Enemy_Animator_init(&enemy_mgr->animator, tex);
}

void Enemy_spawn(EnemyManager *enemy_mgr, Vector2 pos) {
  if (enemy_mgr->enemies_len >= MAX_ENEMIES) {
    error_out("Max enemy number reached");
    return;
  }
  Enemy *enemy = &enemy_mgr->enemies[enemy_mgr->enemies_len++];
  enemy->alive = true;
  enemy->dir = DIR_LEFT;
  enemy->pos = pos;
  enemy->velocity = (Vector2){0};
}

Rectangle Enemy_hitbox(Enemy *enemy) {
  return (Rectangle){enemy->pos.x, enemy->pos.y, ENEMY_SIZE, ENEMY_SIZE};
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
  }
}