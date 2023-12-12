#include <algorithm>
#include <deque>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

Color blue = {30, 29, 45, 1};
Color lightBlue = {136, 179, 249, 255}; // Alpha value is in the range 0 to 255

double lastUpdateTime = 0;

bool eventTrigger(float interval) {
  double cT = GetTime();
  if (cT - lastUpdateTime >= interval) {
    lastUpdateTime = cT;
    return true;
  }
  return false;
}

const int cellSize = 30;
const int cellCount = 25;

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

class Score {
public:
  int score = 0;
  char scoreS[10];
  Score() {}
  void increment() { score++; };
  void Draw() {
    sprintf(scoreS, "Score:%d", score);
    DrawText(scoreS, 1, 1, 50, lightBlue);
  }
};

class Snake {
public:
  std::deque<Vector2> body = {Vector2{3, 3}, Vector2{2, 3}, Vector2{1, 3}};
  Vector2 dir = {1, 0};
  void Draw() {
    for (unsigned int i = 0; i < body.size(); i++) {
      float x = body[i].x * cellSize;
      float y = body[i].y * cellSize;
      Rectangle rec = Rectangle{x, y, cellSize, cellSize};
      DrawRectangleRounded(rec, 0.5, 4, lightBlue);
    }
  }
  void Update() {
    // Remove Last
    body.pop_back();
    // Add First
    body.push_front(Vector2Add(body.front(), dir));
  }
  void Grow() { body.push_front(Vector2Add(body.front(), dir)); }

  void CheckCollision(Food &f , Score &sc) {
    if (body[0].y == 0 && dir.y == -1) {
      body[0].y = cellCount - 1;
    }
    if (body[0].y == cellCount - 1 && dir.y == 1) {
      body[0].y = 0;
    }
    if (body[0].x == 0 && dir.x == -1) {
      body[0].x = cellCount - 1;
    }
    if (body[0].x == cellCount - 1 && dir.x == 1) {
      body[0].x = 0;
    }

    if (body[0].y == f.pos.y && body[0].x == f.pos.x) {
      sc.increment();
      sc.Draw();
      f.pos = f.randomPos();
      f.Draw();
      Grow();
      Draw();
    }
  }
};



class Game {
public:
  Snake s = Snake();
  Food f = Food();
  Score sc = Score();

  void Draw() {
    f.Draw();
    s.Draw();
    sc.Draw();
  }
  void Update() { s.Update(); }
};

int main(int argc, char *argv[]) {
  InitWindow(cellSize * cellCount, cellSize * cellCount, "Snake");
  SetTargetFPS(120);

  Game game = Game();

  // Food f = Food();
  // Snake s = Snake();
  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(blue);
    // DrawText("Score", 0, 0, 50, lightBlue);
    if (IsKeyPressed(KEY_W) && game.s.dir.y != 1) {
      game.s.dir = {0, -1};
    }
    if (IsKeyPressed(KEY_A) && game.s.dir.x != 1) {
      game.s.dir = {-1, 0};
    }
    if (IsKeyPressed(KEY_S) && game.s.dir.y != -1) {
      game.s.dir = {0, 1};
    }
    if (IsKeyPressed(KEY_D) && game.s.dir.x != -1) {
      game.s.dir = {1, 0};
    }

    if (eventTrigger(0.2)) {

      game.s.CheckCollision(game.f, game.sc);

      float nextX = game.s.body[0].x + game.s.dir.x;
      float nextY = game.s.body[0].y + game.s.dir.y;
      for (int i = 1; i < game.s.body.size(); i++) {

        if (nextX == game.s.body[i].x && nextY == game.s.body[i].y) {
          std::cout << "Collided" << std::endl;
          game.s.body.pop_back();
        }
      }

      // s.Update();
      game.Update();
    }

    game.Draw();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
