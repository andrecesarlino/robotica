// Display de 7 segmentos com o CI 4511 contando de 0 a 9, um número por segundo.
// O Arduino manda o número em binário por 4 fios (A, B, C, D) e o 4511
// acende os segmentos certos sozinho.

const int PINO_A = 2;  // bit que vale 1
const int PINO_B = 3;  // bit que vale 2
const int PINO_C = 4;  // bit que vale 4
const int PINO_D = 5;  // bit que vale 8

void setup()
{
  pinMode(PINO_A, OUTPUT);
  pinMode(PINO_B, OUTPUT);
  pinMode(PINO_C, OUTPUT);
  pinMode(PINO_D, OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  for (int numero = 0; numero <= 9; numero++) {
    mostrar(numero);
    Serial.println(numero);
    delay(1000);
  }
}

// Exemplo: 6 em binário é 0110, então D=0, C=1, B=1, A=0
void mostrar(int numero)
{
  digitalWrite(PINO_A, bitRead(numero, 0));
  digitalWrite(PINO_B, bitRead(numero, 1));
  digitalWrite(PINO_C, bitRead(numero, 2));
  digitalWrite(PINO_D, bitRead(numero, 3));
}
