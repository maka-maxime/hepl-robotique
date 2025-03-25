#define CAPTEUR_1 A0  // Définition des broches
#define CAPTEUR_2 A1
#define CAPTEUR_3 A2

void setup() {
  Serial.begin(9600);  // Initialisation du moniteur série
}

void loop() {
  int valeur1 = analogRead(CAPTEUR_1);  // Lecture du capteur 1
  int valeur2 = analogRead(CAPTEUR_2);  // Lecture du capteur 2
  int valeur3 = analogRead(CAPTEUR_3);  // Lecture du capteur 3

  // Affichage des valeurs des capteurs
  Serial.print("Capteur 1: ");
  Serial.print(valeur1);
  Serial.print(" | Capteur 2: ");
  Serial.print(valeur2);
  Serial.print(" | Capteur 3: ");
  Serial.println(valeur3);

  delay(500);  // Pause pour éviter trop d'affichages
}
