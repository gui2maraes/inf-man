#include "enemy.h"
#include "animator.h"
#include "collision.h"
#include "config.h"
#include "game.h"
#include "player.h"
#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stdbool.h>
#include <utils.h>

void Enemy_init(Enemy *enemy, Vector2 pos) {
  enemy->alive = true;
  enemy->dir = DIR_LEFT;
  enemy->last_pos = pos;
  enemy->pos = pos;
  enemy->attack = ATTACK_NULL;
  enemy->target_pos = (Vector2){0};
}

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
    Enemy_init(enemy, enemy_mgr->spawn_positions[i]);
    // enemy->alive = true;
    // // create random wobble from 0 to 10
    // enemy->dir = DIR_LEFT;
    // enemy->pos = enemy_mgr->spawn_positions[i];
    // enemy->velocity = (Vector2){0};
  }
}

static Hitbox Enemy_hitbox(Enemy *enemy) {
  return (Hitbox){enemy->pos, ENEMY_SIZE / 2.0};
}
// static void Enemy_create_attack(Enemy *enemy, Player *player) {
//   Vector2 vertex = Vector2Subtract(player->pos, enemy->pos);
//   // quadratic function in vertex form:
//   // f(x) = a(x - h)^2 + k, where vertex: (h, k) and `a` is constant
//   // 0 = a(-h)^2 + k
//   // 0 = ah^2 + k
//   // -k = ah^2
//   // -k / h^2 = a
//   float h = vertex.x;
//   float k = vertex.y;
//   float a = -k / (h * h);
//   // f(x) = a(x - h)^2 + k
//   // f(x) = a(x^2 - 2xh + h^2) + k
//   // f(x) = ax^2 - 2ahx + h^2 + k
//   // a = a, b = -2ah, c = h^2 + k
//   enemy->in_attack = true;
//   enemy->attack_a = a;
//   enemy->attack_b = -2 * a * h;
//   enemy->attack_c = h * h + k;
//   enemy->attack_t = 0;
// }

static void Enemy_update(Enemy *enemy, Player *player, float delta) {
  if (!enemy->alive) {
    return;
  }
  Vector2 dist = Vector2Subtract(player->pos, enemy->pos);
  if (fabsf(dist.x) > ENEMY_UPDATE_DIST) {
    return;
  }
  if (dist.x > 0.0) {
    enemy->dir = DIR_RIGHT;
  } else {
    enemy->dir = DIR_LEFT;
  }
  // initiate attack if in range
  if (enemy->attack == ATTACK_NULL && fabsf(dist.x) < ENEMY_ATTACK_DIST) {
    enemy->last_pos = enemy->pos;
    enemy->attack = ATTACK_ENTER;
    enemy->target_pos = player->pos;
  }

  // if the target is not reached yet
  Vector2 vel =
      Vector2Normalize(Vector2Subtract(enemy->target_pos, enemy->last_pos));
  switch (enemy->attack) {
  case ATTACK_ENTER:
    // if the enemy reached the target position, leave the attack
    if ((vel.x < 0 && enemy->pos.x <= enemy->target_pos.x) ||
        (vel.x > 0 && enemy->pos.x >= enemy->target_pos.x)) {
      enemy->attack = ATTACK_LEAVE;
    } else {
      enemy->pos.x += vel.x * ENEMY_ATTACK_SPEED * delta;
      enemy->pos.y += vel.y * ENEMY_ATTACK_SPEED * delta;
    }
    break;

  case ATTACK_LEAVE:
    // invert the vertical direction
    vel.y *= -1;
    // if the enemy reached the same height as before the attack, finish it
    if ((vel.y < 0 && enemy->pos.y <= enemy->last_pos.y) ||
        (vel.y > 0 && enemy->pos.y >= enemy->last_pos.y)) {
      enemy->attack = ATTACK_FINISH;
    } else {
      enemy->pos.x += vel.x * ENEMY_ATTACK_SPEED * delta;
      enemy->pos.y += vel.y * ENEMY_ATTACK_SPEED * delta;
    }
    break;
  case ATTACK_FINISH:
    // walk just far enough to not immediately start the attack again
    if (fabsf(dist.x) > ENEMY_ATTACK_DIST + TILE_SIZE * 2) {
      enemy->attack = ATTACK_NULL;
    } else {
      enemy->pos.x += fsign(vel.x) * ENEMY_SPEED * delta;
    }
    break;
  default:
    // default movement
    enemy->pos.x += enemy->dir * ENEMY_SPEED * delta;
  }
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