#define MOTOR_IN1 2
#define MOTOR_IN2 3
#define MOTOR_IN3 4
#define MOTOR_IN4 5
#define ULTRASON_TRIG1 21
#define ULTRASON_ECHO1 22

enum move {
  forward,
  backward,
  left,
  right,
  stop
};

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(ULTRASON_TRIG1, OUTPUT);
  digitalWrite(ULTRASON_TRIG1, LOW);
  pinMode(ULTRASON_ECHO1, INPUT);

  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_IN3, OUTPUT);
  pinMode(MOTOR_IN4, OUTPUT);
}

void loop() {
  float distance = getDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < 10) { // Si un obstacle est trop proche
    Moteurs(stop, 0);
    Serial.println("Stop !!!");
  } else {
    Moteurs(forward, 0);
  }

  delay(500);
}

float getDistance() {
  digitalWrite(ULTRASON_TRIG1, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASON_TRIG1, LOW);
  int duration = pulseIn(ULTRASON_ECHO1, HIGH);
  return duration / 58.0;
}

void Moteurs(move sense, int time) {
  switch (sense) {
    case forward:
      digitalWrite(MOTOR_IN1, LOW);
      digitalWrite(MOTOR_IN4, LOW);
      analogWrite(MOTOR_IN2, 255);
      analogWrite(MOTOR_IN3, 255);
      break;
    case backward:
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN3, LOW);
      analogWrite(MOTOR_IN1, 255);
      analogWrite(MOTOR_IN4, 255);
      break;
    case left:
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN4, LOW);
      analogWrite(MOTOR_IN3, 255);
      analogWrite(MOTOR_IN1, 255);
      break;
    case right:
      digitalWrite(MOTOR_IN1, LOW);
      analogWrite(MOTOR_IN2, 255);
      break;
    case stop:
      digitalWrite(MOTOR_IN1, LOW);
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN3, LOW);
      digitalWrite(MOTOR_IN4, LOW);
      break;
  }
  if (time > 0) delay(time);
}
