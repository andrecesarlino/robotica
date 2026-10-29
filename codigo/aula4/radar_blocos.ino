// C++ code
//
#include <Servo.h>

int angulo = 0;

int distancia = 0;

Servo servo_9;

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

void setup()
{
  servo_9.attach(9, 500, 2500);
  Serial.begin(9600);
  pinMode(8, OUTPUT);
}

void loop()
{
  for (angulo = 15; angulo <= 165; angulo += 1) {
    servo_9.write(angulo);
    delay(30); // Wait for 30 millisecond(s)
    distancia = 0.01723 * readUltrasonicDistance(10, 11);
    Serial.print(angulo);
    Serial.print(",");
    Serial.println(distancia);
    if (distancia < 40) {
      tone(8, 523, 30); // play tone 60 (C5 = 523 Hz)
    }
  }
}
