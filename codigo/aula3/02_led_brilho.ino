// 2: o potenciômetro controla o brilho do LED.
const int POT = A0;
const int LED = 9;

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int pot = analogRead(POT);
  Serial.println(pot);

  int brilho = pot / 4;
  analogWrite(LED, brilho);

  delay(100);
}
