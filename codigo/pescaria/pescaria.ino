//## Biblioteca e pinos | A biblioteca Servo controla a garra. 150 e 60 são a garra aberta e fechada.
// Pescaria com a garra: o joystick abre e fecha a garra e o sensor, em cima do
// pote olhando para o fundo, percebe cada bolinha que cai. Cada bolinha manda
// "PONTO" para a tela.

#include <Servo.h>

const int PINO_GARRA = 9;
const int JOY_X = A0;          // VRx do joystick
const int TRIG = 10;
const int ECHO = 11;
const int BUZZER = 8;
const int GARRA_ABERTA = 150;  // mesmos números da garra na aula 4
const int GARRA_FECHADA = 60;

//## Variáveis | A garra, a distância do pote vazio (fundo) e a hora do último ponto.
Servo garra;
int fundo = 0;                 // distância até o fundo do pote vazio
unsigned long ultimoPonto = 0;

//## setup | Roda uma vez: prepara os pinos, liga a garra no pino 9 e mede o pote vazio.
void setup()
{
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUZZER, OUTPUT);
  garra.attach(PINO_GARRA);
  Serial.begin(9600);
  medirFundo();
}

//## Garra no joystick | Lê o joystick no A0 e gira a garra entre aberta e fechada.
void loop()
{
  garra.write(map(analogRead(JOY_X), 0, 1023, GARRA_ABERTA, GARRA_FECHADA));

//## Contar ponto | Se a distância ficar menor que o fundo, caiu uma bolinha: manda PONTO.
  // bolinha passando na frente do sensor: a distância fica bem menor que o fundo
  int d = medirDistancia();
  if (d > 0 && d < fundo - 3 && millis() - ultimoPonto > 800) {
    Serial.println("PONTO");
    ultimoPonto = millis();
  }

  while (Serial.available() > 0) {
    comando(Serial.read());
  }
  delay(15);
}

//## Medir a distância | Igual ao radar: manda o som e mede quanto tempo o eco demora.
int medirDistancia()
{
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  return pulseIn(ECHO, HIGH, 6000) / 58;  // até uns 100 cm
}

// mede o pote vazio 10 vezes e guarda a maior distância
//## Medir o pote vazio | Mede 10 vezes e guarda a maior distância como o fundo do pote.
void medirFundo()
{
  fundo = 0;
  for (int i = 0; i < 10; i++) {
    int d = medirDistancia();
    if (d > fundo) fundo = d;
    delay(30);
  }
  Serial.print("FUNDO");
  Serial.println(fundo);
}

//## O que cada letra faz | c mede o pote de novo, a é o som do ponto, t a contagem, f o fim.
void comando(char letra)
{
  if (letra == 'c') medirFundo();             // a tela pede antes de começar
  if (letra == 'a') tone(BUZZER, 1500, 120);  // ponto
  if (letra == 't') tone(BUZZER, 1000, 60);   // contagem
  if (letra == 'f') {                         // acabou o tempo
    tone(BUZZER, 880, 200);
    delay(220);
    tone(BUZZER, 660, 200);
    delay(220);
    tone(BUZZER, 440, 500);
  }
}
