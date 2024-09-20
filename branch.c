#include <math.h>
#include <raylib.h>
#include <stdio.h>

float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(320, 240, "Branch");

    SetTargetFPS(60);

    int linecount = 10;
    Vector2 offset = {
        .x = 0,
        .y = 0,
    };
    Vector2 m = GetMousePosition();

    while (!WindowShouldClose()){ 
        BeginDrawing();
        ClearBackground(BLACK);

        Vector2 v = {
            .x = (float)GetScreenWidth() / 2 + offset.x,
            .y = (float)GetScreenHeight() / 2 + offset.y,
        };
        double angle = atan2(v.y - m.y, v.x - m.x);
        double distance = sqrt(pow(v.x - m.x, 2) + pow(v.y - m.y, 2));

        Vector2 real_mouse_position = GetMousePosition();
        m.x = lerp(m.x, real_mouse_position.x, 0.1);
        m.y = lerp(m.y, real_mouse_position.y, 0.1);

        char a_buf[20];
        char d_buf[20];
        char s_buf[20];
        sprintf(a_buf, "Angle: %.5f", angle);
        sprintf(d_buf, "Distance: %.2f", distance);
        sprintf(s_buf, "Segments: %d", linecount);
        DrawText(a_buf, 0, 0, 20, WHITE);
        DrawText(d_buf, 0, 20, 20, WHITE);
        DrawText(s_buf, 0, 40, 20, WHITE);

        Vector2 start = v;
        Vector2 end = v;


        for (int i = 0; i < linecount; i++) {

            Color new_col = ColorFromHSV((float)i * 360 / linecount, 1, 1);

            end.x = end.x - (cos(angle * (i + 1)) * distance);
            end.y = end.y - (sin(angle * (i + 1)) * distance);
            DrawLineEx(start, end, 2, new_col);
            start = end;
        }

        if (IsKeyDown(KEY_Q)) {linecount--;}
        if (IsKeyDown(KEY_E)) {linecount++;}

        if (IsKeyDown(KEY_W)) {offset.y -= 640 * GetFrameTime();}
        if (IsKeyDown(KEY_S)) {offset.y += 640 * GetFrameTime();}

        if (IsKeyDown(KEY_A)) {offset.x -= 640 * GetFrameTime();}
        if (IsKeyDown(KEY_D)) {offset.x += 640 * GetFrameTime();}

        EndDrawing();
    }

    CloseWindow();
}
