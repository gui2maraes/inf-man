#include "gui.h"
#include "config.h"
#include "player.h"
#include "raylib.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

Vector2 gui_text(char *text, int x, int y, float font_scale, TextAlign align) {
  int text_size = MeasureText(text, GUI_FONT_SIZE * font_scale);
  int offset;
  switch (align) {
  case ALIGN_LEFT:
    offset = 0;
    break;
  case ALIGN_CENTER:
    offset = text_size / 2.0;
    break;
  case ALIGN_RIGHT:
    offset = text_size;
    break;
  }

  DrawText(text, x - offset + 3, y + 3, GUI_FONT_SIZE * font_scale, DARKGRAY);
  DrawText(text, x - offset, y, GUI_FONT_SIZE * font_scale, WHITE);
  return (Vector2){x - offset + text_size, y + GUI_FONT_SIZE * font_scale};
}

Vector2 gui_number(int number, int x, int y, float font_scale,
                   TextAlign align) {
  char txt[20] = {0};
  snprintf(txt, 20, "%d", number);
  return gui_text(txt, x, y, font_scale, align);
}

int gui_button(char *label, int x, int y) {
  int text_size = MeasureText(label, GUI_FONT_SIZE);
  Rectangle rect = {x - text_size / 2.0, y, text_size, GUI_FONT_SIZE};
  Vector2 mouse_pos = GetMousePosition();
  Color col = WHITE;
  int out = 0;
  if (CheckCollisionPointRec(mouse_pos, rect)) {
    col = LIGHTGRAY;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      out = 1;
    }
  }
  DrawText(label, rect.x, rect.y, GUI_FONT_SIZE, col);
  return out;
}

Vector2 gui_stats(Player *player, int x, int y) {
  Vector2 next = gui_text("Lives: ", x, y, 1, ALIGN_LEFT);
  gui_number(player->health, next.x, y, 1, ALIGN_LEFT);
  Vector2 next2 = gui_text("Score: ", x, next.y, 1, ALIGN_LEFT);
  next2 = gui_number(player->record.score, next2.x, next.y, 1, ALIGN_LEFT);
  return next2;
}

Vector2 gui_textbox(char *buffer, int x, int y, TextAlign align) {
  char s[PLAYER_NAME_BUF] = {0};
  memset(s, '_', PLAYER_NAME_MAX);
  for (int i = 0; i < PLAYER_NAME_BUF && buffer[i] != 0; ++i) {
    s[i] = buffer[i];
  }

  s[PLAYER_NAME_MAX] = '\0';
  Rectangle area = {x, y, MeasureText(s, GUI_FONT_SIZE), GUI_FONT_SIZE};
  Vector2 next = gui_text(s, x, y, 1.0, align);

  int c = GetCharPressed();
  int len = strlen(buffer);
  if (IsKeyPressed(KEY_BACKSPACE) && len > 0) {
    buffer[--len] = 0;

  } else if ((isalnum(c) || c == '_' || c == '-') && len < PLAYER_NAME_MAX) {
    buffer[len++] = c;
    buffer[len] = 0;
  }
  return next;
}