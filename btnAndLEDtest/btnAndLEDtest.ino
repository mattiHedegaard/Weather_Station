#define UPDATE_BTN D0
#define RED_LED D7
#define YELLOW_LED D6
#define GREEN_LED D5

//Variables
bool testLed = false;
int lastBtn = HIGH;
unsigned long lastChange = 0;

void setup() {
  // Serial
  Serial.begin(9600);

  // LED's
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  // Button
  pinMode(UPDATE_BTN, INPUT_PULLUP);

}

void loop() {
  int btn = digitalRead(UPDATE_BTN);

  if (btn != lastBtn && millis() - lastChange > 50) {
    lastChange = millis();
    lastBtn = btn;
    if (btn == LOW) {
      testLed = !testLed;
    }
  }
  
  if (testLed == HIGH){
    delay(1000);
    digitalWrite(GREEN_LED,HIGH);
    delay(1000);
    digitalWrite(YELLOW_LED, HIGH);
    delay(1000);
    digitalWrite(RED_LED, HIGH);
    delay(1000);
  }
  digitalWrite(RED_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
}