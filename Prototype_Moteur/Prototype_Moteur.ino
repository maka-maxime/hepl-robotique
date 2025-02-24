#define MOTOR_IN1 2
#define MOTOR_IN2 3
#define MOTOR_IN3 4
#define MOTOR_IN4 5
enum move{
  forward,
  backward,
  left,
  rigth,
  stop  
};
void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(9600); // Initialiser la communication série
    Serial.println("DRV8871 test");
  
    pinMode(MOTOR_IN1, OUTPUT);
    pinMode(MOTOR_IN2, OUTPUT);
    pinMode(MOTOR_IN3, OUTPUT);
    pinMode(MOTOR_IN4, OUTPUT);
}

void loop() {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("haut");
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("bas");
    delay(1000);


    //Moteurs(forward,1000);
    //Moteurs(stop,10000);
    Moteurs(left,1000);
    Moteurs(stop,10000);
      


}

void Moteurs(move sense,int time)
{
  switch(sense)
  {
    case forward:
      // ramp up forward
      digitalWrite(MOTOR_IN1, LOW);
      digitalWrite(MOTOR_IN4, LOW);
      for (int i=0; i<255; i++) {
        analogWrite(MOTOR_IN2, i);
        analogWrite(MOTOR_IN3, i);
        delay(10);
      }
      break;
    case backward:
        // ramp up backward
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN3,LOW);
      for (int i=0; i<255; i++) {
        analogWrite(MOTOR_IN1, i);
        analogWrite(MOTOR_IN4,i);
        delay(10);
      }
      break;
    case left:            // voir si on fait pas tourner les roues de gauche en arrière
      digitalWrite(MOTOR_IN2,LOW);
      digitalWrite(MOTOR_IN4, LOW);
      for (int i=0; i<255; i++) {        
        analogWrite(MOTOR_IN3, i);
        analogWrite(MOTOR_IN1,i);
        delay(10);
      }
      break;
    case rigth:
      digitalWrite(MOTOR_IN1, LOW);
      for (int i=0; i<255; i++) {
        analogWrite(MOTOR_IN2, i);
        delay(10);
      }
      break;
    case stop:
      digitalWrite(MOTOR_IN1, LOW);
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN3, LOW);
      digitalWrite(MOTOR_IN4, LOW);
      break;    
  }
  delay(time);

}