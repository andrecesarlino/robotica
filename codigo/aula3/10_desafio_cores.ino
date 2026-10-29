// 10, desafio: misturar cores. Com o alarme calmo, o LED RGB mostra a cor
// que você escolher com os três números. Igual ao 8: só muda a função calmo().
// Precisa da biblioteca "DHT sensor library" (Adafruit).
#include <DHT.h>

const int LED = 9;
const int LDR = A1;
const int LIMITE = 900;   // mexa aqui: entre a luz com o laser e a luz sem ele
const int BOTAO = 2;
const int BUZZER = 8;
const int TOM = 1000;     // mexa aqui: som do alarme
const int VERMELHO = 10;
const int VERDE = 6;
const int AZUL = 5;
const int TEMP_MAXIMA = 28;   // mexa aqui: um pouco acima da temperatura da sala
const int TRIG = 12;
const int ECHO = 13;
const int DISTANCIA_MINIMA = 20;   // mexa aqui: em centímetros

DHT dht(7, DHT11);

bool ligado = false;   // o alarme começa desligado; o botão liga e desliga

// mexa aqui: cada número vai de 0 (apagado) a 255 (máximo)
const int QUANTO_VERMELHO = 150;
const int QUANTO_VERDE = 0;
const int QUANTO_AZUL = 255;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BOTAO, INPUT_PULLUP);
  pinMode(VERMELHO, OUTPUT);
  pinMode(VERDE, OUTPUT);
  pinMode(AZUL, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  verificarBotao();

  int luz = analogRead(LDR);
  float temperatura = dht.readTemperature();
  int distancia = medirDistancia();

  Serial.print(luz);
  Serial.print(" ");
  Serial.print(temperatura);
  Serial.print(" ");
  Serial.println(distancia);

  // disparou fica verdadeiro se algum sensor pegar
  bool disparou = false;
  if (luz < LIMITE) {
    disparou = true;
  }
  if (temperatura > TEMP_MAXIMA) {
    disparou = true;
  }
  if (distancia < DISTANCIA_MINIMA) {
    disparou = true;
  }

  if (ligado == false) {
    desligado();
  } else if (disparou) {
    alarme();
  } else {
    calmo();
  }

  delay(100);
}

// ---------- funções: cada uma faz um pedaço do trabalho ----------

// apertou o botão: inverte ligado/desligado e espera soltar
void verificarBotao() {
  if (digitalRead(BOTAO) == LOW) {
    ligado = !ligado;
    while (digitalRead(BOTAO) == LOW) {
    }
    delay(50);
  }
}

// manda o som e mede quanto o eco demorou; devolve a distância em centímetros
int medirDistancia() {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  int distancia = pulseIn(ECHO, HIGH, 30000) / 58;   // espera o eco no máximo 30 milésimos
  if (distancia == 0) {   // 0 quer dizer que o eco não voltou: não tem nada perto
    distancia = 999;
  }
  return distancia;
}

// algum sensor pegou
void alarme() {
  digitalWrite(LED, HIGH);
  tone(BUZZER, TOM);
  analogWrite(VERMELHO, 255);
  analogWrite(VERDE, 0);
  analogWrite(AZUL, 0);
}

// alarme ligado e tudo em ordem
void calmo() {
  digitalWrite(LED, LOW);
  noTone(BUZZER);
  analogWrite(VERMELHO, QUANTO_VERMELHO);
  analogWrite(VERDE, QUANTO_VERDE);
  analogWrite(AZUL, QUANTO_AZUL);
}

// alarme desligado pelo botão: tudo apagado e quieto
void desligado() {
  digitalWrite(LED, LOW);
  noTone(BUZZER);
  digitalWrite(VERMELHO, LOW);
  digitalWrite(VERDE, LOW);
  digitalWrite(AZUL, LOW);
}
