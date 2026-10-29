const int LED = 9;
const int LDR = A1;
const int LIMITE = 900;
const int BOTAO = 2;
const int BUZZER = 8;
const int TOM = 1000; 
bool ligado = false; 

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BOTAO, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  verificarBotao();
  int luz = analogRead(LDR);
  Serial.println(luz);

  bool disparou = false;
  if (luz < LIMITE) {
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

void verificarBotao() {
  if (digitalRead(BOTAO) == LOW) {
    ligado = !ligado;
    while (digitalRead(BOTAO) == LOW) {
    }
    delay(50);
  }
}

void alarme() {
  digitalWrite(LED, HIGH);
  tone(BUZZER, TOM);
}

void calmo() {
  digitalWrite(LED, LOW);
  noTone(BUZZER);
}

void desligado() {
  digitalWrite(LED, LOW);
  noTone(BUZZER);
}
