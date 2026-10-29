// Aula 1 - Prática 1: LED piscando
// LED no pino 13, com resistor de 300 ohms até o GND

const int LED = 13;

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  digitalWrite(LED, HIGH);  // acende
  delay(1000);              // espera 1 segundo
  digitalWrite(LED, LOW);   // apaga
  delay(1000);
}
