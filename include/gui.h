#pragma once

typedef enum TextAlign { ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT } TextAlign;

/// Draws text in position with scale relative to configuration value and
/// alignment.
void gui_text(char *text, int x, int y, float font_scale, TextAlign align);

/// Draws a number in position with scale relative to configuration value and
/// alignment.
void gui_number(int number, int x, int y, float font_scale, TextAlign align);

/// Draws a button and check if it's been pressed.
/// Returns 1 on pressed and 0 otherwise.
int gui_button(char *label, int x, int y);