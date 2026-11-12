#include <Arduino.h>

struct touch {
  byte wasPressed = LOW;
  byte isPressed = LOW;
};

touch touch;

bool IsTouchPressed(int pin){
  touch.isPressed = digitalRead(pin) == HIGH;

  bool pressed = (touch.isPressed && !touch.wasPressed) ? true : false;

  touch.wasPressed = touch.isPressed;

  return pressed;
}