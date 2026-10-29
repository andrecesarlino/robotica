//## Os pinos | Um nome para cada pino. Se ligar uma peça em outro pino, mude só o número aqui.
// Cabo de guerra: dois jogadores apertam o botão o mais rápido que conseguem.
// Cada aperto vai para a tela (B1 jogador 1, B2 jogador 2); a tela puxa a corda
// e manda de volta a cor de quem está ganhando.

const int BOTAO_1 = 2;
const int BOTAO_2 = 3;
const int BUZZER = 8;
const int LED_VERMELHO = 7;  // LED RGB
const int LED_VERDE = 6;
const int LED_AZUL = 5;

//## Memória dos botões | Guarda se cada botão já estava apertado, para contar um aperto só uma vez.
bool antes1 = false;
bool antes2 = false;

//## setup | Roda uma vez: botões como entrada, LED e buzzer como saída.
void setup()
{
  pinMode(BOTAO_1, INPUT_PULLUP);
  pinMode(BOTAO_2, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  Serial.begin(9600);
}

//## Botão do jogador 1 | Cada aperto manda B1: a corda anda para o lado do jogador 1.
void loop()
{
  bool b1 = digitalRead(BOTAO_1) == LOW;
  if (b1 && !antes1) {
    Serial.println("B1");
  }
  antes1 = b1;

//## Botão do jogador 2 | Cada aperto manda B2. O delay de 15 evita contar um aperto duas vezes.
  bool b2 = digitalRead(BOTAO_2) == LOW;
  if (b2 && !antes2) {
    Serial.println("B2");
  }
  antes2 = b2;

  while (Serial.available() > 0) {
    comando(Serial.read());
  }
  delay(15);  // espera o botão parar de tremer, senão um aperto conta duas vezes
}

//## LED RGB | Acende as cores do LED: HIGH acende, LOW apaga.
void cor(int vermelho, int verde, int azul)
{
  digitalWrite(LED_VERMELHO, vermelho);
  digitalWrite(LED_VERDE, verde);
  digitalWrite(LED_AZUL, azul);
}

//## O que cada letra faz | r e b mostram quem está ganhando, t e a fazem a contagem, f toca o fim da rodada.
void comando(char letra)
{
  if (letra == 'r') cor(HIGH, LOW, LOW);       // jogador 1 ganhando
  if (letra == 'b') cor(LOW, LOW, HIGH);       // jogador 2 ganhando
  if (letra == 'y') cor(HIGH, HIGH, LOW);      // empate
  if (letra == 'g') cor(LOW, HIGH, LOW);
  if (letra == 'o') cor(LOW, LOW, LOW);
  if (letra == 't') tone(BUZZER, 1000, 60);    // contagem 3, 2, 1
  if (letra == 'a') tone(BUZZER, 1600, 300);   // valendo!
  if (letra == 'f') {                          // fim da rodada
    tone(BUZZER, 784, 150);
    delay(170);
    tone(BUZZER, 988, 150);
    delay(170);
    tone(BUZZER, 1319, 300);
  }
}
