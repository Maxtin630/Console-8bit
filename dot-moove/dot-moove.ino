#include <LedControl.h>

// DIN, CLK, CS, number of devices
LedControl lc(12, 11, 10, 1);

// Joystick pins
const int joyX = A0;
const int joyY = A1;

// LED position
int x = 3;
int y = 3;

// Joystick thresholds
const int jtLOW = 300;
const int jtHIGH = 700;

// Prevent repeated movement while joystick is held
bool moved = false;

void setup() {
  Serial.begin(9600);

  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  lc.setLed(0, y, x, true);
}

void loop() {
  int lecturaX = analogRead(joyX);
  int lecturaY = analogRead(joyY);

  Serial.print("X: ");
  Serial.print(lecturaX);
  Serial.print("  Y: ");
  Serial.println(lecturaY);

  // Joystick returned to center
  if (lecturaX > jtLOW && lecturaX < jtHIGH &&
      lecturaY > jtLOW && lecturaY < jtHIGH) {
    moved = false;
  }

  // Move only once until joystick returns to center
  if (!moved) {

    if (lecturaX < jtLOW && x > 0) {
      x--;
      moved = true;
    }
    else if (lecturaX > jtHIGH && x < 7) {
      x++;
      moved = true;
    }
    else if (lecturaY < jtLOW && y < 7) {
      y--;
      moved = true;
    }
    else if (lecturaY > jtHIGH && y > 0) {
      y++;
      moved = true;
    }

    lc.clearDisplay(0);
    lc.setLed(0, y, x, true);
  }

  delay(20);
}