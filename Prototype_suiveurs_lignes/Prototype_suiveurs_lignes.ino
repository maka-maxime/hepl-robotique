#define CAPTEUR_1 A0  // Definition of the analog pin for sensor 1
#define CAPTEUR_2 A1  // Definition of the analog pin for sensor 2
#define CAPTEUR_3 A2  // Definition of the analog pin for sensor 3

void setup() {
  Serial.begin(9600);  // Start serial communication at 9600 baud rate
}

void loop() {
  int valeur1 = analogRead(CAPTEUR_1);  // Read analog value from sensor 1
  int valeur2 = analogRead(CAPTEUR_2);  // Read analog value from sensor 2
  int valeur3 = analogRead(CAPTEUR_3);  // Read analog value from sensor 3

  // Print sensor values to the Serial Monitor
  Serial.print("Sensor 1: ");
  Serial.print(valeur1);
  Serial.print(" | Sensor 2: ");
  Serial.print(valeur2);
  Serial.print(" | Sensor 3: ");
  Serial.println(valeur3);

  delay(500);  // Wait 500 milliseconds before the next reading
}
