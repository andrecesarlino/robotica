// Desafio: cada aperto do botão troca a cor do LED RGB:
// vermelho, verde, azul e de volta ao vermelho.
const int BOTAO = 2;
const int VERMELHO = 10;
const int VERDE = 6;
const int AZUL = 5;

int cor = 0;   // 0 = vermelho, 1 = verde, 2 = azul

void setup() {
  pinMode(BOTAO, INPUT_PULLUP);
  pinMode(VERMELHO, OUTPUT);
  pinMode(VERDE, OUTPUT);
  pinMode(AZUL, OUTPUT);
}

void loop() {
  if (digitalRead(BOTAO) == LOW) {
    cor = cor + 1;
    if (cor > 2) {
      cor = 0;
    }
    while (digitalRead(BOTAO) == LOW) {
    }
    delay(50);
  }

  digitalWrite(VERMELHO, LOW);
  digitalWrite(VERDE, LOW);
  digitalWrite(AZUL, LOW);

  if (cor == 0) {
    digitalWrite(VERMELHO, HIGH);
  } else if (cor == 1) {
    digitalWrite(VERDE, HIGH);
  } else {
    digitalWrite(AZUL, HIGH);
  }
}
