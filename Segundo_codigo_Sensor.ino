#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

PulseOximeter pox;

bool beatDetected = false;

void onBeatDetected()
{
    beatDetected = true;
}

void setup()
{
    Wire.begin();  // Inicializa la comunicación I2C
    Wire.setClock(400000);  // Establece la velocidad de I2C a 400 kHz (Fast Mode)

    Serial.begin(115200);

    if (!pox.begin())
    {
        Serial.println("No se pudo detectar un sensor MAX30100. Conecta el sensor correctamente.");
        while (1);
    }

    pox.setOnBeatDetectedCallback(onBeatDetected);

    Serial.println("Sensor MAX30100 iniciado correctamente.");
}

void loop()
{
    pox.update();

    if (beatDetected)
    {
        Serial.println("♥ Beat!");
        beatDetected = false;
    }

    Serial.print("Heart rate: ");
    Serial.print(pox.getHeartRate());
    Serial.print(" bpm / SpO2: ");
    Serial.print(pox.getSpO2());
    Serial.println("%");
}