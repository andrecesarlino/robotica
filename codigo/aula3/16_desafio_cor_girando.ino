// Desafio: cor no potenciômetro. Girando, o LED RGB vai do azul ao vermelho,
// passando pelo roxo no meio.
const int POT = A0;
const int VERMELHO = 10;
const int AZUL = 5;

void setup() {
  pinMode(VERMELHO, OUTPUT);
  pinMode(AZUL, OUTPUT);
}

void loop() {
  int giro = analogRead(POT);
  int quantoVermelho = giro / 4;   // igual ao brilho do arquivo 02
  analogWrite(VERMELHO, quantoVermelho);
  analogWrite(AZUL, 255 - quantoVermelho);   // o que falta de vermelho vira azul
  delay(20);
}
