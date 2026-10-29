// Desafio: termômetro colorido. O LED RGB mostra a temperatura:
// azul = frio, verde = normal, vermelho = quente.
// Para testar: segure o sensor na mão ou sopre nele.
// Precisa da biblioteca "DHT sensor library" (Adafruit).
#include <DHT.h>

const int VERMELHO = 10;
const int VERDE = 6;
const int AZUL = 5;
const int FRIO = 24;     // mexa aqui: abaixo disso fica azul
const int QUENTE = 28;   // mexa aqui: acima disso fica vermelho

DHT dht(7, DHT11);

void setup() {
  pinMode(VERMELHO, OUTPUT);
  pinMode(VERDE, OUTPUT);
  pinMode(AZUL, OUTPUT);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float temperatura = dht.readTemperature();
  Serial.println(temperatura);

  digitalWrite(VERMELHO, LOW);
  digitalWrite(VERDE, LOW);
  digitalWrite(AZUL, LOW);

  if (temperatura < FRIO) {
    digitalWrite(AZUL, HIGH);
  } else if (temperatura > QUENTE) {
    digitalWrite(VERMELHO, HIGH);
  } else {
    digitalWrite(VERDE, HIGH);
  }

  delay(1000);
}
