#include "HardwareSerial.h"
#include "esp32-hal-gpio.h"
#include <Arduino.h>
#include "touch_button.h"

TouchEvent CheckTouch(int pin){
  static bool wasPressed = false;
  static bool holdFired = false;
  static unsigned long pressStart = 0;
  const unsigned long holdThreshold = 500;

  bool isPressed = digitalRead(pin) == HIGH;
  TouchEvent event = TOUCH_NONE;

  if (isPressed && !wasPressed){ // Finger touches the button
    pressStart = millis();
    holdFired = false;
  } else if (isPressed && !holdFired && millis() - pressStart >= holdThreshold){ // Held long enough to trigger hold
    holdFired = true;
    event = TOUCH_HOLD;
  } else if (!isPressed && wasPressed && !holdFired) { // Released before hold threshold
    event = TOUCH_TAP;
  }

  wasPressed = isPressed;
  return event;
}

bool TouchIsPressed(TouchEvent touch){
  return touch == TOUCH_TAP;
}

bool TouchIsHold(TouchEvent touch){
  return touch == TOUCH_HOLD;
}