// 6: LED RGB mostra o estado do alarme.
// Verde: tudo calmo. Vermelho: algum sensor pegou. Apagado: alarme desligado.

const int LED = 9;
const int LDR = A1;
const int LIMITE = 900;   // mexa aqui: entre a luz com o laser e a luz sem ele
const int BOTAO = 2;
const int BUZZER = 8;
const int TOM = 1000;     // mexa aqui: som do alarme
const int VERMELHO = 10;
const int VERDE = 6;
const int AZUL = 5;

bool ligado = false;   // o alarme começa desligado; o botão liga e desliga

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BOTAO, INPUT_PULLUP);
  pinMode(VERMELHO, OUTPUT);
  pinMode(VERDE, OUTPUT);
  pinMode(AZUL, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  verificarBotao();

  int luz = analogRead(LDR);

  Serial.println(luz);

  // disparou fica verdadeiro se algum sensor pegar
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
