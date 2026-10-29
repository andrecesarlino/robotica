// 7: alarme de temperatura. Se a sala esquentar acima de TEMP_MAXIMA,
// o alarme dispara igual ao do laser.
// Para testar: segure o sensor na mão ou sopre nele.
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

DHT dht(7, DHT11);

bool ligado = false;   // o alarme começa desligado; o botão liga e desliga

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BOTAO, INPUT_PULLUP);
  pinMode(VERMELHO, OUTPUT);
  pinMode(VERDE, OUTPUT);
  pinMode(AZUL, OUTPUT);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  verificarBotao();

  int luz = analogRead(LDR);
  float temperatura = dht.readTemperature();

  Serial.print(luz);
  Serial.print(" ");
  Serial.println(temperatura);

  // disparou fica verdadeiro se algum sensor pegar
  bool disparou = false;
  if (luz < LIMITE) {
    disparou = true;
  }
  if (temperatura > TEMP_MAXIMA) {
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

// algum sensor pegou
void alarme() {
  digitalWrite(LED, HIGH);
  tone(BUZZER, TOM);
  digitalWrite(VERMELHO, HIGH);
  digitalWrite(VERDE, LOW);
}

// alarme ligado e tudo em ordem
void calmo() {
  digitalWrite(LED, LOW);
  noTone(BUZZER);
  digitalWrite(VERMELHO, LOW);
  digitalWrite(VERDE, HIGH);
}

// alarme desligado pelo botão: tudo apagado e quieto
void desligado() {
  digitalWrite(LED, LOW);
  noTone(BUZZER);
  digitalWrite(VERMELHO, LOW);
  digitalWrite(VERDE, LOW);
}
