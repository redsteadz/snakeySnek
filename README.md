# Snake Game

A simple Snake game implemented in C++ using the Raylib library.


## Description

This Snake game is a classic implementation where the player controls a snake to eat food and grow longer. The game ends if the snake collides with the walls or itself.

## Features

- Snake movement controlled by arrow keys (UP, DOWN, LEFT, RIGHT).
- Random generation of food for the snake to eat.
- Score tracking to keep track of the player's performance.
- Game over screen when the snake collides with the walls or itself.

## How to Play

1. Use the arrow keys (UP, DOWN, LEFT, RIGHT) to control the snake's movement.
2. The snake will continuously move in the selected direction.
3. Eat the food to grow longer and increase your score.
4. Avoid colliding with the walls and the snake's own body.
5. The game ends when the snake collides, and your score is displayed.

## Getting Started

### Prerequisites

- C++ compiler
- Raylib library installed (Follow instructions on [Raylib GitHub](https://github.com/raysan5/raylib) for installation)

### Building and Running

1. Clone the repository:

    ```bash
    git clone https://github.com/your-username/snake-game.git
    ```

2. Build the game:

    ```bash
    cd snake-game
    g++ -o snake_game main.cpp -lraylib
    ```

3. Run the game:

    ```bash
    ./snake_game
    ```

## Future Additions

- Add levels with increasing difficulty.
- Implement a high score system.
- Add sound effects and background music.
- Support for additional input methods.

Feel free to contribute to the project and suggest new features!

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
