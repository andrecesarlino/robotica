// Brilha, brilha, estrelinha. Só o buzzer, no pino 8.
int notas[] = {
  262, 262, 392, 392, 440, 440, 392,  // Dó Dó Sol Sol Lá Lá Sol
  349, 349, 330, 330, 294, 294, 262,  // Fá Fá Mi Mi Ré Ré Dó
  392, 392, 349, 349, 330, 330, 294,  // Sol Sol Fá Fá Mi Mi Ré
  392, 392, 349, 349, 330, 330, 294,  // Sol Sol Fá Fá Mi Mi Ré
  262, 262, 392, 392, 440, 440, 392,  // Dó Dó Sol Sol Lá Lá Sol
  349, 349, 330, 330, 294, 294, 262   // Fá Fá Mi Mi Ré Ré Dó
};

void setup() {}

void loop() {
  for (int i = 0; i < 42; i++) {
    int tempo = 400;
    if (i % 7 == 6) tempo = 800;  // fim da frase: nota longa
    tone(8, notas[i], tempo);
    delay(tempo + 100);
  }
  delay(2000);
}
