#include <raylib.h>

typedef struct {
    float x;
    float y;
    char dx;
    char dy;
    char cur_col;
} Bouncer;

Color cols[] = {RED, ORANGE, YELLOW, GREEN, BLUE, VIOLET};

void update(Bouncer* b) { 
        b->x += 100 * GetFrameTime() * b->dx;
        b->y += 100 * GetFrameTime() * b->dy;

        if (b->x < 0) {
            b->dx = 1;
            b->x = 0;
            b->cur_col++;
        }
        if (b->y < 0) {
            b->dy = 1;
            b->y = 0;
            b->cur_col++;
        }
        if (b->x > (GetScreenWidth() - 100)) {
            b->dx = -1;
            b->x = GetScreenWidth() - 100;
            b->cur_col++;
        }
        if (b->y > (GetScreenHeight() - 100)) {
            b->dy = -1;
            b->y = GetScreenHeight() - 100;
            b->cur_col++;
        }

        if (b->cur_col >= (sizeof(cols) / sizeof(cols[0]))) {
            b->cur_col = 0;
        }
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(640, 480, "Hello from C.");

    SetTargetFPS(60);


    Bouncer b = {
        .x = 0,
        .y = 0,
        .dx = 1,
        .dy = 1,
    };

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangle(b.x, b.y, 100, 100, cols[b.cur_col]);
        update(&b);
        EndDrawing();
    }

    CloseWindow();
}
