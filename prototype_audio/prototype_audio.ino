#include <JQ6500_Serial.h>

JQ6500_Serial mp3(Serial3);

void setup() {
  Serial.begin(9600); // Initialize Serial Monitor
  Serial3.begin(9600); // Initialize Serial2 for MP3 module
  mp3.reset(); // Reset the MP3 module
  mp3.setVolume(30); // Set volume (0-30)
  mp3.setLoopMode(MP3_LOOP_NONE); // Set loop mode (0: no loop, 1: loop)
}

void loop() {
  int i=0;
  i++;
  mp3.playFileByIndexNumber(i); // Play the first file in the directory
  Serial.println("test1");
  delay(5000);
}