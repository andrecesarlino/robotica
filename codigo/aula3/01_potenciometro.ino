const int POT = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int pot = analogRead(POT);
  Serial.println(pot);

  delay(100);
}
