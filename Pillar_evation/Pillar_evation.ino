#include <LedControl.h>

// ======================================================
// MAX7219
// ======================================================

LedControl lc(9, 11, 10, 1);


// ======================================================
// BOTONES
// ======================================================

const int up = 5;
const int down = 4;


// ======================================================
// PERSONAJE
// ======================================================

int playerX = 1;
int playerY = 3;


// ======================================================
// OBSTÁCULOS
// ======================================================

int obstacleX[2];
int gapY[2];

const int gapSize = 3;


// ======================================================
// VELOCIDAD
// ======================================================

// Velocidad inicial
int speed = 400;

// Velocidad mínima
const int minSpeed = 120;

unsigned long lastMove = 0;


// ======================================================
// PUNTUACIÓN
// ======================================================

int score = 0;


// ======================================================
// CONTADOR INVISIBLE DE DIFICULTAD
// ======================================================

// Cada cierta cantidad de puntos,
// el juego se vuelve más rápido.

int difficultyCounter = 0;

const int pointsToIncreaseSpeed = 3;


// ======================================================
// GAME OVER
// ======================================================

bool gameOver = false;


// ======================================================
// ESTADO DE BOTONES
// ======================================================

int oldUp = LOW;
int oldDown = LOW;


// ======================================================
// CREAR OBSTÁCULO
// ======================================================

void createObstacle(int i, int x) {

  obstacleX[i] = x;

  // Posición aleatoria del hueco
  gapY[i] = random(0, 6);
}


// ======================================================
// REINICIAR JUEGO
// ======================================================

void restartGame() {

  playerX = 1;
  playerY = 3;

  score = 0;

  difficultyCounter = 0;

  // Volver a la velocidad inicial
  speed = 400;

  gameOver = false;

  createObstacle(0, 7);
  createObstacle(1, 11);

  lastMove = millis();

  lc.clearDisplay(0);
}


// ======================================================
// DIBUJAR JUEGO
// ======================================================

void drawGame() {

  lc.clearDisplay(0);


  // ----------------------
  // PERSONAJE
  // ----------------------

  lc.setLed(0, playerY, playerX, true);


  // ----------------------
  // OBSTÁCULOS
  // ----------------------

  for (int i = 0; i < 2; i++) {

    if (obstacleX[i] >= 0 &&
        obstacleX[i] <= 7) {

      for (int y = 0; y < 8; y++) {

        // Dibujar todo excepto el hueco
        if (y < gapY[i] ||
            y >= gapY[i] + gapSize) {

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
  // Pared superior/inferior
  // ----------------------

  if (playerY < 0 ||
      playerY > 7) {

    gameOver = true;
    return;
  }


  // ----------------------
  // Obstáculos
  // ----------------------

  for (int i = 0; i < 2; i++) {

    if (obstacleX[i] == playerX) {

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

  // Dibujar X

  for (int i = 0; i < 8; i++) {

    lc.setLed(0, i, i, true);

    lc.setLed(0, i, 7 - i, true);
  }

  delay(1500);
}


// ======================================================
// MOSTRAR PUNTUACIÓN
// ======================================================

void showScore() {

  lc.clearDisplay(0);

  /*
     La puntuación se representa con LEDs.

     Ejemplo:

     5 puntos

     ●
     ●
     ●
     ●
     ●

     Como la matriz es 8x8,
     podemos mostrar hasta 64 puntos.
  */


  int points = score;

  if (points > 64) {
    points = 64;
  }


  // Dibujar puntos de izquierda a derecha
  // y de arriba hacia abajo

  for (int i = 0; i < points; i++) {

    int y = i / 8;
    int x = i % 8;

    lc.setLed(0, y, x, true);
  }


  // Mantener la puntuación visible
  delay(3000);
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


  // Iniciar obstáculos

  createObstacle(0, 7);
  createObstacle(1, 11);


  // Dibujar juego

  drawGame();
}


// ======================================================
// LOOP
// ======================================================

void loop() {

  // ====================================================
  // GAME OVER
  // ====================================================

  if (gameOver) {

    // Primero mostrar X

    showGameOver();


    // Después mostrar puntuación

    showScore();


    // Esperar un poco

    delay(500);


    // Reiniciar

    restartGame();

    return;
  }


  // ====================================================
  // LEER BOTONES
  // ====================================================

  int currentUp =
    digitalRead(up);

  int currentDown =
    digitalRead(down);


  // ====================================================
  // SUBIR
  // ====================================================

  if (currentUp == HIGH &&
      oldUp == LOW) {

    if (playerY > 0) {

      playerY--;
    }
  }


  // ====================================================
  // BAJAR
  // ====================================================

  if (currentDown == HIGH &&
      oldDown == LOW) {

    if (playerY < 7) {

      playerY++;
    }
  }


  // Guardar estado de botones

  oldUp = currentUp;
  oldDown = currentDown;


  // ====================================================
  // MOVER OBSTÁCULOS
  // ====================================================

  if (millis() - lastMove >= speed) {

    lastMove = millis();


    for (int i = 0; i < 2; i++) {

      obstacleX[i]--;


      // ==================================================
      // OBSTÁCULO COMPLETAMENTE FUERA
      // ==================================================

      if (obstacleX[i] < 0) {

        // Mandar al final

        obstacleX[i] = 11;


        // Crear nuevo hueco

        gapY[i] = random(0, 6);


        // ==================================================
        // AUMENTAR PUNTUACIÓN
        // ==================================================

        score++;


        // ==================================================
        // CONTADOR INVISIBLE
        // ==================================================

        difficultyCounter++;


        // Cada 3 obstáculos superados
        // aumenta la velocidad

        if (difficultyCounter >= pointsToIncreaseSpeed) {

          difficultyCounter = 0;


          // Aumentar velocidad

          if (speed > minSpeed) {

            speed -= 40;
          }
        }
      }
    }
  }


  // ====================================================
  // COMPROBAR COLISIONES
  // ====================================================

  checkCollision();


  // ====================================================
  // DIBUJAR
  // ====================================================

  drawGame();
}