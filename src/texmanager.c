#include "texmanager.h"
#include "config.h"
#include "raylib.h"

void TextureManager_load(TextureManager *tex_mgr) {
  tex_mgr->player = LoadTexture(SPRITE_PLAYER);
  tex_mgr->enemy = LoadTexture(SPRITE_ENEMY);
  tex_mgr->logo = LoadTexture(SPRITE_LOGO);
}
void TextureManager_unload(TextureManager *tex_mgr) {
  UnloadTexture(tex_mgr->player);
  UnloadTexture(tex_mgr->enemy);
  UnloadTexture(tex_mgr->logo);
}