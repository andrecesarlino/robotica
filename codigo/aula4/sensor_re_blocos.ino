// C++ code
//
#include <Servo.h>

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
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  servo_9.write(90);
}

void loop()
{
  distancia = 0.01723 * readUltrasonicDistance(10, 11);
  Serial.println(distancia);
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
  if (distancia < 10) {
    digitalWrite(7, HIGH);
    tone(8, 1661, 200); // play tone 80 (G#6 = 1661 Hz)
  } else {
    if (distancia < 30) {
      digitalWrite(6, HIGH);
      tone(8, 523, 50); // play tone 60 (C5 = 523 Hz)
    } else {
      digitalWrite(5, HIGH);
    }
  }
  delay(200); // Wait for 200 millisecond(s)
}
