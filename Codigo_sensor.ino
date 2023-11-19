#include <Wire.h>
#include "MAX30105.h"
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

MAX30105 Sensor;

int c_ = 0;
int y;
void setup(){
  Serial.begin(115200);
  SerialBT.begin("ESP32_FRECUENCIA_CARDIACA_SPO2"); // Nombre del dispositivo Bluetooth
  Serial.println("Initializing...");

  // Initialize sensor
  if (!Sensor.begin(Wire, I2C_SPEED_FAST)){ //Use default I2C port, 400kHz speed STANDART 100KHZ
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }

  //Setup to sense a nice looking saw tooth on the plotter
  byte ledBrightness = 0x1F; //Options: 0=Off to 255=50mA
  byte sampleAverage = 1; //Options: 1, 2, 4, 8, 16, 32
  byte ledMode = 1; //Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
  int sampleRate = 3200; //Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
  int pulseWidth = 69; //Options: 69, 118, 215, 411
  int adcRange = 16384; //Options: 2048, 4096, 8192, 16384

  Sensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings

  //Arduino plotter auto-scales annoyingly. To get around this, pre-populate
  //the plotter with 500 of an average reading from the sensor

  //Take an average of IR readings at power up
  const byte avgAmount = 64;
  long baseValue = 0;

  for (byte x = 0 ; x < avgAmount ; x++){
    baseValue += Sensor.getIR(); //Read the IR value
  }
  baseValue /= avgAmount;

  //Pre-populate the plotter so that the Y scale is close to IR values
  for (int x = 0 ; x < 500 ; x++){
    Serial.println(baseValue);
  }
}

void loop(){
  if (c_>32000){
    c_ = 0;
  }

  y = Sensor.getIR();
  Serial.print(c_); //Send raw data to plotter
  Serial.print(";");
  Serial.println(y);
  SerialBT.print(c_);
  SerialBT.print(";H");
  SerialBT.println(y); // Envía el voltaje por Bluetooth
  c_ += 1;
  delay(1);
}
