const int LED = 9;
const int LDR = A1;
const int LIMITE = 900;  
const int BUZZER = 8;
const int TOM = 1000;  

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int luz = analogRead(LDR);

  Serial.println(luz);

  if (luz < LIMITE) {
    digitalWrite(LED, HIGH);
    tone(BUZZER, TOM);
  } else {
    digitalWrite(LED, LOW);
    noTone(BUZZER);
  }

  delay(100);
}
