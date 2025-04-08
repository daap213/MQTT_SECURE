#include <WiFi.h>
#include <WebSocketsClient.h>  // incluir antes de MQTTPubSubClient.h
#include <MQTTPubSubClient.h>

const char* ssid = "Wokwi-GUEST";  // Reemplaza con tus credenciales de WiFi
const char* pass = "";  // Reemplaza con tu contraseña de WiFi

const char* mqttBroker = "mqttest.daaptech.org";  // El dominio de tu broker MQTT
const int mqttPort = 443;  // El puerto MQTT sin TLS (ya que se usa WebSocket)

const char* mqttUser = "admin";  // Usuario para la conexión MQTT
const char* mqttPassword = "miclaveprueba";  // Clave para la conexión MQTT

WebSocketsClient webSocketClient;
MQTTPubSubClient mqtt;

void connect() {
  // Conectar a WiFi
  Serial.print("Conectando a WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" conectado!");

  // Conectar al WebSocket (broker MQTT sobre WebSocket)
  Serial.println("Conectando al WebSocket...");
  webSocketClient.disconnect();
  webSocketClient.beginSSL(mqttBroker, mqttPort,"/","","mqtt" );  // Se pasa el usuario y la clave
  webSocketClient.setReconnectInterval(2000);

  // Intentar conectar al broker MQTT
  Serial.print("Conectando al broker MQTT...");
  while (!mqtt.connect("ESP32Client", mqttUser, mqttPassword)) {
    Serial.print(".");
    delay(1000);
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi desconectado");
      return;
    }
    if (!webSocketClient.isConnected()) {
      Serial.println("WebSocket desconectado");
      return;
    }
  }
  Serial.println(" conectado!");
}

void setup() {
  Serial.begin(115200);

  // Iniciar WiFi
  WiFi.begin(ssid, pass);

  // Inicializar cliente MQTT
  mqtt.begin(webSocketClient);

  // Conectar a WiFi, WebSocket y al broker MQTT
  connect();

  // Función callback cuando llega un mensaje MQTT
  mqtt.subscribe([](const String& topic, const String& payload, const size_t size) {
    Serial.println("Mensaje recibido: " + topic + " - " + payload);
  });

  // Suscripción al tópico "/hello"
  mqtt.subscribe("/hello", [](const String& payload, const size_t size) {
    Serial.print("/hello ");
    Serial.println(payload);
  });
}

void loop() {
  mqtt.update();  // Actualizar el estado de MQTT

  // Reconectar si se ha desconectado
  if (!mqtt.isConnected()) {
    connect();
  }

  // Publicar un mensaje cada segundo
  static uint32_t prev_ms = millis();
  if (millis() > prev_ms + 1000) {
    prev_ms = millis();
    mqtt.publish("/hello", "world");
  }
}
