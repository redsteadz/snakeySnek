#include <deque>
#include <fstream>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

Color blue = {30, 29, 45, 1};
Color lightBlue = {136, 179, 249, 255}; // Alpha value is in the range 0 to 255

double lastUpdateTime = 0;

bool ElementInDeque(Vector2 element, std::deque<Vector2> deque) {
  for (int i = 0; i < deque.size(); i++) {

    if (Vector2Equals(element, deque[i])) {
      return true;
    }
  }
  return false;
}

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
  Food(std::deque<Vector2> body) {
    Image img = LoadImage("assets/nutritionLB.png");
    tex = LoadTextureFromImage(img);
    UnloadImage(img);
    pos = randomPos(body);
  }

  ~Food() { UnloadTexture(tex); }
  void Draw() {
    // DrawRectangle(pos.x * cellSize, pos.y * cellSize, cellSize, cellSize,
    //               lightBlue);
    DrawTexture(tex, pos.x * cellSize, pos.y * cellSize, lightBlue);
  }
  Vector2 randomPos(std::deque<Vector2> snakeBody) {

    float x = GetRandomValue(0, cellCount - 1);
    float y = GetRandomValue(0, cellCount - 1);
    Vector2 pos = {x, y};
    if (ElementInDeque(pos, snakeBody)) {
      return randomPos(snakeBody);
    }
    return {x, y};
  }
};

class Score {
private:
  int maxscore;

public:
  int score = 0;
  char scoreS[10];
  Score() {
    // Load Maximum Score
    std::ifstream inputFile("maxscore.txt");

    if (!inputFile.is_open()) {
      // If the file doesn't exist, create it
      std::ofstream outputFile("maxscore.txt");

      if (!outputFile.is_open()) {
        std::cerr << "Error creating file: "
                  << "maxscore.txt" << std::endl;
        return;
      }

      // Write the initial value to the file
      outputFile << 0 << std::endl;

      // Close the file
      outputFile.close();

      std::cout << "File created successfully: "
                << "maxscore.txt" << std::endl;

      // Set the initial score to 0
      maxscore = 0;
    } else {
      // Take input from the file
      std::cout << "Read file: maxscore.txt" << std::endl;
      inputFile >> maxscore;
      inputFile.close();
    }
  }

  void UpdateMaxScore(int score) {
    if (score > maxscore) {
      maxscore = score;
      std::ofstream outputFile("maxscore.txt");
      outputFile << maxscore;
      outputFile.close();
    }
  }

  void increment() {
    Draw();
    score++;
    UpdateMaxScore(score);
  };
  void Draw() {
    sprintf(scoreS, "Score:%d", score);
    DrawText(scoreS, 1, 1, 50, lightBlue);
    sprintf(scoreS, "MaxScore:%d", maxscore);
    DrawText(scoreS, 450, 1, 50, lightBlue);
  }
};

class Snake {
public:
  std::deque<Vector2> body = {Vector2{3, 3}, Vector2{2, 3}, Vector2{1, 3}};
  Vector2 dir = {1, 0};
  bool growing = false;

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
    body.push_front(Vector2Add(body.front(), dir));

    if (growing) {
      growing = false;
    } else {

      body.pop_back();
    }
    // Add First
  }

  void CheckCollision(Food &f, Score &sc) {

    // Upper Boundary
    if (body[0].y < 0) {
      body[0].y = cellCount - 1;
    }

    // Bottom Boundary
    if (body[0].y > cellCount - 1) {
      body[0].y = 0;
    }
    if (body[0].x < 0) {
      body[0].x = cellCount - 1;
    }
    if (body[0].x > cellCount - 1) {
      body[0].x = 0;
    }

    if (Vector2Equals(body[0], f.pos)) {
      sc.increment();

      f.pos = f.randomPos(body);
      f.Draw();
      growing = true;
      Draw();
    }
  }
};

class Game {
public:
  Snake s = Snake();
  Food f = Food(s.body);
  Score sc = Score();
  bool paused = true;

  void Draw() {
    if (!paused) {
      f.Draw();
      s.Draw();
      sc.Draw();
    } else {

      DrawText("Start!", cellSize * cellCount / 2 - 95,
               cellSize * cellCount / 2 - 50, 50, lightBlue);
    }
  }

  void Update() { s.Update(); }

  void Restart() {
    s.body = {Vector2{3, 3}, Vector2{2, 3}, Vector2{1, 3}};
    s.dir = {1, 0};
    f.pos = f.randomPos(s.body);
    paused = true;
    sc.score = 0;
    Draw();
  }
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

    if (IsKeyPressed(KEY_SPACE)) {
      game.paused = false;
    }

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

    // eventTrigger
    if (eventTrigger(0.2)) {

      game.s.CheckCollision(game.f, game.sc);

      float nextX = game.s.body[0].x + game.s.dir.x;
      float nextY = game.s.body[0].y + game.s.dir.y;

      for (int i = 1; i < game.s.body.size(); i++) {
        if (nextX == game.s.body[i].x && nextY == game.s.body[i].y) {
          std::cout << "Collided" << std::endl;
          game.Restart();
          // game.s.body.pop_back();
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
