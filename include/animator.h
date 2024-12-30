#pragma once
#include "raylib.h"
#include "utils.h"
#include <stdbool.h>

#define ANIMATION_MAP_SIZE 5

/// Represents an animation.
typedef struct Animation {
  /// Atlas region containing this animation.
  Rectangle sub_atlas;
  /// Number of sprites in the atlas.
  int num_sprites;
  /// How much time each sprite gets in the animation.
  float secs_per_sprite;
} Animation;

typedef struct Animator {
  /// Array containing all the animations
  Animation map[ANIMATION_MAP_SIZE];
  /// Size of the animation map
  int map_len;
  /// Atlas containing all the animations
  Texture atlas;
  /// Index of the active animation.
  int state;
  /// Current sprite of the active animation.
  int curr_sprite;
  /// Keeps count of elapsed time.
  float timer;
} Animator;

Rectangle Animator_current_sprite(Animator *animator);
/// Updates the sprite of the animation currently playing.
void Animator_update(Animator *animator, float delta);
/// Draws the current sprite of the animation currently playing.
void Animator_draw(Animator *animator, Vector2 pos, Direction direction);
/// Changes the animator active animation.
void Animator_change(Animator *animator, int new_state);