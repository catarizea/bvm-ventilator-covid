#include <Wire.h>
#include <AccelStepper.h>

AccelStepper stepper(1, 3, 2);

const int relayPin = 4;
const int trackerPin = 9;

const int I2CSlaveAddress = 4;

bool homeSet = false;
bool isHoming = true;

const int stepperParamsSize = 3;
int stepperParams[stepperParamsSize] = {0, 0, 0};

const int maxBVMVolume = 1600;
const int stepsPerRevolution = 500;
const int maximumSpeed = 3000;
const int defaultSpeed = 1000;

const int settingsSize = 4;
int settings[settingsSize] = {0, 0, 0, 0};
int prevSettings[settingsSize] = {0, 0, 0, 0};

bool isClockWise = true;
bool newStepperParams = false;

void setup() {
  pinMode(relayPin, OUTPUT);
  pinMode(trackerPin, INPUT);
  
  digitalWrite(relayPin, LOW);

  Wire.begin(I2CSlaveAddress);
  Wire.onReceive(receiveEvent);
  
  Serial.begin(115200);

  delay(3000);
  digitalWrite(relayPin, HIGH);
  delay(4000);

  stepper.setMaxSpeed(maximumSpeed);
  stepper.setSpeed(defaultSpeed);
  
  Serial.println("");
  Serial.println("Starting");
}

void loop() {
  if (homeSet == false) {
    executeHoming();
  }

  if (isHoming == false) {
    if (hasNewValues() == true) {
      Serial.println("hasNewValues");
      
      if (prevSettings[0] != 0) {
        newStepperParams = true;  
      }
      
      for (int i = 0; i < settingsSize; i++) {
        prevSettings[i] = settings[i];
      }

      computeNewParams();
    }

    if (nothingToDo() == true) {
      return;
    }

    if (newStepperParams == true) {
      Serial.println("newStepperParams move to zero");
      stepper.moveTo(0);
      stepper.setSpeed(defaultSpeed);
      stepper.runSpeedToPosition();

      printPositionalData();
      if (stepper.distanceToGo() != 0) {
        return;
      } else {
        Serial.println("Stepper is back at zero");
        newStepperParams = false;
        isClockWise = true;
      }
    }
    
    moveStepper();
  }
}

void printPositionalData() {
  Serial.print("Current position: ");
  Serial.println(stepper.currentPosition());
  Serial.print("Distance to go: ");
  Serial.println(stepper.distanceToGo());
}

void moveStepper() {
  if (isClockWise == true) {
    stepper.moveTo(stepperParams[0]);
    stepper.setSpeed(stepperParams[1]);

    printPositionalData();
    if (stepper.distanceToGo() == 0) {
      isClockWise = false;
    }
  } else {
    stepper.moveTo(0);
    stepper.setSpeed(stepperParams[2]);
    
    printPositionalData();
    if (stepper.distanceToGo() == 0) {
      isClockWise = true;
    }
  }

  stepper.runSpeedToPosition();
}

void receiveEvent(int howMany) {
  Serial.println("I2C Receive event");
  char t[30];
  int i = 0;
  
  while (Wire.available()) { 
    t[i] = Wire.read();
    i = i + 1;
  }
  
  int j = 0;

  if (checkForData(t, howMany, settingsSize) == false) return;

  String sett = "";
  for (int i = 0; i < howMany; i++) {
    String current = String(t[i]);

    // look for x as the values separator
    if (current != "x") {
      sett = sett + current;   
    } else {
      settings[j] = sett.toInt();
      sett = "";
      j++;
    }

    if (i == howMany - 1) {
      settings[j] = sett.toInt();
    }
  }
}

void executeHoming() {
  const int notAtHome = digitalRead(trackerPin);

  if (notAtHome == false) {
    Serial.println("Homing successful");
    stepper.setCurrentPosition(0); //now the current motor speed is zero
    homeSet = true;
    isHoming = false;
    return;
  }

  stepper.runSpeed();
}

bool hasNewValues() {
  bool newValues = false;
  
  for (int i = 0; i < settingsSize; i++) {
    if (newValues == false && settings[i] != prevSettings[i]) {
      newValues = true;
    }
  }

  return newValues;
}

bool checkForData(char *arr, int howMany, int size) {
  bool hasData = false;
  int xCount = 0;

  for (int i = 0; i < howMany; i++) {
    String current = String(arr[i]);
    
    // look for x as the values separator
    if (current == "x") {
      xCount++;
    }
  }

  if (xCount == size - 1) {
    hasData = true;
  }

  return hasData;
}

bool nothingToDo() {
  bool nothing = false;

  for (int i = 0; i < settingsSize; i++) {
    if (nothing == false && settings[i] == 0) {
      nothing = true;
    }
  }

  if (nothing == false) {
    for (int i = 0; i < stepperParamsSize; i++) {
      if (nothing == false && stepperParams[i] == 0) {
        nothing = true;
      }
    }
  }

  return nothing;
}

void computeNewParams() {
  Serial.println("computeNewParams");
  Serial.println("settings[0]: " + String(settings[0]));
  Serial.println("settings[1]: " + String(settings[1]));
  Serial.println("settings[2]: " + String(settings[2]));
  Serial.println("settings[3]: " + String(settings[3]));
  
  // Compute steps to move 
  // TODO measure precise volumes to determine 
  // the corelation between piston amplitude and pushed volume of air
  // the current formula assumes a linear corelation
  stepperParams[0] = ceil((float(stepsPerRevolution) / 2) * (float(settings[0]) / float(maxBVMVolume)));

  const float secondsPerFraction = 60 / ((float(settings[2]) + float(settings[3])) * float(settings[1]));
  
  // compute the steps per second for inspiration
  stepperParams[1] = ceil(float(stepperParams[0]) / (secondsPerFraction * float(settings[2])));

  // compute the steps per second for expiration
  stepperParams[2] = ceil(float(stepperParams[0]) / (secondsPerFraction * float(settings[3])));

  Serial.println("stepperParams[0]: " + String(stepperParams[0]));
  Serial.println("stepperParams[1]: " + String(stepperParams[1]));
  Serial.println("stepperParams[2]: " + String(stepperParams[2]));
}
