int trig = 22;       // Trigger pin connected to digital pin 22
int echo = 23;       // Echo pin connected to digital pin 23
int duration;        // Variable to store the duration of the echo pulse
float distance;      // Distance in centimeters
float meter;         // Distance in meters

void setup() 
{ 
  Serial.begin(9600);            // Start serial communication at 9600 baud rate
  pinMode(trig, OUTPUT);         // Set the trigger pin as output
  digitalWrite(trig, LOW);       // Ensure trigger pin starts low
  delayMicroseconds(2);          // Short delay for stability
  pinMode(echo, INPUT);          // Set the echo pin as input
  delay(6000);                   // Initial delay (can be reduced or removed)
  Serial.println("Distance:");   // Print header text
}  

void loop() 
{   
  digitalWrite(trig, HIGH);      // Send a 10 microsecond pulse to trigger the sensor
  delayMicroseconds(10);  
  digitalWrite(trig, LOW);       // Set trigger pin low again

  duration = pulseIn(echo, HIGH); // Measure the length of the echo pulse

  if(duration >= 38000){          // If duration is too long (no object detected or too far)
      Serial.print("Out range");  // Print out of range
  }  
  else{ 
      distance = duration / 58.0;   // Convert duration to distance in cm
      Serial.print(distance);     
      Serial.print("cm");  

      meter = distance / 100.0;     // Convert cm to meters
      Serial.print("\t"); 
      Serial.print(meter);  
      Serial.println("m"); 
  }  
  delay(1000);                     // Wait one second before next measurement
}
