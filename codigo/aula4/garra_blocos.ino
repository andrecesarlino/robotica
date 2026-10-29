// C++ code
//
#include <Servo.h>

int angulo = 0;

Servo servo_9;

void setup()
{
  pinMode(A0, INPUT);
  servo_9.attach(9, 500, 2500);
  Serial.begin(9600);
}

void loop()
{
  angulo = map(analogRead(A0), 0, 1023, 150, 60);
  servo_9.write(angulo);
  Serial.println(angulo);
  delay(20); // Wait for 20 millisecond(s)
}
