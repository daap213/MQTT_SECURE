#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <PubSubClient.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// MQTT
const char* mqttBroker = "test.mosquitto.org";
const int mqttPort = 1883;
const char* baseTopic = "dap/test/";
const char* topicLCD = "dap/test/esp";

// Pines
int LED1 = 23, LED2 = 19, LED3 = 18, LED4 = 5;
int PULSADOR1 = 32, PULSADOR2 = 33, PULSADOR3 = 34, PULSADOR4_A = 35, PULSADOR4_B = 25;

// Estados
bool estadoLed1 = false, estadoLed2 = false, estadoLed3 = false, estadoLed4 = false;
bool estadoPulsador1 = false, estadoPulsador2 = false, estadoPulsador3 = false, estadoPulsador4A = false, estadoPulsador4B = false;

// WiFi y MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// === Funciones ===

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
  client.publish(topic.c_str(), mensaje.c_str());
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String mensaje = "";
  for (unsigned int i = 0; i < length; i++) {
    mensaje += (char)payload[i];
  }

  if (String(topic) == topicLCD) {
    mostrarEnLCD("Comd. externo:", mensaje);
  }
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
  while (!client.connected()) {
    Serial.print("Conectando a MQTT...");
    String clientId = "ESP32Client-" + String((uint32_t)ESP.getEfuseMac());
    if (client.connect(clientId.c_str())) {
      Serial.println("conectado");
      client.subscribe(topicLCD);
    } else {
      Serial.print("fallo, rc=");
      Serial.print(client.state());
      Serial.println(" intentando en 5 segundos");
      delay(5000);
    }
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
  client.setServer(mqttBroker, mqttPort);
  client.setCallback(mqttCallback);
}

void loop() {
  if (!client.connected()) {
    conectarMQTT();
  }
  client.loop();

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
