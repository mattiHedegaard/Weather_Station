#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Screen setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pin setup
#define UPDATE_BTN D0
#define RED_LED D7
#define YELLOW_LED D6
#define GREEN_LED D5

// Button setup
int lastBtn = HIGH;
unsigned long lastChange = 0;

void setup() {
  // Serial
  Serial.begin(9600);
  Wire.begin(D2,D1);
  delay(500);

  // LED's
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  // Button
  pinMode(UPDATE_BTN, INPUT_PULLUP);

  // Screen 
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)){
    Serial.println(F("SSD1306 not found"));
    for (;;);
  }
}

// Varables
int state = 0;
int states = 4;
const char* screenMsg = "Ready!";
uint16_t textColor = SSD1306_WHITE;
bool redraw = true;

void loop() {
  int btn = digitalRead(UPDATE_BTN);

  // State
  if (CheckBtn(btn)){
  state++;
  if (state == states) {
    state = 0;
  }
  }

  switch(state){
    case 0:
      digitalWrite(RED_LED, LOW);
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      screenMsg = "Ready!";
    break;

    case 1:
      digitalWrite(GREEN_LED, HIGH);
      screenMsg = "Green LED";
    break;

    case 2:
      digitalWrite(YELLOW_LED, HIGH);
      screenMsg = "Yellow LED";
    break;

    case 3:
      digitalWrite(RED_LED, HIGH);
      screenMsg = "Red LED";
    break;
  }

  // Screen
  if (redraw){
    Serial.println("redraw");
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(textColor);
    display.setCursor(0,0);
    display.println(screenMsg);
    display.setTextSize(1);
    display.setTextSize(textColor);
    display.print(F("State: "));
    display.println(state);
    display.display();
    redraw = false;
  }
}

bool CheckBtn(int btn){
  if (btn != lastBtn && millis() - lastChange > 50) {
    lastChange = millis();
    lastBtn = btn;
    if (btn == LOW) {
      redraw = true;
      return true;
    }
  }

  return false;
}