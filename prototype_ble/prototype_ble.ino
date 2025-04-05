#define BLE_TX 16
#define BLE_RX 17
#define LED_PIN 8

uint8_t led_state;
uint32_t input;
uint8_t buffer[32]={0};
size_t bytes_read;

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  led_state = 0;
  Serial.println("Waiting for a connection...");
}

void loop() {
  if (Serial2.available() != 0)
  {
    bytes_read = Serial2.readBytes(buffer, 31);
    Serial.print("Received bytes: ");
    Serial.println(bytes_read);
    Serial.print("Data received: ");
    Serial.println((const char *)buffer);
  }
}
