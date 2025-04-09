#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <ArduinoWebsockets.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// MQTT over WebSockets
const char* mqttBroker = "mqttest.daaptech.org";
const char* baseTopic = "dap/test/";
const char* topicLCD = "dap/test/esp";

using namespace websockets;
WebsocketsClient wsClient;

// Pines
int LED1 = 23, LED2 = 19, LED3 = 18, LED4 = 5;
int PULSADOR1 = 32, PULSADOR2 = 33, PULSADOR3 = 34, PULSADOR4_A = 35, PULSADOR4_B = 25;

// Estados
bool estadoLed1 = false, estadoLed2 = false, estadoLed3 = false, estadoLed4 = false;
bool estadoPulsador1 = false, estadoPulsador2 = false, estadoPulsador3 = false, estadoPulsador4A = false, estadoPulsador4B = false;

void mostrarEnLCD(String linea1, String linea2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(linea1);
  lcd.setCursor(0, 1);
  lcd.print(linea2);
}

void publicarEstado(String color, bool estado) {
  String topic = baseTopic + color;
  String mensaje = estado ? "ON" : "OFF";
  String payload = "{\"topic\":\"" + topic + "\",\"message\":\"" + mensaje + "\"}";
  wsClient.send(payload);
}

void conectarWiFi() {
  Serial.print("Conectando a WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado");
}

void conectarMQTT() {
  String url = String("wss://") + mqttBroker + "/"; // Ensure trailing slash
  Serial.print("Conectando a MQTT WebSocket: ");
  Serial.println(url);

  if (wsClient.connect(mqttBroker,443,"/")) {
    Serial.println("Conectado a MQTT WebSocket");
    // Subscribe to topics
    String subscribePayload = "{\"action\":\"subscribe\",\"topics\":[\"" + String(topicLCD) + "\",\"dap/test/ext/motor1\",\"dap/test/ext/motor2\",\"dap/test/ext/motor3\",\"dap/test/ext/motor4\",\"dap/test/ext/msg\",\"dap/test/ext/all\"]}";
    wsClient.send(subscribePayload);
    Serial.println("Suscripción a los temas enviada.");
  } else {
    Serial.println("Error al conectar a MQTT WebSocket");
  }
}

void verificarConexionMQTT() {
  if (!wsClient.available() ) {
    Serial.println("Conexión MQTT perdida. Reconectando...");
    conectarMQTT();
  }
}

void setup() {
  Serial.begin(115200);

  // Pines
  pinMode(LED1, OUTPUT); pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT); pinMode(LED4, OUTPUT);
  pinMode(PULSADOR1, INPUT_PULLUP); pinMode(PULSADOR2, INPUT_PULLUP);
  pinMode(PULSADOR3, INPUT_PULLUP); pinMode(PULSADOR4_A, INPUT_PULLUP);
  pinMode(PULSADOR4_B, INPUT_PULLUP);

  // LCD
  lcd.init(); lcd.backlight();
  mostrarEnLCD("Iniciando...", "");

  // WiFi y MQTT
  conectarWiFi();
  conectarMQTT();
}

void loop() {
  verificarConexionMQTT();

  if (wsClient.available()) {
    WebsocketsMessage message = wsClient.readBlocking();
    Serial.println("Mensaje recibido: " + message.data());
    // Aquí puedes procesar los mensajes recibidos
  }

  // Lectura de pulsadores
  bool lectura1 = digitalRead(PULSADOR1);
  bool lectura2 = digitalRead(PULSADOR2);
  bool lectura3 = digitalRead(PULSADOR3);
  bool lectura4A = digitalRead(PULSADOR4_A);
  bool lectura4B = digitalRead(PULSADOR4_B);

  // PULSADOR 1
  if (lectura1 == LOW && !estadoPulsador1) {
    estadoLed1 = !estadoLed1;
    digitalWrite(LED1, estadoLed1);
    mostrarEnLCD("MOTOR1: " + String(estadoLed1 ? "ON" : "OFF"), "LED: Amarillo");
    publicarEstado("amarillo", estadoLed1);
    estadoPulsador1 = true;
    delay(200);
  } else if (lectura1 == HIGH && estadoPulsador1) {
    estadoPulsador1 = false;
  }

  // PULSADOR 2
  if (lectura2 == LOW && !estadoPulsador2) {
    estadoLed2 = !estadoLed2;
    digitalWrite(LED2, estadoLed2);
    mostrarEnLCD("MOTOR2: " + String(estadoLed2 ? "ON" : "OFF"), "LED: Azul");
    publicarEstado("azul", estadoLed2);
    estadoPulsador2 = true;
    delay(200);
  } else if (lectura2 == HIGH && estadoPulsador2) {
    estadoPulsador2 = false;
  }

  // PULSADOR 3
  if (lectura3 == LOW && !estadoPulsador3) {
    estadoLed3 = !estadoLed3;
    digitalWrite(LED3, estadoLed3);
    mostrarEnLCD("MOTOR3: " + String(estadoLed3 ? "ON" : "OFF"), "LED: Rojo");
    publicarEstado("rojo", estadoLed3);
    estadoPulsador3 = true;
    delay(200);
  } else if (lectura3 == HIGH && estadoPulsador3) {
    estadoPulsador3 = false;
  }

  // PULSADOR 4A
  if (lectura4A == LOW && !estadoPulsador4A) {
    estadoLed4 = !estadoLed4;
    digitalWrite(LED4, estadoLed4);
    mostrarEnLCD("MOTOR4: " + String(estadoLed4 ? "ON" : "OFF"), "LED: Verde");
    publicarEstado("verde", estadoLed4);
    estadoPulsador4A = true;
    delay(200);
  } else if (lectura4A == HIGH && estadoPulsador4A) {
    estadoPulsador4A = false;
  }

  // PULSADOR 4B (igual al 4A)
  if (lectura4B == LOW && !estadoPulsador4B) {
    estadoLed4 = !estadoLed4;
    digitalWrite(LED4, estadoLed4);
    mostrarEnLCD("MOTOR4: " + String(estadoLed4 ? "ON" : "OFF"), "LED: Verde");
    publicarEstado("verde", estadoLed4);
    estadoPulsador4B = true;
    delay(200);
  } else if (lectura4B == HIGH && estadoPulsador4B) {
    estadoPulsador4B = false;
  }
}
