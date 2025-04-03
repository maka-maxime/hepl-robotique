#define MOTOR_IN1 2
#define MOTOR_IN2 3
#define MOTOR_IN3 4
#define MOTOR_IN4 5
#define ULTRASON_TRIG1 21
#define ULTRASON_ECHO1 22
#define ULTRASON_TRIG2 23
#define ULTRASON_ECHO2 24

#define CAPTEUR_GAUCHE A0
#define CAPTEUR_CENTRE A1
#define CAPTEUR_DROITE A2

#define VITESSE_MOTEURS 50
#define TAILLE_HISTORIQUE 10

enum move {
  forward,
  backward,
  left,
  right,
  stop,
  uturn
  
};
int historiqueGauche[TAILLE_HISTORIQUE] = {0};
int historiqueCentre[TAILLE_HISTORIQUE] = {0};
int historiqueDroite[TAILLE_HISTORIQUE] = {0};
int indexHistorique = 0;

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

  pinMode(CAPTEUR_GAUCHE, INPUT);
  pinMode(CAPTEUR_CENTRE, INPUT);
  pinMode(CAPTEUR_DROITE, INPUT);
}

void loop() {
  float distance1 = getDistance(ULTRASON_TRIG1, ULTRASON_ECHO1);
  float distance2 = getDistance(ULTRASON_TRIG2, ULTRASON_ECHO2);
  Serial.print("Distance Capteur 1: ");
  Serial.print(distance1);
  Serial.print(" cm | Distance Capteur 2: ");// attention la distance 2 va dans les négatif jsp pq
  Serial.print(distance2);
  Serial.println(" cm");

  int valGauche = analogRead(CAPTEUR_GAUCHE);
  int valCentre = analogRead(CAPTEUR_CENTRE);
  int valDroite = analogRead(CAPTEUR_DROITE);
  Serial.print("Capteur Gauche: ");
  Serial.print(valGauche);
  Serial.print(" | Capteur Centre: ");
  Serial.print(valCentre);
  Serial.print(" | Capteur Droite: ");
  Serial.println(valDroite);

    // Mise à jour de l'historique
  historiqueGauche[indexHistorique] = valGauche;
  historiqueCentre[indexHistorique] = valCentre;
  historiqueDroite[indexHistorique] = valDroite;
  indexHistorique = (indexHistorique + 1) % TAILLE_HISTORIQUE;

  // Calcul des moyennes glissantes
  int moyGauche = moyenneGlissante(historiqueGauche);
  int moyCentre = moyenneGlissante(historiqueCentre);
  int moyDroite = moyenneGlissante(historiqueDroite);

  // Calcul de la médiane
  int seuil = mediane(moyGauche, moyCentre, moyDroite);
  Serial.print("Valeur seuil ");
  Serial.println(seuil);

  if (distance1 < 10 || (distance2 < 10 && distance2>=0)) {  // Si un obstacle est détecté par au moins un capteur
    Moteurs(stop, 0);
    Serial.println("Obstacle stop");
    // mettre le play du son 
    //penser a mettre un delay ou voir attendre valeur retour d'attente du son
  } else if (valCentre < seuil && valGauche > seuil && valDroite > seuil) {
    Moteurs(forward, 0);
    Serial.println("Tout droit");
  } else if ( valDroite< seuil) {
    Moteurs(left, 0);
    Serial.println("Tourne à gauche");
  } else if (valGauche < seuil) {
    Moteurs(right, 0);
    Serial.println("Tourne à droite");
  } else {
    Moteurs(stop, 0);
    Serial.println("Hors de la ligne !");
  }

  delay(15);
}
int moyenneGlissante(int *tableau) {
  int somme = 0;
  for (int i = 0; i < TAILLE_HISTORIQUE; i++) {
    somme += tableau[i];
  }
  return somme / TAILLE_HISTORIQUE;
}

int mediane(int a, int b, int c) {
  if ((a >= b && a <= c) || (a >= c && a <= b)) return a;
  if ((b >= a && b <= c) || (b >= c && b <= a)) return b;
  return c;
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
      analogWrite(MOTOR_IN2, VITESSE_MOTEURS);
      analogWrite(MOTOR_IN3, VITESSE_MOTEURS);
      break;
    case backward:
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN3, LOW);
      analogWrite(MOTOR_IN1, VITESSE_MOTEURS);
      analogWrite(MOTOR_IN4, VITESSE_MOTEURS);
      break;
    case left:
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN4, LOW);
      analogWrite(MOTOR_IN3, VITESSE_MOTEURS);
      analogWrite(MOTOR_IN1, VITESSE_MOTEURS);
      break;
    case right:
      digitalWrite(MOTOR_IN1, LOW);
      digitalWrite(MOTOR_IN3, LOW);
      analogWrite(MOTOR_IN2, VITESSE_MOTEURS);
      analogWrite(MOTOR_IN4, VITESSE_MOTEURS);
      break;
    case stop:
      digitalWrite(MOTOR_IN1, LOW);
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN3, LOW);
      digitalWrite(MOTOR_IN4, LOW);
      break;
    case uturn:
      digitalWrite(MOTOR_IN1, VITESSE_MOTEURS);
      digitalWrite(MOTOR_IN4, VITESSE_MOTEURS);
      analogWrite(MOTOR_IN2, VITESSE_MOTEURS);
      analogWrite(MOTOR_IN3, VITESSE_MOTEURS);
      delay(20);
      digitalWrite(MOTOR_IN1, LOW);
      digitalWrite(MOTOR_IN2, LOW);
      digitalWrite(MOTOR_IN3, LOW);
      digitalWrite(MOTOR_IN4, LOW);


  }
  if (time > 0) delay(time);
}
