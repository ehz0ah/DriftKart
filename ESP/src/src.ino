#define TX_PIN 17 // UART2 TX Pin
#define RX_PIN 16 // UART2 RX Pin

void setup() {
  // Begin Serial communication on UART2 with a baud rate of 9600
  Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
}

void loop() {
  // Send 'r', 'g', 'b' commands every 2 seconds for KL25Z to control LEDs
  Serial2.write('r');  // Send 'r' to turn on red LED
  delay(2000);         // Wait for 2 seconds

  Serial2.write('g');  // Send 'g' to turn on green LED
  delay(2000);         // Wait for 2 seconds

  Serial2.write('b');  // Send 'b' to turn on blue LED
  delay(2000);         // Wait for 2 seconds
}
