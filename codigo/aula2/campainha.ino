// Aula 2: Campainha. Enquanto o botão está apertado, o buzzer toca.
// Botão no pino 2 e no GND (sem resistor, usa o INPUT_PULLUP); buzzer no pino 8.

const int BOTAO = 2;
const int BUZZER = 8;

void setup() {
  pinMode(BOTAO, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
}

void loop() {
  if (digitalRead(BOTAO) == LOW) {
    tone(BUZZER, 440);
  } else {
    noTone(BUZZER);
  }
}
