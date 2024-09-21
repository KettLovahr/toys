#include <math.h>
#include <raylib.h>

typedef struct {
  float amp;
  float freq;
  float phase;
  float phase_rate;
} SineWave;

float sample_wave(SineWave wave, float location) {
  return sin(location * wave.freq + wave.phase) * wave.amp;
}

void update_wave(SineWave *wave, float rate) {
  wave->phase += rate * wave->phase_rate;
}

void draw_waves(SineWave wave[], int wave_count, float amp, float freq,
               float phase) {
  int half_height = GetScreenHeight() / 2;
  int width = GetScreenWidth();
  for (int i = 0; i < width; i++) {
    float value = 0;
    for (int j = 0; j < wave_count; j++) {
      value += sample_wave(wave[j], (float)i / width);
    }
    int end_position = half_height + value * half_height;
    DrawLine(i, half_height, i, end_position, WHITE);
  }
}

int main() {
  SineWave waves[] = {
      {
          .amp = 0.3,
          .freq = 2 * PI,
          .phase = 0,
          .phase_rate = 0.55,
      },
      {
          .amp = 0.2,
          .freq = 4 * PI,
          .phase = 0,
          .phase_rate = 0.8,
      },
      {
          .amp = 0.05,
          .freq = 7 * PI,
          .phase = 0,
          .phase_rate = 0.5,
      },
      {
          .amp = 0.25,
          .freq = 0.3 * PI,
          .phase = 0,
          .phase_rate = 0.05,
      },
      {
          .amp = 0.10,
          .freq = PI,
          .phase = 0,
          .phase_rate = -0.5,
      },
      {
          .amp = 0.35,
          .freq = 3.3 * PI,
          .phase = 0,
          .phase_rate = -0.95,
      },
  };
  float time = 0;
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(640, 480, "Waves");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    int wave_count = sizeof(waves) / sizeof(waves[0]);
    float delta = GetFrameTime();
    for (int i = 0; i < wave_count; i++) {
      update_wave(&waves[i], delta);
    }
    draw_waves(waves, wave_count, 1.0, 2 * PI, time);
    EndDrawing();
  }

  CloseWindow();
}
