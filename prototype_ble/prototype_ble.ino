#define BLE_TX 16
#define BLE_RX 17
#define BLE_STATE 2
#define LED_PIN 8

uint8_t led_state;
uint32_t input;
uint8_t buffer[32]={0};
size_t bytes_read;

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  pinMode(BLE_STATE, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BLE_TX, OUTPUT);
  pinMode(BLE_RX, INPUT);
  led_state = 0;
}

void loop() {

  if (LOW == digitalRead(BLE_STATE))
  {
    Serial.println("Waiting for a connection...");
    while (LOW == digitalRead(BLE_STATE)) {
    }
    Serial.println("Device connected.");
  }

  if (Serial2.available() != 0)
  {
    bytes_read = Serial2.readBytes(buffer, 31);
    Serial.print("Received bytes: ");
    Serial.println(bytes_read);
    Serial.print("Data received: ");
    Serial.println((const char *)buffer);
    Serial2.println("ACK\n");
  }
}
