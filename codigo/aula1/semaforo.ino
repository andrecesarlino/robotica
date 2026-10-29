// Aula 1 - Prática 2: Semáforo
// Cada LED com seu resistor de 300 ohms até o GND
// Verde 3 s, amarelo 1 s, vermelho 3 s, repete

const int VERMELHO = 12;
const int AMARELO = 11;
const int VERDE = 10;

void setup() {
  pinMode(VERMELHO, OUTPUT);
  pinMode(AMARELO, OUTPUT);
  pinMode(VERDE, OUTPUT);
}

void loop() {
  digitalWrite(VERMELHO, LOW); // apaga o anterior
  digitalWrite(VERDE, HIGH);
  delay(3000);

  digitalWrite(VERDE, LOW);
  digitalWrite(AMARELO, HIGH);
  delay(1000);

  digitalWrite(AMARELO, LOW);
  digitalWrite(VERMELHO, HIGH);
  delay(3000);
}
