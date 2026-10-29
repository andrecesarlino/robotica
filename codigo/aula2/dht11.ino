// DHT11: mostra temperatura e umidade no Monitor Serial e no Plotter Serial.
// Precisa da biblioteca "DHT sensor library" (Adafruit). Dados no pino 7.
#include <DHT.h>

DHT dht(7, DHT11);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  delay(2000); // o DHT11 só mede a cada 2 segundos
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  if (isnan(temperatura) || isnan(umidade)) {
    Serial.println("Erro ao ler o sensor");
    return;
  }
  Serial.print("Temperatura:");
  Serial.print(temperatura);
  Serial.print(",Umidade:");
  Serial.println(umidade);
}
