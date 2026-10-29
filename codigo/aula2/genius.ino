// Genius: repita a sequência de luzes e sons.
// Botões nos pinos 2 a 5 (INPUT_PULLUP), LEDs nos pinos 9 a 12, buzzer no pino 8.
int botoes[] = {2, 3, 4, 5};
int leds[]   = {9, 10, 11, 12};
int notas[]  = {262, 330, 392, 523};
int sequencia[100];
int tamanho = 0;

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(botoes[i], INPUT_PULLUP);
    pinMode(leds[i], OUTPUT);
  }
  randomSeed(analogRead(A0)); // sorteio diferente a cada vez
}

void tocar(int cor) {
  digitalWrite(leds[cor], HIGH);
  tone(8, notas[cor], 300);
  delay(400);
  digitalWrite(leds[cor], LOW);
}

int lerBotao() {
  while (true) {
    for (int i = 0; i < 4; i++) {
      if (digitalRead(botoes[i]) == LOW) {
        tocar(i);
        while (digitalRead(botoes[i]) == LOW) {} // espera soltar
        delay(50);
        return i;
      }
    }
  }
}

void loop() {
  if (tamanho == 100) tamanho = 0;
  sequencia[tamanho] = random(4); // sorteia a próxima cor
  tamanho++;
  delay(800);
  for (int i = 0; i < tamanho; i++) tocar(sequencia[i]);

  for (int i = 0; i < tamanho; i++) {
    if (lerBotao() != sequencia[i]) {
      tone(8, 150, 1000); // errou: som grave
      delay(1500);
      tamanho = 0;        // recomeça do zero
      return;
    }
  }
}
