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

bool apertadoAntes = false;

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

  // botão: manda B1 só no momento em que aperta
  bool apertado = digitalRead(BOTAO) == LOW;
  if (apertado && !apertadoAntes) {
    Serial.println("B1");
  }
  apertadoAntes = apertado;

  // letras que a tela mandou
  while (Serial.available() > 0) {
    comando(Serial.read());
  }
  delay(30);
}

void cor(int vermelho, int verde, int azul)
{
  digitalWrite(LED_VERMELHO, vermelho);
  digitalWrite(LED_VERDE, verde);
  digitalWrite(LED_AZUL, azul);
}

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
