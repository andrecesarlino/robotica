//## Os pinos | Um nome para cada pino. Se ligar uma peça em outro pino, mude só o número aqui.
// Quiz de botões: a pergunta aparece na tela e o aluno responde no botão A ou B.
// A tela decide se acertou e manda uma letra para acender o LED e tocar o buzzer.

const int BOTAO_A = 2;
const int BOTAO_B = 3;
const int BUZZER = 8;
const int LED_VERMELHO = 7;
const int LED_VERDE = 6;

//## Memória dos botões | Guarda se cada botão já estava apertado, para contar um aperto só uma vez.
bool aAntes = false;
bool bAntes = false;

//## setup | Roda uma vez: botões como entrada (INPUT_PULLUP), LEDs e buzzer como saída.
void setup()
{
  pinMode(BOTAO_A, INPUT_PULLUP);
  pinMode(BOTAO_B, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  Serial.begin(9600);
}

//## Botão A | No momento em que aperta, manda B1 para a tela.
void loop()
{
  bool a = digitalRead(BOTAO_A) == LOW;
  if (a && !aAntes) {
    Serial.println("B1");
  }
  aAntes = a;

//## Botão B | No momento em que aperta, manda B2 para a tela.
  bool b = digitalRead(BOTAO_B) == LOW;
  if (b && !bAntes) {
    Serial.println("B2");
  }
  bAntes = b;

//## Ouvir a tela | Lê as letras que a tela manda e passa para a função comando.
  while (Serial.available() > 0) {
    comando(Serial.read());
  }
  delay(20);
}

//## LEDs e buzzer | g acende o verde com som de acerto, r acende o vermelho com som de erro, t faz o tique.
void comando(char letra)
{
  if (letra == 'g') {            // acertou
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_VERMELHO, LOW);
    tone(BUZZER, 1000, 100);
    delay(120);
    tone(BUZZER, 1500, 150);
  }
  if (letra == 'r') {            // errou
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, HIGH);
    tone(BUZZER, 200, 400);
  }
  if (letra == 'o') {            // apaga os dois
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, LOW);
  }
  if (letra == 't') tone(BUZZER, 1500, 20);   // tique do relógio
}
