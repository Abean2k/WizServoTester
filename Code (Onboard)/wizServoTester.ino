#include <ESP32Servo.h>

// Servo PWM pins
const int SERVO_PINS[4] = {0, 1, 2, 3};

// Rotary Encoder pins (A, B, Button)
const int ENCODER_A[4] = {16, 18, 20, 22};
const int ENCODER_B[4] = {17, 19, 21, 23};  
const int ENCODER_BUTTON[4] = {4, 7, 10, 11};

// USB Power detection (ADC pin reading USB voltage divider)
const int USB_DETECT_PIN = ADC1_CHANNEL_4; 
const int USB_THRESHOLD = 1500;  // ADC threshold for USB detection (adjust when i get pcb)

// Servo control
Servo servos[4];
const int SERVO_MIN = 1000;    // 0° position (µs)    TODO: Change based on gobiulda servo... each one is different 
const int SERVO_MAX = 2000;    // 300° position (µs)  TODO: Change based on gobiulda servo... each one is different 
const int SERVO_CENTER = 1500; // Center for continuous rotation

// Servo state (did it this way to keep it organized)
struct ServoState {
  int currentAngle;           // Current position (0-300)
  int targetAngle;            // Target position
  bool isContinuous;          // true = continuous rotation, false = normal servo
  bool isMoving;              // Currently moving to target
  int continuousSpeed;        // Speed for continuous mode (-100 to 100)
  unsigned long lastUpdate;   // For smooth movement timing
};

ServoState servoState[4] = {
  {150, 150, false, false, 0, 0},
  {150, 150, false, false, 0, 0},
  {150, 150, false, false, 0, 0},
  {150, 150, false, false, 0, 0}
};

// Encoder State
struct EncoderState {
  int lastA;
  int lastB;
  int position;  // 0-255 for servo angle mapping
  unsigned long lastButtonPress;
};

EncoderState encoderState[4] = {
  {0, 0, 128, 0},
  {0, 0, 128, 0},
  {0, 0, 128, 0},
  {0, 0, 128, 0}
};

// Power State
bool isUSBPowered = false;
int lastActiveServo = -1;  // Track which servo is currently moving (USB mode)


void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("Servo Tester Initializing...");
  
  // Setup servos
  for (int i = 0; i < 4; i++) {
    servos[i].setPeriodHertz(50);  // Standard servo frequency
    servos[i].attach(SERVO_PINS[i], SERVO_MIN, SERVO_MAX);
    servos[i].write(150);  // Center all servos
  }
  
  // Setup encoder pins
  for (int i = 0; i < 4; i++) {
    pinMode(ENCODER_A[i], INPUT_PULLUP);
    pinMode(ENCODER_B[i], INPUT_PULLUP);
    pinMode(ENCODER_BUTTON[i], INPUT_PULLUP);
    encoderState[i].lastA = digitalRead(ENCODER_A[i]);
    encoderState[i].lastB = digitalRead(ENCODER_B[i]);
  }
  
  // Setup USB detection ADC
  analogReadResolution(12);  // 12-bit resolution (0-4095)
  
  Serial.println("Setup complete!");
}

void loop() {
  // Check USB power status
  updateUSBPowerStatus();
  
  // Read all encoders
  for (int i = 0; i < 4; i++) {
    updateEncoder(i);
    updateEncoderButton(i);
  }
  
  // Update servo positions based on power mode
  if (isUSBPowered) {
    updateServosUSBMode();
  } else {
    updateServosBatteryMode();
  }
  
  // Print status (will disable when done with testing)
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 500) {
    printStatus();
    lastPrint = millis();
  }
  
  delay(20);  // 50Hz update rate
}


void updateUSBPowerStatus() {
  int adcValue = analogRead(USB_DETECT_PIN);
  isUSBPowered = (adcValue > USB_THRESHOLD);
  
  // When switching modes, stop all servos
  static bool lastUSBState = false;
  if (isUSBPowered != lastUSBState) {
    Serial.print("Power mode changed to: ");
    Serial.println(isUSBPowered ? "USB" : "Battery");
    lastUSBState = isUSBPowered;
  }
}


void updateEncoder(int encoderNum) {
  int currentA = digitalRead(ENCODER_A[encoderNum]);
  int currentB = digitalRead(ENCODER_B[encoderNum]);
  
  // Detect rotation (quadrature encoding)
  if (currentA != encoderState[encoderNum].lastA) {
    if (currentA == currentB) {
      // Clockwise
      encoderState[encoderNum].position = constrain(
        encoderState[encoderNum].position + 5, 0, 255
      );
    } else {
      // Counter-clockwise
      encoderState[encoderNum].position = constrain(
        encoderState[encoderNum].position - 5, 0, 255
      );
    }
  }
  
  encoderState[encoderNum].lastA = currentA;
  encoderState[encoderNum].lastB = currentB;
  
  // Map encoder position (0-255) to servo angle (0-300)
  int newAngle = map(encoderState[encoderNum].position, 0, 255, 0, 300);
  
  // For normal servo mode, update target angle
  if (!servoState[encoderNum].isContinuous) {
    servoState[encoderNum].targetAngle = newAngle;
  } else {
    // For continuous mode, map to speed (-100 to 100)
    servoState[encoderNum].continuousSpeed = map(newAngle, 0, 180, -100, 100);
  }
}

void updateEncoderButton(int encoderNum) {
  if (digitalRead(ENCODER_BUTTON[encoderNum]) == LOW) {
    unsigned long now = millis();
    
    // Debounce: only register press if >200ms since last press
    if (now - encoderState[encoderNum].lastButtonPress > 200) {
      // Toggle mode
      servoState[encoderNum].isContinuous = !servoState[encoderNum].isContinuous;
      
      Serial.print("Servo ");
      Serial.print(encoderNum);
      Serial.print(" switched to ");
      Serial.println(servoState[encoderNum].isContinuous ? "Continuous" : "Normal");
      
      // Reset servo to neutral on mode switch
      if (servoState[encoderNum].isContinuous) {
        servoState[encoderNum].continuousSpeed = 0;
      } else {
        servoState[encoderNum].targetAngle = 150;
      }
      
      encoderState[encoderNum].lastButtonPress = now;
    }
  }
}

// SERVO CONTROL - USB MODE 
void updateServosUSBMode() {
  // USB mode: only 1 servo at a time
  // Priority: any continuous-mode servo running, else first normal-mode servo needing movement
  
  int servoToMove = -1;
  
  // Check if any continuous servo is running
  for (int i = 0; i < 4; i++) {
    if (servoState[i].isContinuous && servoState[i].continuousSpeed != 0) {
      servoToMove = i;
      break;
    }
  }
  
  // If no continuous servo, find first normal servo that needs movement
  if (servoToMove == -1) {
    for (int i = 0; i < 4; i++) {
      if (!servoState[i].isContinuous && servoState[i].currentAngle != servoState[i].targetAngle) {
        servoToMove = i;
        break;
      }
    }
  }
  
  // Move the selected servo
  for (int i = 0; i < 4; i++) {
    if (i == servoToMove) {
      moveServo(i);
    } else {
      // Stop other servos
      if (servoState[i].isContinuous) {
        servos[i].writeMicroseconds(SERVO_CENTER);  // Stop
      }
    }
  }
}

// ============= SERVO CONTROL - BATTERY MODE =============
void updateServosBatteryMode() {
  // Battery mode: all servos can move simultaneously
  for (int i = 0; i < 4; i++) {
    moveServo(i);
  }
}

// ============= MOVE INDIVIDUAL SERVO =============
void moveServo(int servoNum) {
  ServoState& servo = servoState[servoNum];
  
  if (servo.isContinuous) {
    // Continuous rotation mode
    if (servo.continuousSpeed == 0) {
      // Stop
      servos[servoNum].writeMicroseconds(SERVO_CENTER);
    } else if (servo.continuousSpeed > 0) {
      // Forward: speed 0-100 maps to 1500-2000 µs
      int microseconds = map(servo.continuousSpeed, 0, 100, SERVO_CENTER, SERVO_MAX);
      servos[servoNum].writeMicroseconds(microseconds);
    } else {
      // Reverse: speed -100 to 0 maps to 1000-1500 µs
      int microseconds = map(servo.continuousSpeed, -100, 0, SERVO_MIN, SERVO_CENTER);
      servos[servoNum].writeMicroseconds(microseconds);
    }
  } else {
    // Normal servo mode: smooth movement to target
    int direction = (servo.currentAngle < servo.targetAngle) ? 1 : -1;
    
    if (servo.currentAngle != servo.targetAngle) {
      servo.currentAngle += direction;
      servo.isMoving = true;
    } else {
      servo.isMoving = false;
    }
    
    // Map angle (0-300)
    int microseconds = map(servo.currentAngle, 0, 300, SERVO_MIN, SERVO_MAX);
    servos[servoNum].writeMicroseconds(microseconds);
  }
}

// printing thing (copied from rp2040 rocket thing code)
void printStatus() {
  Serial.print("Power: ");
  Serial.print(isUSBPowered ? "USB" : "Battery");
  Serial.print(" | Servos: ");
  
  for (int i = 0; i < 4; i++) {
    Serial.print("[");
    Serial.print(i);
    Serial.print(":");
    if (servoState[i].isContinuous) {
      Serial.print("C");
      Serial.print(servoState[i].continuousSpeed);
    } else {
      Serial.print("N");
      Serial.print(servoState[i].currentAngle);
    }
    Serial.print("]");
  }
  Serial.println();
}
