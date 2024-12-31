#include "animator.h"
#include "raylib.h"
#include "utils.h"

Animation Animation_new(Rectangle sub_atlas, int num_sprites,
                        float secs_per_sprite) {
  /// -1 indicates no next state
  return (Animation){sub_atlas, num_sprites, secs_per_sprite};
}

void Animator_init(Animator *animator, Texture atlas) {
  animator->atlas = atlas;
  animator->state = 0;
  animator->curr_sprite = 0;
  animator->timer = 0;
  for (int i = 0; i < ANIMATION_MAP_SIZE; ++i) {
    animator->map[i] = (Animation){0};
  }
}
void Animator_add(Animator *animator, int state, Animation animation) {
  if (state >= ANIMATION_MAP_SIZE) {
    error_out("invalid animation state index");
    return;
  }
  animator->map[state] = animation;
}

void Animator_update(Animator *animator, float delta) {
  animator->timer += delta;
  Animation *anim = &animator->map[animator->state];
  while (animator->timer > anim->secs_per_sprite) {
    int next_sprite = animator->curr_sprite + 1;
    animator->curr_sprite = next_sprite % anim->num_sprites;
    animator->timer -= anim->secs_per_sprite;
  }
}
static Rectangle Animation_sprite(Animation *animation, int sprite_n) {
  sprite_n %= animation->num_sprites;
  int sprite_width = animation->sub_atlas.width / animation->num_sprites;
  Rectangle sub_atlas = animation->sub_atlas;
  Rectangle region = {.x = sub_atlas.x + sprite_n * sprite_width,
                      .y = animation->sub_atlas.y,
                      .width = sprite_width,
                      .height = sub_atlas.height};
  return region;
}
void Animator_draw(Animator *animator, Vector2 pos, Direction direction) {
  Animation *anim = &animator->map[animator->state];
  Rectangle sprite_rect = Animation_sprite(anim, animator->curr_sprite);
  pos.x -= sprite_rect.width / 2.0;
  pos.y -= sprite_rect.height / 2.0;
  if (direction == DIR_RIGHT) {
    sprite_rect.width *= -1;
  }
  DrawTextureRec(animator->atlas, sprite_rect, pos, WHITE);
}
void Animator_change(Animator *animator, int new_state) {
  if (animator->state == new_state) {
    return;
  }
  if (new_state >= ANIMATION_MAP_SIZE) {
    error_out("invalid animation state.");
    return;
  }
  animator->state = new_state;
  animator->curr_sprite = 0;
  animator->timer = 0;
}

Rectangle Animator_current_sprite(Animator *animator) {
  return Animation_sprite(&animator->map[animator->state],
                          animator->curr_sprite);
}