#define MOTOR_IN1 2
#define MOTOR_IN2 3
#define MOTOR_IN3 4
#define MOTOR_IN4 5
#define ULTRASON_TRIG1 21
#define ULTRASON_ECHO1 22
#define ULTRASON_TRIG2 23
#define ULTRASON_ECHO2 24

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

  pinMode(ULTRASON_TRIG2, OUTPUT);
  digitalWrite(ULTRASON_TRIG2, LOW);
  pinMode(ULTRASON_ECHO2, INPUT);

  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_IN3, OUTPUT);
  pinMode(MOTOR_IN4, OUTPUT);
}

void loop() {
  float distance1 = getDistance(ULTRASON_TRIG1, ULTRASON_ECHO1);
  float distance2 = getDistance(ULTRASON_TRIG2, ULTRASON_ECHO2);
  Serial.print("Distance Capteur 1: ");
  Serial.print(distance1);
  Serial.print(" cm | Distance Capteur 2: ");// attention la distance 2 va dans les négatif jsp pq
  Serial.print(distance2);
  Serial.println(" cm");

  if (distance1 < 10 || (distance2 < 10 && distance2>=0)) {  // Si un obstacle est détecté par au moins un capteur
    Moteurs(stop, 0);
    Serial.println("stop");
  } else {
    Moteurs(forward, 0);
  }

  delay(500);
}

float getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  int duration = pulseIn(echoPin, HIGH);
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
