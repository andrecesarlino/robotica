// Aula 1 - Desafio de engenharia: Semáforo de pedestre
// Carro fica verde até alguém apertar o botão.
// Aí: amarelo 1 s, depois vermelho 4 s com o pedestre verde, e volta.
// Sem delay(): o botão é lido a qualquer momento com millis().
//
// Carro: vermelho 12, amarelo 11, verde 10
// Pedestre: vermelho 7, verde 6
// Chave momentânea: pino 2 e GND (INPUT_PULLUP, apertado = LOW)

const int CARRO_VERMELHO = 12;
const int CARRO_AMARELO = 11;
const int CARRO_VERDE = 10;
const int PEDESTRE_VERMELHO = 7;
const int PEDESTRE_VERDE = 6;
const int BOTAO = 2;

const unsigned long TEMPO_AMARELO = 1000;
const unsigned long TEMPO_PEDESTRE = 4000;

enum Estado { CARRO_VERDE_ESTADO, AMARELO_ESTADO, PEDESTRE_ESTADO };

Estado estado = CARRO_VERDE_ESTADO;
unsigned long inicio = 0; // quando o estado atual começou
bool pedido = false;      // alguém apertou o botão?

void acender(int carroVermelho, int carroAmarelo, int carroVerde,
             int pedVermelho, int pedVerde) {
  digitalWrite(CARRO_VERMELHO, carroVermelho);
  digitalWrite(CARRO_AMARELO, carroAmarelo);
  digitalWrite(CARRO_VERDE, carroVerde);
  digitalWrite(PEDESTRE_VERMELHO, pedVermelho);
  digitalWrite(PEDESTRE_VERDE, pedVerde);
}

void mudarPara(Estado novo) {
  estado = novo;
  inicio = millis();
}

void setup() {
  pinMode(CARRO_VERMELHO, OUTPUT);
  pinMode(CARRO_AMARELO, OUTPUT);
  pinMode(CARRO_VERDE, OUTPUT);
  pinMode(PEDESTRE_VERMELHO, OUTPUT);
  pinMode(PEDESTRE_VERDE, OUTPUT);
  pinMode(BOTAO, INPUT_PULLUP);
  mudarPara(CARRO_VERDE_ESTADO);
}

void loop() {
  // o botão é lido sempre
  if (digitalRead(BOTAO) == LOW) {
    pedido = true;
  }

  unsigned long passou = millis() - inicio;

  switch (estado) {
    case CARRO_VERDE_ESTADO:
      acender(LOW, LOW, HIGH, HIGH, LOW);
      if (pedido) {
        mudarPara(AMARELO_ESTADO);
      }
      break;

    case AMARELO_ESTADO:
      acender(LOW, HIGH, LOW, HIGH, LOW);
      if (passou >= TEMPO_AMARELO) {
        mudarPara(PEDESTRE_ESTADO);
      }
      break;

    case PEDESTRE_ESTADO:
      acender(HIGH, LOW, LOW, LOW, HIGH);
      if (passou >= TEMPO_PEDESTRE) {
        pedido = false; // apertos durante a travessia não contam
        mudarPara(CARRO_VERDE_ESTADO);
      }
      break;
  }
}
