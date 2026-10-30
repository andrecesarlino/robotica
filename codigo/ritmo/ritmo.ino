//## Os pinos e as notas | Os 4 botões e as 4 notas ficam em listas: o botão 1 toca a nota 1, e assim por diante.
// Jogo de ritmo: cada botão toca uma nota e avisa a tela qual coluna foi apertada.
// A tela decide se foi na hora certa e manda a cor do LED.

const int BOTOES[4] = {2, 3, 4, 12};
const int NOTAS[4] = {262, 294, 330, 392};  // dó, ré, mi, sol
const int BUZZER = 8;
const int LED_VERMELHO = 7;  // LED RGB
const int LED_VERDE = 6;
const int LED_AZUL = 5;

//## Memória dos botões | Guarda se cada botão já estava apertado.
bool antes[4] = {false, false, false, false};

//## setup | Roda uma vez: os 4 botões como entrada, LED e buzzer como saída.
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

//## Os 4 botões | Para cada botão: se acabou de apertar, manda B1 a B4 para a tela e toca a nota.
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

//## Ouvir a tela | Lê as letras que a tela manda e passa para a função comando.
  while (Serial.available() > 0) {
    int n = Serial.read();  // a tela manda números, iguais aos dos blocos: 1 = r, 2 = g, 3 = b, 4 = y...
    if (n > 0 && n < 12) comando(" rgbyotaefxc"[n]);
  }
  delay(10);
}

//## LED RGB | Acende as cores do LED: HIGH acende, LOW apaga.
void cor(int vermelho, int verde, int azul)
{
  digitalWrite(LED_VERMELHO, vermelho);
  digitalWrite(LED_VERDE, verde);
  digitalWrite(LED_AZUL, azul);
}

//## O que cada letra faz | r vermelho (errou), g verde (acertou), b azul (perfeito), o apaga.
void comando(char letra)
{
  if (letra == 'r') cor(HIGH, LOW, LOW);   // errou
  if (letra == 'g') cor(LOW, HIGH, LOW);   // acertou
  if (letra == 'b') cor(LOW, LOW, HIGH);   // acerto perfeito
  if (letra == 'o') cor(LOW, LOW, LOW);
}
