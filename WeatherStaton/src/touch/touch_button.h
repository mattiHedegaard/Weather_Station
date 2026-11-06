#pragma once

enum TouchEvent {TOUCH_NONE, TOUCH_TAP, TOUCH_HOLD};
TouchEvent CheckTouch(int pin);

bool TouchIsPressed(TouchEvent touch);
bool TouchIsHold(TouchEvent touch);