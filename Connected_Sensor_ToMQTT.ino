#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "SAMIR";
const char* password = "jcz204upq762";
const char* mqtt_server = "test.mosquitto.org";
const char* mqtt_topic_heart_rate = "sensor/heart_rate";
const char* mqtt_topic_blood_oxygen = "sensor/blood_oxygen";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastReconnectAttempt = 0;
const unsigned long reconnectInterval = 5000;  // Intentar reconectar cada 5 segundos

void setup() {
  Serial.begin(115200);
  setupWiFi();
  setupMQTT();
}

void loop() {
  checkAndMaintainConnection();
  publishSensorData();
  delay(1000);
}

void setupWiFi() {
  Serial.println("Conectando a Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("\nConectado a Wi-Fi");
}

void setupMQTT() {
  client.setServer(mqtt_server, 1883);
  Serial.println("Conectando a MQTT...");
  reconnectMQTT();
}

void checkAndMaintainConnection() {
  if (!client.connected()) {
    reconnectMQTT();
  } else {
    client.loop();
  }
}

void reconnectMQTT() {
  Serial.println("Intentando reconectar a MQTT...");
  if (millis() - lastReconnectAttempt > reconnectInterval) {
    lastReconnectAttempt = millis();
    connectMQTT();
  }
}

void connectMQTT() {
  if (client.connect("ESP32Client")) {
    Serial.println("Conectado al broker MQTT");
  } else {
    Serial.print("Error al conectar al broker MQTT, rc=");
    Serial.print(client.state());
    Serial.println(" Intentando nuevamente en 5 segundos...");
  }
}

void publishSensorData() {
  publishSensorValue(mqtt_topic_heart_rate, getHeartRate());
  publishSensorValue(mqtt_topic_blood_oxygen, getBloodOxygen());
}

void publishSensorValue(const char* topic, int value) {
  if (client.connected()) {
    client.publish(topic, String(value).c_str(), true);
    Serial.println("Publicado " + String(topic) + ": " + String(value));
    Serial.print(String(topic) + " - Estado de conexión MQTT: ");
    Serial.println(client.state());
  } else {
    Serial.println("Cliente MQTT no conectado.");
  }
}

int getHeartRate() {
  // Función simulada, reemplazar con la lógica real de obtención de la frecuencia cardíaca
  return 85;
}

int getBloodOxygen() {
  // Función simulada, reemplazar con la lógica real de obtención de la saturación de oxígeno en sangre
  return 90;
}
