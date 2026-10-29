//## Os pinos | Um nome para cada pino. Se ligar uma peça em outro pino, mude só o número aqui.
// Desarme a bomba: o potenciômetro escolhe o número e o botão trava o dígito.
// O Arduino manda o potenciômetro (P0 a P1023) e o botão (B1) para a tela;
// a tela manda o tique-taque, as cores e a explosão.

const int POT = A0;
const int BOTAO = 2;
const int BUZZER = 8;
const int LED_VERMELHO = 7;  // LED RGB
const int LED_VERDE = 6;
const int LED_AZUL = 5;

//## Memória do botão | Guarda se o botão já estava apertado, para contar um aperto só uma vez.
bool apertadoAntes = false;

//## setup | Roda uma vez: botão como entrada, LED e buzzer como saída.
void setup()
{
  pinMode(BOTAO, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  Serial.begin(9600);
}

//## Potenciômetro | Manda para a tela P e o valor do potenciômetro, de 0 a 1023. A tela transforma em 0 a 9.
void loop()
{
  Serial.print("P");
  Serial.println(analogRead(POT));

//## Botão | No momento em que aperta, manda B1: tenta o número escolhido.
  bool apertado = digitalRead(BOTAO) == LOW;
  if (apertado && !apertadoAntes) {
    Serial.println("B1");
  }
  apertadoAntes = apertado;

  while (Serial.available() > 0) {
    comando(Serial.read());
  }
  delay(40);
}

//## LED RGB | Acende as cores do LED: HIGH acende, LOW apaga.
void cor(int vermelho, int verde, int azul)
{
  digitalWrite(LED_VERMELHO, vermelho);
  digitalWrite(LED_VERDE, verde);
  digitalWrite(LED_AZUL, azul);
}

//## O que cada letra faz | t é o tique-taque, a o acerto, e o erro, f desarmou e x a explosão.
void comando(char letra)
{
  if (letra == 'r') cor(HIGH, LOW, LOW);
  if (letra == 'g') cor(LOW, HIGH, LOW);
  if (letra == 'y') cor(HIGH, HIGH, LOW);
  if (letra == 'o') cor(LOW, LOW, LOW);
  if (letra == 't') tone(BUZZER, 2000, 15);    // tique-taque
  if (letra == 'a') tone(BUZZER, 1300, 120);   // dígito certo
  if (letra == 'e') tone(BUZZER, 300, 250);    // dígito errado
  if (letra == 'f') {                          // desarmou
    tone(BUZZER, 1047, 120);
    delay(140);
    tone(BUZZER, 1319, 120);
    delay(140);
    tone(BUZZER, 1568, 400);
  }
  if (letra == 'x') {                          // explodiu
    for (int f = 600; f > 60; f = f - 20) {
      tone(BUZZER, f, 30);
      delay(25);
    }
  }
}
