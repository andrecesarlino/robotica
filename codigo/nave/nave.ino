//## Os pinos | Um nome para cada pino. Se ligar uma peça em outro pino, mude só o número aqui.
// Jogo da nave: a mão sobre o sensor sobe e desce a nave na tela.
// O Arduino manda a distância e o botão para a tela; a tela manda de volta
// uma letra para acender o LED ou tocar o buzzer.

const int BOTAO = 2;         // atira
const int TRIG = 10;
const int ECHO = 11;
const int BUZZER = 8;
const int LED_VERMELHO = 7;  // LED RGB
const int LED_VERDE = 6;
const int LED_AZUL = 5;

//## Memória do botão | Guarda se o botão já estava apertado, para contar um aperto só uma vez.
bool apertadoAntes = false;

//## setup | Roda uma vez: diz quais pinos são entrada e quais são saída e liga o Monitor Serial.
void setup()
{
  pinMode(BOTAO, INPUT_PULLUP);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AZUL, OUTPUT);
  Serial.begin(9600);
}

//## Sensor ultrassônico | Mede a distância da mão e manda para a tela, por exemplo D23.
void loop()
{
  // mede a distância e manda "D" + centímetros, por exemplo D23
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long tempo = pulseIn(ECHO, HIGH, 20000);
  int distancia = tempo / 58;
  Serial.print("D");
  Serial.println(distancia);

//## Botão | No momento em que aperta, manda B1 para a tela (a nave atira).
  // botão: manda B1 só no momento em que aperta
  bool apertado = digitalRead(BOTAO) == LOW;
  if (apertado && !apertadoAntes) {
    Serial.println("B1");
  }
  apertadoAntes = apertado;

//## Ouvir a tela | Lê as letras que a tela manda e passa para a função comando.
  // letras que a tela mandou
  while (Serial.available() > 0) {
    int n = Serial.read();  // a tela manda números, iguais aos dos blocos: 1 = r, 2 = g, 3 = b, 4 = y...
    if (n > 0 && n < 12) comando(" rgbyotaefxc"[n]);
  }
  delay(30);
}

//## LED RGB | Acende as cores do LED: HIGH acende, LOW apaga.
void cor(int vermelho, int verde, int azul)
{
  digitalWrite(LED_VERMELHO, vermelho);
  digitalWrite(LED_VERDE, verde);
  digitalWrite(LED_AZUL, azul);
}

//## O que cada letra faz | r, g, b, y mudam a cor do LED; t, a, e tocam os sons do jogo.
void comando(char letra)
{
  if (letra == 'r') cor(HIGH, LOW, LOW);       // vermelho
  if (letra == 'g') cor(LOW, HIGH, LOW);       // verde
  if (letra == 'b') cor(LOW, LOW, HIGH);       // azul
  if (letra == 'y') cor(HIGH, HIGH, LOW);      // amarelo
  if (letra == 'o') cor(LOW, LOW, LOW);        // apaga
  if (letra == 't') tone(BUZZER, 1800, 20);    // tiro
  if (letra == 'a') tone(BUZZER, 1200, 80);    // acertou
  if (letra == 'e') tone(BUZZER, 150, 400);    // bateu
}
