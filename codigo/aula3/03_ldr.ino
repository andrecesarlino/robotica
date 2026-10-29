// 3: o LDR mede a luz. Se escurecer (luz menor que o LIMITE), o LED acende.
// O Monitor Serial mostra os dois números: potenciômetro e luz.
const int POT = A0;
const int LED = 9;
const int LDR = A1;
const int LIMITE = 650;   // mexa aqui: entre a luz da sala e a luz com a mão em cima

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int pot = analogRead(POT);
  int luz = analogRead(LDR);

  Serial.print(pot);
  Serial.print(" ");
  Serial.println(luz);

  if (luz < LIMITE) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }

  delay(100);
}
