// Aula 2: Piano. Cada botão toca uma nota: dó, ré, mi e fá.
// Botões nos pinos 2, 3, 4 e 5 (outro lado no GND); buzzer no pino 8.

const int DO = 2;
const int RE = 3;
const int MI = 4;
const int FA = 5;
const int BUZZER = 8;

void setup() {
  pinMode(DO, INPUT_PULLUP);
  pinMode(RE, INPUT_PULLUP);
  pinMode(MI, INPUT_PULLUP);
  pinMode(FA, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  if (digitalRead(DO) == LOW) {
    tone(BUZZER, 262);
  } else if (digitalRead(RE) == LOW) {
    tone(BUZZER, 294);
  } else if (digitalRead(MI) == LOW) {
    tone(BUZZER, 330);
  } else if (digitalRead(FA) == LOW) {
    tone(BUZZER, 349);
  } else {
    noTone(BUZZER);
  }
}
