/*
  Lectura de 4 botones con detección de flanco.
  Imprime una sola vez cuando se presiona cada botón.
*/

// ----------------------
// Pines
// ----------------------
const int right = 5; //3
const int down  = 4;//4
const int up    = 2;//5
const int left  = 3;//6

const int ledPin = 13;

// ----------------------
// Estado actual de los botones
// ----------------------
int buttonState_right = HIGH;
int buttonState_down  = HIGH;
int buttonState_up    = HIGH;
int buttonState_left  = HIGH;

// ----------------------
// Estado anterior de los botones
// ----------------------
int a_right = HIGH;
int a_down  = HIGH;
int a_up    = HIGH;
int a_left  = HIGH;

void setup() {

  // LED
  pinMode(ledPin, OUTPUT);

  // Comunicación serial
  Serial.begin(9600);

  // Configuración de botones
  // Si usas resistencias externas de 10k, deja INPUT.
  // Si NO las usas, cambia INPUT por INPUT_PULLUP.
  pinMode(right, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(up, INPUT_PULLUP);
  pinMode(left, INPUT_PULLUP);
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

  if (buttonState_right == LOW && a_right == HIGH) {
    Serial.println("Right");
  }

  if (buttonState_down == LOW && a_down == HIGH) {
    Serial.println("Down");
  }

  if (buttonState_up == LOW && a_up == HIGH) {
    Serial.println("Up");
  }

  if (buttonState_left == LOW && a_left == HIGH) {
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