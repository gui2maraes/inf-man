#pragma once

#include "raylib.h"

typedef struct TextureManager {
  Texture2D player;
  Texture2D enemy;
  Texture2D logo;

} TextureManager;

void TextureManager_load(TextureManager *tex_mgr);
void TextureManager_unload(TextureManager *tex_mgr);