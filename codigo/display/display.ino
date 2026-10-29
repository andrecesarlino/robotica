//## Os pinos | Os 7 segmentos (a até g) ficam numa lista, nos pinos 2 a 8.
// Display de 7 segmentos contando de 0 a 9, um número por segundo.
// Cada segmento é um LED; a tabela NUMEROS diz quais acendem em cada número.

const int SEGMENTOS[7] = {2, 3, 4, 5, 6, 7, 8};  // pinos dos segmentos a, b, c, d, e, f, g
const bool CATODO_COMUM = true;  // troque para false se o pino comum do seu display vai no 5V

// 1 = aceso, 0 = apagado, na ordem a, b, c, d, e, f, g
//## A tabela dos números | Uma linha por número: 1 acende o segmento, 0 apaga.
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

//## setup | Roda uma vez: os 7 pinos como saída.
void setup()
{
  for (int i = 0; i < 7; i++) {
    pinMode(SEGMENTOS[i], OUTPUT);
  }
  Serial.begin(9600);
}

//## Contar de 0 a 9 | Mostra cada número por 1 segundo e manda o número para a tela.
void loop()
{
  for (int numero = 0; numero <= 9; numero++) {
    mostrar(numero);
    Serial.println(numero);
    delay(1000);
  }
}

//## Mostrar um número | Lê a linha do número na tabela e acende ou apaga cada segmento.
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
