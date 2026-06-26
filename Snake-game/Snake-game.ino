#include <LedControl.h>

LedControl lc(12, 11, 10, 1);

// Joystick pins
const int joyX = A0;
const int joyY = A1;

// Snake body (max 64 cells)
int snakeX[64];
int snakeY[64];
int length = 3;

// Direction
int dx = 1;
int dy = 0;

// Food
int foodX;
int foodY;

bool gameOver = false;

unsigned long lastMove = 0;
const int speed = 250;

void spawnFood() {
  bool ok;

  do {
    ok = true;
    foodX = random(0, 8);
    foodY = random(0, 8);

    for (int i = 0; i < length; i++) {
      if (snakeX[i] == foodX && snakeY[i] == foodY) {
        ok = false;
      }
    }
  } while (!ok);
}

void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  randomSeed(analogRead(A2));

  // Initial snake
  snakeX[0] = 3;
  snakeY[0] = 3;

  snakeX[1] = 2;
  snakeY[1] = 3;

  snakeX[2] = 1;
  snakeY[2] = 3;

  spawnFood();
}

void loop() {

  if (gameOver) {
    lc.clearDisplay(0);

    // simple X pattern
    for (int i = 0; i < 8; i++) {
      lc.setLed(0, i, i, true);
      lc.setLed(0, i, 7 - i, true);
    }

    while (1);
  }

  int x = analogRead(joyX);
  int y = analogRead(joyY);

  // X controls
  if (x < 300 && dx != 1) {
    dx = -1;
    dy = 0;
  }
  else if (x > 700 && dx != -1) {
    dx = 1;
    dy = 0;
  }

  // Y controls (INVERTED)
  if (y > 700 && dy != -1) {
    dx = 0;
    dy = 1;
  }
  else if (y < 300 && dy != 1) {
    dx = 0;
    dy = -1;
  }

  // Movement timing
  if (millis() - lastMove > speed) {
    lastMove = millis();

    // Move body
    for (int i = length - 1; i > 0; i--) {
      snakeX[i] = snakeX[i - 1];
      snakeY[i] = snakeY[i - 1];
    }

    // Move head
    snakeX[0] += dx;
    snakeY[0] += dy;

    // Wall collision
    if (snakeX[0] < 0 || snakeX[0] > 7 ||
        snakeY[0] < 0 || snakeY[0] > 7) {
      gameOver = true;
    }

    // Self collision
    for (int i = 1; i < length; i++) {
      if (snakeX[0] == snakeX[i] &&
          snakeY[0] == snakeY[i]) {
        gameOver = true;
      }
    }

    // Eat food
    if (snakeX[0] == foodX &&
        snakeY[0] == foodY) {

      snakeX[length] = snakeX[length - 1];
      snakeY[length] = snakeY[length - 1];

      length++;

      if (length < 64) {
        spawnFood();
      }
    }

    // Draw
    lc.clearDisplay(0);

    // food
    lc.setLed(0, foodY, foodX, true);

    // snake
    for (int i = 0; i < length; i++) {
      lc.setLed(0, snakeY[i], snakeX[i], true);
    }
  }
}