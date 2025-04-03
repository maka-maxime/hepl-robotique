#include <Servo.h>

#define SERVO_PIN 9
#define SERVO_REST 0
#define SERVO_POS1 30
#define SERVO_POS2 120

Servo servo;
uint8_t position; 

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  delay(500);
  servo.attach(SERVO_PIN);
  servo.write(SERVO_REST);
  delay(5000);
  digitalWrite(LED_BUILTIN,LOW);
}

void loop() {
  delay(4000);
  servo.write(SERVO_POS1);
  digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) ^ 0x01);
  delay(4000);
  servo.write(SERVO_POS2);
  digitalWrite(LED_BUILTIN, digitalRead(LED_BUILTIN) ^ 0x01);
}
