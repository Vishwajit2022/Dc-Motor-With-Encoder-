// Pin definitions
#define ENCODER_PIN_A 26   // Optical Encoder A pin
#define ENCODER_PIN_B 27   // Optical Encoder B pin
/*Pin description 
orange wire =VIN(power 5v)
Brawn wire =GND
red Wire =26
Yellow wire =27
*/
// Variables to track the encoder state
volatile long encoderCount = 0; // Tracks the pulse count
volatile bool direction = true; // true = forward, false = reverse

// Variables for RPM calculation
#define ENCODER_RESOLUTION 360 // Pulses per revolution
unsigned long lastTime = 0;    // Time for RPM calculation
unsigned long interval = 1000; // Interval in milliseconds
long lastEncoderCount = 0;     // Encoder count from the last interval

void setup() {
  // Configure encoder pins
  pinMode(ENCODER_PIN_A, INPUT_PULLUP);
  pinMode(ENCODER_PIN_B, INPUT_PULLUP);

  // Attach interrupt for the encoder
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), encoderISR, CHANGE);

  // Start Serial Monitor
  Serial.begin(115200);
}

void loop() {
  // Get the current time
  unsigned long currentTime = millis();

  // Print encoder data every second
  if (currentTime - lastTime >= interval) {
    lastTime = currentTime;

    // Calculate RPM
    long countDifference = encoderCount - lastEncoderCount;
    float rpm = (countDifference * 60.0) / ENCODER_RESOLUTION;

    // Determine if the motor is steady
    bool isSteady = (countDifference == 0);

    // Print to Serial Monitor
    Serial.print("Encoder Count: ");
    Serial.print(encoderCount);
    Serial.print(" | Direction: ");
    Serial.print(direction ? "Forward" : "Reverse");
    Serial.print(" | RPM: ");
    Serial.print(rpm);
    if (isSteady) {
      Serial.println(" | Motor is steady (not rotating)");
    } else {
      Serial.println(" | Motor is rotating");
    }

    // Update last encoder count
    lastEncoderCount = encoderCount;
  }
}

// Interrupt Service Routine for encoder
void encoderISR() {
  // Read the current state of encoder pins
  int a = digitalRead(ENCODER_PIN_A);
  int b = digitalRead(ENCODER_PIN_B);

  // Determine direction based on the state of A and B
  if (a == HIGH) {
    direction = (b == LOW); // Forward if B is LOW
  } else {
    direction = (b == HIGH); // Reverse if B is HIGH
  }

  // Update encoder count
  encoderCount += (direction ? 1 : -1);
}
