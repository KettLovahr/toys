#include <math.h>
#include <raylib.h>
#include <stdio.h>

static float time = 0;

typedef enum {
  INT,
  FASTINT,
  FLOAT,
  FASTFLOAT,
} MenuItemTypeHint;

typedef struct {
  char *text;
  char value[10];
  MenuItemTypeHint hint;
  void *data;
} MenuItem;
static int menu_position = 0;

float lerp(float a, float b, float t) { return a + (b - a) * t; }

void create_branch(Vector2 origin, int child_gens, int splits, float direction,
                   float length, float spread, float length_decay, float sway) {
  direction += sin(time + (child_gens)) * sway;
  Vector2 end_position = {
      .x = origin.x + cos(direction) * length,
      .y = origin.y + sin(direction) * length,
  };
  DrawLineV(origin, end_position, WHITE);

  if (child_gens > 0) {
    for (int i = 0; i < splits; i++) {
      float new_dir = direction - spread +
                      (lerp(0, spread * 2, (float)i / (float)(splits - 1)));
      create_branch(end_position, child_gens - 1, splits, new_dir,
                    length * length_decay, spread, length_decay, sway);
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
  if (IsKeyPressed(KEY_UP)) {
    menu_position = menu_position == 0 ? (length - 1) : (menu_position - 1);
  }
  if (IsKeyPressed(KEY_DOWN)) {
    menu_position = (menu_position + 1) % length;
  }

  switch (menu[menu_position].hint) {
  case INT:
    if (IsKeyPressed(KEY_LEFT)) {
      *(int *)menu[menu_position].data -= 1;
    }
    if (IsKeyPressed(KEY_RIGHT)) {
      *(int *)menu[menu_position].data += 1;
    }
    break;
  case FASTINT:
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT)) {
      *(int *)menu[menu_position].data -= 1;
    }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT)) {
      *(int *)menu[menu_position].data += 1;
    }
    break;
  case FLOAT:
    if (IsKeyPressed(KEY_LEFT)) {
      *(float *)menu[menu_position].data -= 0.01;
    }
    if (IsKeyPressed(KEY_RIGHT)) {
      *(float *)menu[menu_position].data += 0.01;
    }
    break;
  case FASTFLOAT:
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT)) {
      *(float *)menu[menu_position].data -= 0.01;
    }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT)) {
      *(float *)menu[menu_position].data += 0.01;
    }
    break;
  }

  for (int i = 0; i < length; i++) {
    switch (menu[i].hint) {
    case INT:
    case FASTINT:
      sprintf(menu[i].value, "%d", *(int *)menu[i].data);
      break;
    case FLOAT:
    case FASTFLOAT:
      sprintf(menu[i].value, "%.2f", *(float *)menu[i].data);
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

  MenuItem menu[] = {
      {
          .text = "Child Generations",
          .hint = INT,
          .data = &child_gens,
      },
      {
          .text = "Splits",
          .hint = INT,
          .data = &splits,
      },
      {
          .text = "Direction",
          .hint = FASTFLOAT,
          .data = &direction,
      },
      {
          .text = "Length",
          .hint = FASTINT,
          .data = &length,
      },
      {
          .text = "Spread",
          .hint = FASTFLOAT,
          .data = &spread,
      },
      {
          .text = "Decay",
          .hint = FASTFLOAT,
          .data = &decay,
      },
      {
          .text = "Sway",
          .hint = FLOAT,
          .data = &sway,
      },
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
                  sway);

    execute_menu_behavior(menu, menu_length);
    draw_menu(menu, menu_length);
    EndDrawing();
  }

  CloseWindow();
}
