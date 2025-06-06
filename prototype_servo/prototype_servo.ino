#include <Servo.h>

<<<<<<< Updated upstream
#define SERVO_PIN 6
#define SERVO_REST 0
#define SERVO_POS1 30
#define SERVO_POS2 120
=======
#define SERVO1_PIN 10
#define SERVO1_MIN  0
#define SERVO1_MAX 50
#define SERVO2_PIN  9
#define SERVO2_MIN  0
#define SERVO2_MAX 50
>>>>>>> Stashed changes

Servo servo1;
Servo servo2;

void move_arm() {
  uint8_t angle1 = SERVO1_MIN;
  uint8_t angle2 = SERVO2_MIN;

  // Raise the arm
  while (angle1 < SERVO1_MAX)
  {
    servo1.write(angle1);
    angle1++;
    delay(20);
  }
  delay(1000);

  // Extend the arm
  while (angle2 < SERVO2_MAX) {
    servo2.write(angle2);
    angle2++;
    delay(20);
  }
  delay(2000);
  
  // Retract the arm
  while (angle2 > SERVO2_MIN) {
    servo2.write(angle2);
    angle2--;
    delay(20);
  }
  delay(1000);

  // Lower the arm
  while (angle1 > SERVO1_MIN)
  {
    servo1.write(angle1);
    angle1--;
    delay(20);
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  servo1.attach(SERVO1_PIN);
  servo1.write(SERVO1_MIN);

  servo2.attach(SERVO2_PIN);
  servo2.write(SERVO2_MIN);

  digitalWrite(LED_BUILTIN, HIGH);
  move_arm();
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
}
