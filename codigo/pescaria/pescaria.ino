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

Servo garra;
int fundo = 0;                 // distância até o fundo do pote vazio
unsigned long ultimoPonto = 0;

void setup()
{
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUZZER, OUTPUT);
  garra.attach(PINO_GARRA);
  Serial.begin(9600);
  medirFundo();
}

void loop()
{
  garra.write(map(analogRead(JOY_X), 0, 1023, GARRA_ABERTA, GARRA_FECHADA));

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
