#define RGB_RED D7
#define RGB_GREEN D6
#define RGB_BLUE D5

void setup() {
  pinMode(RGB_RED, OUTPUT);
  pinMode(RGB_GREEN, OUTPUT);
  pinMode(RGB_BLUE, OUTPUT);

}

void loop() {
  RgbRed();
  delay(1000);

  RgbGreen();
  delay(1000);

  RgbBlue();
  delay(1000);

  RgbYellow();
  delay(1000);

  RgbPurple();
  delay(1000);

  RgbCyan();
  delay(1000);

  RgbWhite();
  delay(1000);

}

void TurnOffRGB(){
  digitalWrite(RGB_RED, LOW);
  digitalWrite(RGB_GREEN, LOW);
  digitalWrite(RGB_BLUE, LOW);
}
void RgbRed(){
  digitalWrite(RGB_RED, HIGH);
  digitalWrite(RGB_GREEN, LOW);
  digitalWrite(RGB_BLUE, LOW);
}
void RgbGreen(){
  digitalWrite(RGB_RED, LOW);
  digitalWrite(RGB_GREEN, HIGH);
  digitalWrite(RGB_BLUE, LOW);
}
void RgbBlue(){
  digitalWrite(RGB_RED, LOW);
  digitalWrite(RGB_GREEN, LOW);
  digitalWrite(RGB_BLUE, HIGH);
}
void RgbYellow(){
  digitalWrite(RGB_RED, HIGH);
  digitalWrite(RGB_GREEN, HIGH);
  digitalWrite(RGB_BLUE, LOW);
}
void RgbPurple(){
  digitalWrite(RGB_RED, HIGH);
  digitalWrite(RGB_GREEN, LOW);
  digitalWrite(RGB_BLUE, HIGH);
}
void RgbCyan(){
  digitalWrite(RGB_RED, LOW);
  digitalWrite(RGB_GREEN, HIGH);
  digitalWrite(RGB_BLUE, HIGH);
}
void RgbWhite(){
  digitalWrite(RGB_RED, HIGH);
  digitalWrite(RGB_GREEN, HIGH);
  digitalWrite(RGB_BLUE, HIGH);
}