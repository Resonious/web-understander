#include "raylib.h"

Font global_font;










bool DrawButton(Rectangle rect, const char *text, Color bgcolor) {
  DrawRectangleRounded(rect, 0.5f, 8, bgcolor);
  Vector2 textPos;
  textPos.x = rect.x + 10;
  textPos.y = rect.y + 10;
  DrawTextEx(global_font, text, textPos, 14, 1.0, BLACK);
  return false;
}
