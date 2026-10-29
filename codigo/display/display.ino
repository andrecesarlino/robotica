// Display de 7 segmentos contando de 0 a 9, um número por segundo.
// Cada segmento é um LED; a tabela NUMEROS diz quais acendem em cada número.

const int SEGMENTOS[7] = {2, 3, 4, 5, 6, 7, 8};  // pinos dos segmentos a, b, c, d, e, f, g
const bool CATODO_COMUM = true;  // troque para false se o pino comum do seu display vai no 5V

// 1 = aceso, 0 = apagado, na ordem a, b, c, d, e, f, g
const int NUMEROS[10][7] = {
  {1, 1, 1, 1, 1, 1, 0},  // 0
  {0, 1, 1, 0, 0, 0, 0},  // 1
  {1, 1, 0, 1, 1, 0, 1},  // 2
  {1, 1, 1, 1, 0, 0, 1},  // 3
  {0, 1, 1, 0, 0, 1, 1},  // 4
  {1, 0, 1, 1, 0, 1, 1},  // 5
  {1, 0, 1, 1, 1, 1, 1},  // 6
  {1, 1, 1, 0, 0, 0, 0},  // 7
  {1, 1, 1, 1, 1, 1, 1},  // 8
  {1, 1, 1, 1, 0, 1, 1},  // 9
};

void setup()
{
  for (int i = 0; i < 7; i++) {
    pinMode(SEGMENTOS[i], OUTPUT);
  }
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

void mostrar(int numero)
{
  for (int i = 0; i < 7; i++) {
    int aceso = NUMEROS[numero][i];
    if (!CATODO_COMUM) {
      aceso = !aceso;  // no ânodo comum o segmento acende com LOW
    }
    digitalWrite(SEGMENTOS[i], aceso);
  }
}
