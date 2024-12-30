#include "animator.h"
#include "raylib.h"
#include "utils.h"

void Animator_update(Animator *animator, float delta) {
  animator->timer += delta;
  Animation *anim = &animator->map[animator->state];
  while (animator->timer > anim->secs_per_sprite) {
    animator->curr_sprite = (animator->curr_sprite + 1) % anim->num_sprites;
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
  // DrawTexture(animator->atlas, pos.x, pos.y, WHITE);
  if (direction == DIR_RIGHT) {
    sprite_rect.width *= -1;
  }
  DrawTextureRec(animator->atlas, sprite_rect, pos, WHITE);
}
void Animator_change(Animator *animator, int new_state) {
  if (animator->state == new_state) {
    return;
  }
  if (new_state >= animator->map_len) {
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