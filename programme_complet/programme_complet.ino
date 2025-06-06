#include <QTRSensors.h>
#include <JQ6500_Serial.h>
#include <Servo.h>

// Servomoteurs
#define SERVO1_PIN  6
#define SERVO1_MIN  0

#define SERVO1_MAX 50
#define SERVO2_PIN  7
#define SERVO2_MIN  0
#define SERVO2_MAX 50

// Moteurs
#define MOTOR_IN1 2
#define MOTOR_IN2 3
#define MOTOR_IN3 4
#define MOTOR_IN4 5

// Capteurs ultrasons
#define ULTRASON_TRIG1 22
#define ULTRASON_ECHO1 23
#define ULTRASON_TRIG2 24
#define ULTRASON_ECHO2 25

// Audio
#define AUDIO_TX 18
#define AUDIO_RX 19
#define AUDIO_BAUD 9600
#define AUDIO_STOP_SOUND 1
#define AUDIO_VOLUME ((byte)30)

// BLE
#define BLE_TX 16
#define BLE_RX 17
#define LED_PIN 8

// Vitesse moteur
#define VITESSE_MOTEURS 57

// Objets
JQ6500_Serial audio(Serial3);
QTRSensors qtr;

const uint8_t SensorCount = 3;
uint16_t sensorValues[SensorCount];

enum move {
  forward,
  backward,
  left,
  right,
  stop,
  uturn
};

// BLE
uint8_t led_state;
uint32_t input;
uint8_t buffer[32] = {0};
size_t bytes_read;

uint8_t stopped = false;
volatile float distance1 = 100;
volatile float distance2 = 100;
volatile bool shouldMeasure = false; // Nouveau flag
Servo servo1;
Servo servo2;
uint8_t positionservo;

// Timer2: demande de mesure périodique
/*ISR(TIMER2_COMPA_vect) {
  shouldMeasure = true;
}

void setupTimer2() {
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;
  OCR2A = 255;  // Fréquence ~60Hz
  TCCR2A |= (1 << WGM21);
  TCCR2B |= (1 << CS22) | (1 << CS20); // Prescaler 128
  TIMSK2 |= (1 << OCIE2A);
  sei();
}*/

void setup() {
  Serial.begin(9600);

  // Servomoteurs
  servo1.attach(SERVO1_PIN);
  servo1.write(SERVO1_MIN);
  servo2.attach(SERVO2_PIN);
  servo2.write(SERVO2_MIN);

  // LED BLE
  pinMode(LED_PIN, OUTPUT);
  led_state = 0;
  Serial.println("Waiting for a connection...");

  // Ultrasons
  pinMode(ULTRASON_TRIG1, OUTPUT);
  digitalWrite(ULTRASON_TRIG1, LOW);
  pinMode(ULTRASON_ECHO1, INPUT);

  pinMode(ULTRASON_TRIG2, OUTPUT);
  digitalWrite(ULTRASON_TRIG2, LOW);
  pinMode(ULTRASON_ECHO2, INPUT);
  //setupTimer2();

  // Moteurs
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_IN3, OUTPUT);
  pinMode(MOTOR_IN4, OUTPUT);

  // Audio
  Serial3.begin(AUDIO_BAUD);
  audio.reset();
  //audio.setEqualizer(MP3_EQ_NORMAL);
  audio.setVolume(AUDIO_VOLUME);
  audio.setLoopMode(MP3_LOOP_NONE);

  // QTR
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2}, SensorCount);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("QTRCalibration Start...");
  for (uint16_t i = 0; i < 400; i++) {
    qtr.calibrate();
    delay(5);
  }
  Serial.println("QTRCalibration Fin");
  digitalWrite(LED_BUILTIN, LOW);

  
}

void loop() {
  // Lancer les mesures hors interruption
 
  distance1 = getDistance(ULTRASON_TRIG1, ULTRASON_ECHO1);
  distance2 = getDistance(ULTRASON_TRIG2, ULTRASON_ECHO2);
     // Si un obstacle est détecté
  if (distance1 < 7 || distance2 < 7) {
    Moteurs(stop, 0);
    if (!stopped) {
      audio.playFileByIndexNumber(AUDIO_STOP_SOUND);
      stopped = true;
      Serial.println("Obstacle détecté!!!!");
      /*servo1.write(SERVO_POS1);
      servo2.write(SERVO_POS1);
      delay(20);
      servo1.write(SERVO_POS2);
      servo2.write(SERVO_POS2);*/
      delay(2000);
    }
    return;
  }
  else {
      stopped=false;
  }
  

  // BLE communication
  if (Serial2.available() != 0) {
    bytes_read = Serial2.readBytes(buffer, 31);
    Serial.print("Received bytes: ");
    Serial.println(bytes_read);
    Serial.print("Data received: ");
    Serial.println((const char *)buffer);

    if (strstr((const char*)buffer, "LED ON")) {
      digitalWrite(LED_PIN, HIGH);
    } else if (strstr((const char*)buffer, "LED OFF")) {
      digitalWrite(LED_PIN, LOW);
    }
  }

  // Affichage des distances
  Serial.print("Distance1: "); Serial.print(distance1);
  Serial.print(" cm | Distance2: "); Serial.println(distance2);

  // Suivi de ligne (si pas d’obstacle devant)
  stopped = false;
  uint16_t position = qtr.readLineBlack(sensorValues);
  Serial.print("Position: ");
  Serial.println(position);
  Serial.print(sensorValues[0]);Serial.print(" "); Serial.print(sensorValues[1]); Serial.print(" "); Serial.print(sensorValues[2]);
  if (sensorValues[0] < 50 && sensorValues[1] < 50 && sensorValues[2] < 50) {
    Moteurs(stop, 0);
    Serial.println("Tous les capteurs sur blanc : arrêt");
    move_arm();
    Moteurs(uturn, 0);
    return;
  }
  if (position < 800) {
    Moteurs(left, 5);
    Serial.println("Tourne à gauche");
  } else if (position > 1200) {
    Moteurs(right, 5);
    Serial.println("Tourne à droite");
  } else {
    Moteurs(forward, 0);
    Serial.println("Tout droit");
  }

  delay(5);
}

// Fonctions utilitaires

float getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  uint32_t duration = pulseIn(echoPin, HIGH, 20000); // Timeout 20ms
  if (duration == 0) return 1000.0; // Pas de réponse
  return duration / 58.0;
}

void Moteurs(move sense, int time) {
  switch (sense) {
    case forward:
      digitalWrite(MOTOR_IN1, LOW);
      digitalWrite(MOTOR_IN4, LOW);
      analogWrite(MOTOR_IN2, VITESSE_MOTEURS);
      analogWrite(MOTOR_IN3, VITESSE_MOTEURS);
      break;
    case backward:
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN3, LOW);
      analogWrite(MOTOR_IN1, VITESSE_MOTEURS);
      analogWrite(MOTOR_IN4, VITESSE_MOTEURS);
      break;
    case right:
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN4, LOW);
      analogWrite(MOTOR_IN3, VITESSE_MOTEURS + 5);
      analogWrite(MOTOR_IN1, VITESSE_MOTEURS + 5);
      break;
    case left:
      digitalWrite(MOTOR_IN1, LOW);
      digitalWrite(MOTOR_IN3, LOW);
      analogWrite(MOTOR_IN2, VITESSE_MOTEURS + 5);
      analogWrite(MOTOR_IN4, VITESSE_MOTEURS + 5);
      break;
    case stop:
      digitalWrite(MOTOR_IN1, LOW);
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN3, LOW);
      digitalWrite(MOTOR_IN4, LOW);
      break;
    case uturn:
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN4, LOW);
      analogWrite(MOTOR_IN3, VITESSE_MOTEURS + 5);
      analogWrite(MOTOR_IN1, VITESSE_MOTEURS + 5);
      delay(6200);
      break;

  }

  if (time > 0) delay(time);
}

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