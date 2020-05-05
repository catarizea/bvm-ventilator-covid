#include <Wire.h>

const int I2CSlaveAddress = 4;

void setup() {
  Wire.begin(I2CSlaveAddress);
  Wire.onReceive(receiveEvent);
  
  Serial.begin(115200);
  
  Serial.println("");
  Serial.println("Starting");
}

void loop() {
  delay(100);
}

void receiveEvent(int howMany) {
  char t[30];
  int i = 0;
  
  while (Wire.available()) { 
    t[i] = Wire.read();
    i = i + 1;
  }
  
  int settings[4];
  int j = 0;

  String sett = "";
  for (int i = 0; i < howMany; i++) {
    String current = String(t[i]);
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

  Serial.println(settings[0]);
  Serial.println(settings[1]);
  Serial.println(settings[2]);
  Serial.println(settings[3]);
}
