#define MOTOR_IN1 2  // Motor 1 control pin 1
#define MOTOR_IN2 3  // Motor 1 control pin 2
#define MOTOR_IN3 4  // Motor 2 control pin 1
#define MOTOR_IN4 5  // Motor 2 control pin 2

enum move {
  forward,
  backward,
  left,
  rigth,
  stop  
};

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);    // Initialize the built-in LED for indication
    Serial.begin(9600);              // Start serial communication at 9600 baud rate
    Serial.println("DRV8871 test");

    pinMode(MOTOR_IN1, OUTPUT);      // Set motor control pins as outputs
    pinMode(MOTOR_IN2, OUTPUT);
    pinMode(MOTOR_IN3, OUTPUT);
    pinMode(MOTOR_IN4, OUTPUT);
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);  // Turn LED on
    Serial.println("haut");            // Print "up" in French
    delay(1000);                      // Wait 1 second
    digitalWrite(LED_BUILTIN, LOW);   // Turn LED off
    Serial.println("bas");             // Print "down" in French
    delay(1000);

    // Test motor commands (uncomment to use)
    //Moteurs(forward,1000);
    //Moteurs(stop,10000);
    Moteurs(left, 1000);               // Turn left for 1 second
    Moteurs(stop, 10000);              // Stop motors for 10 seconds
}

// Function to control motors based on move direction and duration
void Moteurs(move sense, int time) {
  switch(sense) {
    case forward:
      // Ramp up motors forward speed gradually
      digitalWrite(MOTOR_IN1, LOW);
      digitalWrite(MOTOR_IN4, LOW);
      for (int i = 0; i < 255; i++) {
        analogWrite(MOTOR_IN2, i);
        analogWrite(MOTOR_IN3, i);
        delay(10);
      }
      break;

    case backward:
      // Ramp up motors backward speed gradually
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN3, LOW);
      for (int i = 0; i < 255; i++) {
        analogWrite(MOTOR_IN1, i);
        analogWrite(MOTOR_IN4, i);
        delay(10);
      }
      break;

    case left:
      // Turn left: one motor forward, other backward
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN4, LOW);
      for (int i = 0; i < 255; i++) {
        analogWrite(MOTOR_IN3, i);
        analogWrite(MOTOR_IN1, i);
        delay(10);
      }
      break;

    case rigth:
      // Turn right (only partially implemented)
      digitalWrite(MOTOR_IN1, LOW);
      for (int i = 0; i < 255; i++) {
        analogWrite(MOTOR_IN2, i);
        delay(10);
      }
      break;

    case stop:
      // Stop all motors
      digitalWrite(MOTOR_IN1, LOW);
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN3, LOW);
      digitalWrite(MOTOR_IN4, LOW);
      break;
  }
  delay(time);  // Wait for the specified time duration
}
