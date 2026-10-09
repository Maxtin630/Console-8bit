#include <LedControl.h>

// ======================================================
// MAX7219
// ======================================================

LedControl lc(9, 11, 10, 1);


// ======================================================
// BOTONES
// ======================================================

const int right = 2;
const int down  = 5;
const int up    = 3;
const int left  = 4;


// ======================================================
// SERPIENTE
// ======================================================

int snakeX[64];
int snakeY[64];

int length = 3;


// ======================================================
// DIRECCIÓN
// ======================================================

int dx = 1;
int dy = 0;


// ======================================================
// COMIDA
// ======================================================

int foodX;
int foodY;


// ======================================================
// JUEGO
// ======================================================

bool gameOver = false;
bool victory = false;


// ======================================================
// PUNTUACIÓN
// ======================================================

int score = 0;


// ======================================================
// VELOCIDAD
// ======================================================

unsigned long lastMove = 0;
const int speed = 350;


// ======================================================
// ESTADO ACTUAL DE LOS BOTONES
// ======================================================

int buttonState_right = LOW;
int buttonState_down  = LOW;
int buttonState_up    = LOW;
int buttonState_left  = LOW;


// ======================================================
// ESTADO ANTERIOR DE LOS BOTONES
// ======================================================

int a_right = LOW;
int a_down  = LOW;
int a_up    = LOW;
int a_left  = LOW;


// ======================================================
// CREAR COMIDA
// ======================================================

void spawnFood() {

  bool ok;

  do {

    ok = true;

    foodX = random(0, 8);
    foodY = random(0, 8);


    // Comprobar que no aparezca
    // dentro de la serpiente

    for (int i = 0; i < length; i++) {

      if (snakeX[i] == foodX &&
          snakeY[i] == foodY) {

        ok = false;
      }
    }

  } while (!ok);
}


// ======================================================
// MOSTRAR PUNTUACIÓN
// ======================================================

void showScore() {

  lc.clearDisplay(0);

  /*
     La puntuación se representa
     con puntos.

     Cada LED = 1 punto.

     Máximo 64 puntos.
  */

  int points = score;

  if (points > 64) {
    points = 64;
  }


  for (int i = 0; i < points; i++) {

    int y = i / 8;
    int x = i % 8;

    lc.setLed(0, y, x, true);
  }

  delay(3000);
}


// ======================================================
// MOSTRAR GAME OVER
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
// MOSTRAR CORONA
// ======================================================

void showCrown() {

  lc.clearDisplay(0);


  /*
       CORONA

       10000001
       11000011
       10100101
       10111101
       11111111
       01111110
       01111110
       00000000
  */


  lc.setRow(0, 0, B10000001);
  lc.setRow(0, 1, B11000011);
  lc.setRow(0, 2, B10100101);
  lc.setRow(0, 3, B10111101);
  lc.setRow(0, 4, B11111111);
  lc.setRow(0, 5, B01111110);
  lc.setRow(0, 6, B01111110);
  lc.setRow(0, 7, B00000000);


  // Mantener la corona visible

  delay(4000);
}


// ======================================================
// REINICIAR JUEGO
// ======================================================

void restartGame() {

  length = 3;

  dx = 1;
  dy = 0;

  score = 0;

  gameOver = false;
  victory = false;


  // Posición inicial

  snakeX[0] = 3;
  snakeY[0] = 3;

  snakeX[1] = 2;
  snakeY[1] = 3;

  snakeX[2] = 1;
  snakeY[2] = 3;


  spawnFood();


  lastMove = millis();


  lc.clearDisplay(0);
}


// ======================================================
// SETUP
// ======================================================

void setup() {

  // MAX7219

  lc.shutdown(0, false);

  lc.setIntensity(0, 8);

  lc.clearDisplay(0);


  // Semilla aleatoria

  randomSeed(analogRead(A2));


  // Botones

  pinMode(right, INPUT);
  pinMode(down, INPUT);
  pinMode(up, INPUT);
  pinMode(left, INPUT);


  // Serpiente inicial

  snakeX[0] = 3;
  snakeY[0] = 3;

  snakeX[1] = 2;
  snakeY[1] = 3;

  snakeX[2] = 1;
  snakeY[2] = 3;


  // Primera comida

  spawnFood();
}


// ======================================================
// LOOP
// ======================================================

void loop() {


  // ====================================================
  // GAME OVER
  // ====================================================

  if (gameOver) {

    showGameOver();

    showScore();

    restartGame();

    return;
  }


  // ====================================================
  // VICTORIA
  // ====================================================

  if (victory) {

    showCrown();

    showScore();

    restartGame();

    return;
  }


  // ====================================================
  // LEER BOTONES
  // ====================================================

  buttonState_right = digitalRead(right);
  buttonState_down  = digitalRead(down);
  buttonState_up    = digitalRead(up);
  buttonState_left  = digitalRead(left);


  // ====================================================
  // CAMBIAR DIRECCIÓN
  // ====================================================

  // IZQUIERDA

  if (buttonState_left == HIGH &&
      a_left == LOW &&
      dx != 1) {

    dx = -1;
    dy = 0;
  }


  // DERECHA

  else if (buttonState_right == HIGH &&
           a_right == LOW &&
           dx != -1) {

    dx = 1;
    dy = 0;
  }


  // ABAJO

  if (buttonState_down == HIGH &&
      a_down == LOW &&
      dy != -1) {

    dx = 0;
    dy = 1;
  }


  // ARRIBA

  else if (buttonState_up == HIGH &&
           a_up == LOW &&
           dy != 1) {

    dx = 0;
    dy = -1;
  }


  // ====================================================
  // GUARDAR ESTADO ANTERIOR
  // ====================================================

  a_right = buttonState_right;
  a_down  = buttonState_down;
  a_up    = buttonState_up;
  a_left  = buttonState_left;


  // ====================================================
  // MOVER SERPIENTE
  // ====================================================

  if (millis() - lastMove > speed) {

    lastMove = millis();


    // -----------------------------------------------
    // Mover cuerpo
    // -----------------------------------------------

    for (int i = length - 1; i > 0; i--) {

      snakeX[i] = snakeX[i - 1];
      snakeY[i] = snakeY[i - 1];
    }


    // -----------------------------------------------
    // Mover cabeza
    // -----------------------------------------------

    snakeX[0] += dx;
    snakeY[0] += dy;


    // -----------------------------------------------
    // CHOQUE CON PAREDES
    // -----------------------------------------------

    if (snakeX[0] < 0 ||
        snakeX[0] > 7 ||
        snakeY[0] < 0 ||
        snakeY[0] > 7) {

      gameOver = true;
    }


    // -----------------------------------------------
    // CHOQUE CON EL CUERPO
    // -----------------------------------------------

    for (int i = 1; i < length; i++) {

      if (snakeX[0] == snakeX[i] &&
          snakeY[0] == snakeY[i]) {

        gameOver = true;
      }
    }


    // -----------------------------------------------
    // COMER COMIDA
    // -----------------------------------------------

    if (snakeX[0] == foodX &&
        snakeY[0] == foodY) {


      // Aumentar puntuación

      score++;


      // Aumentar serpiente

      if (length < 64) {

        snakeX[length] = snakeX[length - 1];
        snakeY[length] = snakeY[length - 1];

        length++;
      }


      // ---------------------------------------------
      // ¿LLENÓ TODA LA MATRIZ?
      // ---------------------------------------------

      if (length >= 64) {

        length = 64;

        victory = true;
      }


      // ---------------------------------------------
      // Crear nueva comida
      // ---------------------------------------------

      else {

        spawnFood();
      }
    }


    // =================================================
    // DIBUJAR
    // =================================================

    lc.clearDisplay(0);


    // Dibujar comida

    if (!victory) {

      lc.setLed(
        0,
        foodY,
        foodX,
        true
      );
    }


    // Dibujar serpiente

    for (int i = 0; i < length; i++) {

      lc.setLed(
        0,
        snakeY[i],
        snakeX[i],
        true
      );
    }
  }
}