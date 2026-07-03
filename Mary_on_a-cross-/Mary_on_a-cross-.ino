/*
  Lectura de 4 botones con detección de flanco.
  Imprime una sola vez cuando se presiona cada botón.
*/

// ----------------------
// Pines
// ----------------------
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

void setup() {

  // LED
  pinMode(ledPin, OUTPUT);

  // Comunicación serial
  Serial.begin(9600);

  // Configuración de botones
  // Si usas resistencias externas de 10k, deja INPUT.
  // Si NO las usas, cambia INPUT por INPUT_PULLUP.
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

  // ----------------------
  // Detectar nueva pulsación
  // ----------------------

  if (buttonState_right == HIGH && a_right == LOW) {
    Serial.println("Right");
  }

  if (buttonState_down == HIGH && a_down == LOW) {
    Serial.println("Down");
  }

  if (buttonState_up == HIGH && a_up == LOW) {
    Serial.println("Up");
  }

  if (buttonState_left == HIGH && a_left == LOW) {
    Serial.println("Left");
  }

  // Guardar el estado actual para la siguiente vuelta
  a_right = buttonState_right;
  a_down  = buttonState_down;
  a_up    = buttonState_up;
  a_left  = buttonState_left;

  // Pequeño antirrebote
  delay(20);
}