// Desafio: instrumento de um botão só. Segurando o botão, o buzzer toca;
// o potenciômetro escolhe a nota (grave de um lado, agudo do outro).
const int POT = A0;
const int BOTAO = 2;
const int BUZZER = 8;
const int GRAVE = 200;    // mexa aqui: a nota mais grave

void setup() {
  pinMode(BOTAO, INPUT_PULLUP);
}

void loop() {
  int giro = analogRead(POT);
  int nota = giro + GRAVE;   // o giro vai de 0 a 1023; somando, nunca fica grave demais

  if (digitalRead(BOTAO) == LOW) {
    tone(BUZZER, nota);
  } else {
    noTone(BUZZER);
  }
}
