// Jogo de ritmo: cada botão toca uma nota e avisa a tela qual coluna foi apertada.
// A tela decide se foi na hora certa e manda a cor do LED.

const int BOTOES[4] = {2, 3, 4, 12};
const int NOTAS[4] = {262, 294, 330, 392};  // dó, ré, mi, sol
const int BUZZER = 8;
const int LED_VERMELHO = 7;  // LED RGB
const int LED_VERDE = 6;
const int LED_AZUL = 5;

bool antes[4] = {false, false, false, false};

void setup()
{
  for (int i = 0; i < 4; i++) {
    pinMode(BOTOES[i], INPUT_PULLUP);
  }
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  for (int i = 0; i < 4; i++) {
    bool apertado = digitalRead(BOTOES[i]) == LOW;
    if (apertado && !antes[i]) {
      Serial.print("B");
      Serial.println(i + 1);      // B1, B2, B3 ou B4
      tone(BUZZER, NOTAS[i], 150);
    }
    antes[i] = apertado;
  }

  while (Serial.available() > 0) {
    comando(Serial.read());
  }
  delay(10);
}

void cor(int vermelho, int verde, int azul)
{
  digitalWrite(LED_VERMELHO, vermelho);
  digitalWrite(LED_VERDE, verde);
  digitalWrite(LED_AZUL, azul);
}

void comando(char letra)
{
  if (letra == 'r') cor(HIGH, LOW, LOW);   // errou
  if (letra == 'g') cor(LOW, HIGH, LOW);   // acertou
  if (letra == 'b') cor(LOW, LOW, HIGH);   // acerto perfeito
  if (letra == 'o') cor(LOW, LOW, LOW);
}
