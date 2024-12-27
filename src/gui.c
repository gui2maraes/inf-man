#include "gui.h"
#include "config.h"
#include "raylib.h"

void gui_text(char *text, int x, int y, float font_scale, TextAlign align) {
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

  DrawText(text, x - offset, y, GUI_FONT_SIZE * font_scale, WHITE);
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