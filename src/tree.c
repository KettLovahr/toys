#include <math.h>
#include <raylib.h>
#include <stdio.h>

static float time = 0;

typedef enum {
  UINT,
  FASTUINT,
  UCHAR,
  FLOAT,
  FASTFLOAT,
  ANGLE,
} MenuItemTypeHint;

typedef struct {
  char *text;
  char value[10];
  MenuItemTypeHint hint;
  void *data;
} MenuItem;
static int menu_position = 0;

float lerp(float a, float b, float t) { return a + (b - a) * t; }
Color color_lerp(Color a, Color b, float t) {
    Color result = {.r = 0, .g = 0, .b = 0, .a = 0};
    result.r = (unsigned char)lerp(a.r, b.r, t);
    result.g = (unsigned char)lerp(a.g, b.g, t);
    result.b = (unsigned char)lerp(a.b, b.b, t);
    result.a = (unsigned char)lerp(a.a, b.a, t);
    return result;
}

int predicted_line_count(int child_gens, int splits) {
  int result = 0;
  for (int i = 0; i <= child_gens; i++) {
    result += pow(splits, i);
  }
  return result;
}

void create_branch(Vector2 origin, int child_gens, int splits, float direction,
                   float length, float spread, float decay, float sway, Color col, Color target_col) {
  direction += sin(time + (child_gens)) * sway;
  Vector2 end_position = {
      .x = origin.x + cos(direction) * length,
      .y = origin.y + sin(direction) * length,
  };
  DrawLineV(origin, end_position, col);

  if (child_gens > 0) {
    for (int i = 0; i < splits; i++) {
      float new_dir =
          splits == 1
              ? direction
              : direction - spread +
                    (lerp(0, spread * 2, (float)i / (float)(splits - 1)));
      create_branch(end_position, child_gens - 1, splits, new_dir,
                    length * decay, spread, decay, sway, color_lerp(col, target_col, 1.0 / (child_gens + 1)), target_col);
    }
  }
}

void draw_menu(MenuItem menu[], int length) {
  for (int i = 0; i < length; i++) {
    Color col = menu_position == i ? GREEN : WHITE;
    DrawText(menu[i].text, 20, 20 + 20 * i, 20, col);
    DrawText(menu[i].value, 250, 20 + 20 * i, 20, col);

    if (menu_position == i) {
      DrawText("<", 5, 20 + 20 * i, 20, col);
      DrawText(">", 300, 20 + 20 * i, 20, col);
    }
  }
}

void execute_menu_behavior(MenuItem menu[], int length) {
  int mouse_v = 0;
  Vector2 m = GetMousePosition();
  for (int i = 0; i < length; i++) {
    if (m.x > 0 && m.x < 330 && m.y > 20 + 20 * i && m.y < 20 + 20 * i + 20) {
      if (m.x > 0 && m.x < 100) {
        mouse_v = -1;
      }
      if (m.x > 230 && m.x < 330) {
        mouse_v = +1;
      }
      menu_position = i;
    }
  }
  if (IsKeyPressed(KEY_UP)) {
    menu_position = menu_position == 0 ? (length - 1) : (menu_position - 1);
  }
  if (IsKeyPressed(KEY_DOWN)) {
    menu_position = (menu_position + 1) % length;
  }

  switch (menu[menu_position].hint) {
  case UINT:
    if (IsKeyPressed(KEY_LEFT) ||
        (mouse_v == -1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) {
      if (*(int *)menu[menu_position].data > 0) {
        *(int *)menu[menu_position].data -= 1;
      }
    }
    if (IsKeyPressed(KEY_RIGHT) ||
        (mouse_v == 1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) {
      *(int *)menu[menu_position].data += 1;
    }
    break;
  case FASTUINT:
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT) ||
        (mouse_v == -1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))) {
      if (*(int *)menu[menu_position].data > 0) {
        *(int *)menu[menu_position].data -= 1;
      }
    }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT) ||
        (mouse_v == 1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))) {
      *(int *)menu[menu_position].data += 1;
    }
    break;
  case UCHAR:
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT) ||
        (mouse_v == -1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))) {
      if (*(unsigned char *)menu[menu_position].data > 0) {
        *(unsigned char *)menu[menu_position].data -= 1;
      }
    }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT) ||
        (mouse_v == 1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))) {
      if (*(unsigned char *)menu[menu_position].data < 255) {
        *(unsigned char *)menu[menu_position].data += 1;
      }
    }
    break;
  case FLOAT:
    if (IsKeyPressed(KEY_LEFT) ||
        (mouse_v == -1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) {
      *(float *)menu[menu_position].data -= 0.01;
    }
    if (IsKeyPressed(KEY_RIGHT) ||
        (mouse_v == 1 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))) {
      *(float *)menu[menu_position].data += 0.01;
    }
    break;
  case FASTFLOAT:
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT) ||
        (mouse_v == -1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))) {
      *(float *)menu[menu_position].data -= 0.01;
    }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT) ||
        (mouse_v == 1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))) {
      *(float *)menu[menu_position].data += 0.01;
    }
    break;
  case ANGLE:
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT) ||
        (mouse_v == -1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))) {
      *(float *)menu[menu_position].data -= 1 * DEG2RAD;
    }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT) ||
        (mouse_v == 1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT))) {
      *(float *)menu[menu_position].data += 1 * DEG2RAD;
    }
    break;
  }

  for (int i = 0; i < length; i++) {
    switch (menu[i].hint) {
    case UINT:
    case FASTUINT:
      sprintf(menu[i].value, "%d", *(int *)menu[i].data);
      break;
    case UCHAR:
      sprintf(menu[i].value, "%d", *(unsigned char *)menu[i].data);
      break;
    case FLOAT:
    case FASTFLOAT:
      sprintf(menu[i].value, "%.2f", *(float *)menu[i].data);
      break;
    case ANGLE:
      sprintf(menu[i].value, "%.0f°", *(float *)menu[i].data * RAD2DEG);
      break;
    }
  }
}

int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
  InitWindow(640, 480, "Kettree");

  int child_gens = 6;
  int splits = 4;
  float direction = -PI / 2;
  int length = 240;
  float spread = PI / 3;
  float decay = 0.5;
  float sway = 0.05;
  Color start_color = {255, 255, 255, 255};
  Color end_color = {127, 127, 127, 255};

  MenuItem menu[] = {
      {.text = "Child Generations", .hint = UINT, .data = &child_gens},
      {.text = "Splits", .hint = UINT, .data = &splits},
      {.text = "Length", .hint = FASTUINT, .data = &length},
      {.text = "Decay", .hint = FASTFLOAT, .data = &decay},
      {.text = "Direction", .hint = ANGLE, .data = &direction},
      {.text = "Spread", .hint = ANGLE, .data = &spread},
      {.text = "Sway", .hint = FLOAT, .data = &sway},
      {.text = "Start Color R", .hint = UCHAR, .data = &start_color.r},
      {.text = "Start Color G", .hint = UCHAR, .data = &start_color.g},
      {.text = "Start Color B", .hint = UCHAR, .data = &start_color.b},
      {.text = "End Color R", .hint = UCHAR, .data = &end_color.r},
      {.text = "End Color G", .hint = UCHAR, .data = &end_color.g},
      {.text = "End Color B", .hint = UCHAR, .data = &end_color.b},
  };
  int menu_length = sizeof(menu) / sizeof(menu[0]);

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    time += GetFrameTime();
    ClearBackground(BLACK);
    Vector2 origin = {
        .x = GetScreenWidth() / 2.0,
        .y = GetScreenHeight(),
    };
    create_branch(origin, child_gens, splits, direction, length, spread, decay,
                  sway, start_color, end_color);

    execute_menu_behavior(menu, menu_length);
    draw_menu(menu, menu_length);

    char e[25];
    sprintf(e, "Line Count: %d", predicted_line_count(child_gens, splits));
    DrawText(e, 20, GetScreenHeight() - 40, 20, WHITE);

    EndDrawing();
  }

  CloseWindow();
}
