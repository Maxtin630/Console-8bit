#include <LedControl.h> // Librería para controlar el MAX7219 (matriz 8x8)

// Creamos el objeto para la matriz (DIN=12, CLK=11, CS=10, 1 pantalla)
LedControl lc(9, 11, 10, 1);

// Pines del joystick
const int joyX = A0; // eje X
const int joyY = A1; // eje Y

// Arrays para guardar el cuerpo de la serpiente (hasta 64 bloques)
int snakeX[64];
int snakeY[64];

// Longitud inicial de la serpiente
int length = 3;

// Dirección actual de movimiento
int dx = 1; // movimiento en X (1 = derecha, -1 = izquierda)
int dy = 0; // movimiento en Y (1 = abajo, -1 = arriba)

// Posición de la comida
int foodX;
int foodY;

// Estado del juego
bool gameOver = false;

// Control de tiempo para mover la serpiente
unsigned long lastMove = 0;
const int speed = 350; // velocidad (menor = más rápido)
const int right = 3;
const int down  = 4;
const int up    = 5;
const int left  = 6;

const int ledPin = 13;

// ----------------------
// Estado actual de los botones
// ----------------------
int buttonState_right = LOW;
int buttonState_down  = LOW;
int buttonState_up    = LOW;
int buttonState_left  = LOW;

// ----------------------
// Estado anterior de los botones
// ----------------------
int a_right = LOW;
int a_down  = LOW;
int a_up    = LOW;
int a_left  = LOW;

// Función para generar comida en posición aleatoria
void spawnFood() {
  bool ok;

  do {
    ok = true;

    // posición aleatoria en la matriz (0 a 7)
    foodX = random(0, 8);
    foodY = random(0, 8);

    // evitar que la comida aparezca dentro de la serpiente
    for (int i = 0; i < length; i++) {
      if (snakeX[i] == foodX && snakeY[i] == foodY) {
        ok = false; // si coincide, se repite
      }
    }
  } while (!ok);
}

void setup() {
  // activar la matriz MAX7219
  lc.shutdown(0, false);

  // brillo de la matriz (0 a 15)
  lc.setIntensity(0, 8);

  // limpiar pantalla
  lc.clearDisplay(0);

  // semilla aleatoria para la comida
  randomSeed(analogRead(A2));

  // posición inicial de la serpiente (3 bloques)
  snakeX[0] = 3;
  snakeY[0] = 3;

  snakeX[1] = 2;
  snakeY[1] = 3;

  snakeX[2] = 1;
  snakeY[2] = 3;

  // crear primera comida
  spawnFood();
    pinMode(right, INPUT);
  pinMode(down, INPUT);
  pinMode(up, INPUT);
  pinMode(left, INPUT);
}

void loop() {
  // Leer el estado actual de cada botón
  buttonState_right = digitalRead(right);
  buttonState_down  = digitalRead(down);
  buttonState_up    = digitalRead(up);
  buttonState_left  = digitalRead(left);
  // si el juego terminó
  if (gameOver) {
    lc.clearDisplay(0); // limpiar matriz

    // dibujar una X como pantalla de game over
    for (int i = 0; i < 8; i++) {
      lc.setLed(0, i, i, true);
      lc.setLed(0, i, 7 - i, true);
    }

    // detener el programa aquí
    while (1);
  }

  // leer joystick
  int x = analogRead(joyX);
  int y = analogRead(joyY);

  // mover izquierda / derecha
  if (buttonState_left == HIGH && a_left == LOW && dx != 1) { // izquierda
    dx = -1;
    dy = 0;
  }
  else if (buttonState_right == HIGH && a_right == LOW && dx != -1) { // derecha
    dx = 1;
    dy = 0;
  }

  // mover arriba / abajo (Y INVERTIDO)
  if (buttonState_down == HIGH && a_down == LOW && dy != -1) { // abajo
    dx = 0;
    dy = 1;
  }
  else if (buttonState_up == HIGH && a_up == LOW && dy != 1) { // arriba
    dx = 0;
    dy = -1;
  }

  // controlar velocidad del movimiento
  if (millis() - lastMove > speed) {
    lastMove = millis();

    // mover el cuerpo de la serpiente
    for (int i = length - 1; i > 0; i--) {
      snakeX[i] = snakeX[i - 1];
      snakeY[i] = snakeY[i - 1];
    }

    // mover la cabeza
    snakeX[0] += dx;
    snakeY[0] += dy;

    // choque con paredes
    if (snakeX[0] < 0 || snakeX[0] > 7 ||
        snakeY[0] < 0 || snakeY[0] > 7) {
      gameOver = true;
    }

    // choque con el cuerpo
    for (int i = 1; i < length; i++) {
      if (snakeX[0] == snakeX[i] &&
          snakeY[0] == snakeY[i]) {
        gameOver = true;
      }
    }

    // comer comida
    if (snakeX[0] == foodX &&
        snakeY[0] == foodY) {

      // aumentar longitud
      snakeX[length] = snakeX[length - 1];
      snakeY[length] = snakeY[length - 1];
      length++;

      // generar nueva comida
      if (length < 64) {
        spawnFood();
      }
    }

    // limpiar pantalla antes de dibujar
    lc.clearDisplay(0);

    // dibujar comida
    lc.setLed(0, foodY, foodX, true);

    // dibujar serpiente
    for (int i = 0; i < length; i++) {
      lc.setLed(0, snakeY[i], snakeX[i], true);
    }
  }
}