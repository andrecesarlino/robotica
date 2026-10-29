// Desafio: luz noturna. Quanto mais escuro no LDR, mais forte o LED.
// Para testar: tape o LDR com a mão (o laser bate nele e deixa claro).
const int LDR = A1;
const int LED = 9;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int luz = analogRead(LDR);
  int brilho = 255 - luz / 4;   // invertido: pouca luz, muito brilho
  analogWrite(LED, brilho);
  Serial.println(luz);
  delay(100);
}
