#include <JQ6500_Serial.h>

/// Connector P2 (UART)
/// SPK+  === Haut-parleur R
/// SPK-  === Haut-parleur N
/// ADC_L === NC
/// ADC_H === NC
/// VCC   === VCC
/// GND   === GND
/// TX    === AUDIO_RX
/// RX    === AUDIO_TX

#define AUDIO_TX 18
#define AUDIO_RX 19
#define AUDIO_BAUD 9600
#define AUDIO_VOLUME ((byte)30)

JQ6500_Serial jq6500(Serial1);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial1.begin(AUDIO_BAUD);
  jq6500.reset();
  jq6500.setEqualizer(MP3_EQ_NORMAL);
  jq6500.setVolume(AUDIO_VOLUME);
  jq6500.setLoopMode(MP3_LOOP_ONE_STOP);
  jq6500.playFileByIndexNumber(2);
  while (jq6500.getStatus() == MP3_STATUS_PLAYING)
    delay(100);
    delay(200);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {

}
