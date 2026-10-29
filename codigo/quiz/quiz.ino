// Quiz de botões: a pergunta aparece na tela e o aluno responde no botão A ou B.
// A tela decide se acertou e manda uma letra para acender o LED e tocar o buzzer.

const int BOTAO_A = 2;
const int BOTAO_B = 3;
const int BUZZER = 8;
const int LED_VERMELHO = 7;
const int LED_VERDE = 6;

bool aAntes = false;
bool bAntes = false;

void setup()
{
  pinMode(BOTAO_A, INPUT_PULLUP);
  pinMode(BOTAO_B, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  bool a = digitalRead(BOTAO_A) == LOW;
  if (a && !aAntes) {
    Serial.println("B1");
  }
  aAntes = a;

  bool b = digitalRead(BOTAO_B) == LOW;
  if (b && !bAntes) {
    Serial.println("B2");
  }
  bAntes = b;

  while (Serial.available() > 0) {
    comando(Serial.read());
  }
  delay(20);
}

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
