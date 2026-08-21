#include <LedControl.h>

// MAX7219
// DIN = 9, CLK = 11, CS = 10
LedControl lc(9, 11, 10, 1);

// ----------------------
// BOTONES
// ----------------------
const int up = 5;
const int down = 4;

// ----------------------
// PERSONAJE
// ----------------------
int playerX = 1;
int playerY = 3;

// ----------------------
// OBSTÁCULOS
// ----------------------

// Tenemos 2 obstáculos
int obstacleX[2];

// Posición inicial del hueco
int gapY[2];

// Tamaño del hueco
const int gapSize = 3;

// ----------------------
// VELOCIDAD
// ----------------------
unsigned long lastMove = 0;
const int speed = 400;

// ----------------------
// PUNTUACIÓN
// ----------------------
int score = 0;

// ----------------------
// GAME OVER
// ----------------------
bool gameOver = false;

// ----------------------
// ESTADO DE BOTONES
// ----------------------
int oldUp = LOW;
int oldDown = LOW;


// ======================================================
// CREAR OBSTÁCULO
// ======================================================

void createObstacle(int i, int x) {

  obstacleX[i] = x;

  // El hueco puede estar entre 0 y 5
  gapY[i] = random(0, 6);
}


// ======================================================
// REINICIAR JUEGO
// ======================================================

void restartGame() {

  playerX = 1;
  playerY = 3;

  score = 0;
  gameOver = false;

  createObstacle(0, 7);
  createObstacle(1, 11);

  lc.clearDisplay(0);
}


// ======================================================
// DIBUJAR
// ======================================================

void drawGame() {

  lc.clearDisplay(0);

  // ----------------------
  // Dibujar personaje
  // ----------------------

  lc.setLed(0, playerY, playerX, true);


  // ----------------------
  // Dibujar obstáculos
  // ----------------------

  for (int i = 0; i < 2; i++) {

    // Si está dentro de la pantalla
    if (obstacleX[i] >= 0 && obstacleX[i] <= 7) {

      for (int y = 0; y < 8; y++) {

        // No dibujar el hueco
        if (y < gapY[i] || y >= gapY[i] + gapSize) {

          lc.setLed(0, y, obstacleX[i], true);
        }
      }
    }
  }
}


// ======================================================
// COMPROBAR COLISIÓN
// ======================================================

void checkCollision() {

  // ----------------------
  // Paredes superior/inferior
  // ----------------------

  if (playerY < 0 || playerY > 7) {
    gameOver = true;
    return;
  }


  // ----------------------
  // Obstáculos
  // ----------------------

  for (int i = 0; i < 2; i++) {

    if (obstacleX[i] == playerX) {

      // Si NO está dentro del hueco
      if (playerY < gapY[i] ||
          playerY >= gapY[i] + gapSize) {

        gameOver = true;
      }
    }
  }
}


// ======================================================
// PANTALLA GAME OVER
// ======================================================

void showGameOver() {

  lc.clearDisplay(0);

  // X
  for (int i = 0; i < 8; i++) {

    lc.setLed(0, i, i, true);
    lc.setLed(0, i, 7 - i, true);
  }
}


// ======================================================
// SETUP
// ======================================================

void setup() {

  // MAX7219
  lc.shutdown(0, false);

  lc.setIntensity(0, 8);

  lc.clearDisplay(0);


  // Botones
  pinMode(up, INPUT);
  pinMode(down, INPUT);


  // Semilla aleatoria
  randomSeed(analogRead(A2));


  // Crear obstáculos
  createObstacle(0, 7);
  createObstacle(1, 11);


  // Dibujar
  drawGame();
}


// ======================================================
// LOOP
// ======================================================

void loop() {

  // ----------------------
  // GAME OVER
  // ----------------------

  if (gameOver) {

    showGameOver();

    // Esperar hasta reiniciar
    while (digitalRead(up) == LOW &&
           digitalRead(down) == LOW) {
    }

    delay(300);

    restartGame();

    return;
  }


  // ----------------------
  // LEER BOTONES
  // ----------------------

  int currentUp = digitalRead(up);
  int currentDown = digitalRead(down);


  // Subir
  if (currentUp == HIGH && oldUp == LOW) {

    if (playerY > 0) {
      playerY--;
    }
  }


  // Bajar
  if (currentDown == HIGH && oldDown == LOW) {

    if (playerY < 7) {
      playerY++;
    }
  }


  // Guardar estados
  oldUp = currentUp;
  oldDown = currentDown;


  // ----------------------
  // MOVER OBSTÁCULOS
  // ----------------------

  if (millis() - lastMove >= speed) {

    lastMove = millis();


    for (int i = 0; i < 2; i++) {

      obstacleX[i]--;


      // Cuando sale de la pantalla
      if (obstacleX[i] < 0) {

        // Mandarlo al final
        obstacleX[i] = 11;

        // Crear nuevo hueco
        gapY[i] = random(0, 6);

        // Aumentar puntuación
        score++;
      }
    }
  }


  // ----------------------
  // COLISIONES
  // ----------------------

  checkCollision();


  // ----------------------
  // DIBUJAR
  // ----------------------

  drawGame();
}