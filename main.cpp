#include <iostream>
#include <raylib.h>

Color blue = {30, 29, 45, 1};
Color lightBlue = {136, 179, 249, 255}; // Alpha value is in the range 0 to 255

int cellSize = 30;
int cellCount = 25;

class Food {
public:
  Vector2 pos;
  Texture tex;
  Food() { 
    Image img = LoadImage("assets/nutritionLB.png");
    tex = LoadTextureFromImage(img);
    UnloadImage(img);
    pos = randomPos();
  }

  ~Food() { UnloadTexture(tex); }
  void Draw() {
    // DrawRectangle(pos.x * cellSize, pos.y * cellSize, cellSize, cellSize,
    //               lightBlue);
    DrawTexture(tex, pos.x * cellSize, pos.y * cellSize, lightBlue);
  }
  Vector2 randomPos() {
    float x = GetRandomValue(0, cellCount - 1);
    float y = GetRandomValue(0, cellCount - 1);

    return {x, y};
  }
};

int main(int argc, char *argv[]) {
  InitWindow(cellSize * cellCount, cellSize * cellCount, "Snake");
  SetTargetFPS(60);

  Food f = Food();

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(blue);

    f.Draw();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
