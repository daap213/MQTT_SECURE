#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WebSocketsClient.h>  // include before MQTTPubSubClient.h
#include <MQTTPubSubClient.h>

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// MQTT over WebSockets
const char* mqttBroker = "mqttest.daaptech.org";
const int mqttPort = 443; // MQTT over WebSockets Secure (WSS)
const char* baseTopic = "dap/test/";
const char* topicLCD = "dap/test/esp";

// Pines
int LED1 = 23, LED2 = 19, LED3 = 18, LED4 = 5;
int PULSADOR1 = 32, PULSADOR2 = 33, PULSADOR3 = 34, PULSADOR4_A = 35, PULSADOR4_B = 25;

// Estados
bool estadoLed1 = false, estadoLed2 = false, estadoLed3 = false, estadoLed4 = false;
bool estadoPulsador1 = false, estadoPulsador2 = false, estadoPulsador3 = false, estadoPulsador4A = false, estadoPulsador4B = false;

// WebSocket y MQTT
WebSocketsClient client;
MQTTPubSubClient mqtt;

void mostrarEnLCD(String linea1, String linea2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(linea1);
  lcd.setCursor(0, 1);
  lcd.print(linea2);
}

void connect() {
connect_to_wifi:
    Serial.print("Conectando a WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println(" conectado!");

connect_to_host:
    Serial.println("Conectando al host...");
    client.disconnect();
    client.beginSSL(mqttBroker, mqttPort,"/","","mqtt");
    client.setReconnectInterval(2000);

    Serial.print("Conectando al broker MQTT...");
    while (!mqtt.connect("ESP32Client")) {
        Serial.print(".");
        delay(1000);
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("WiFi desconectado");
            goto connect_to_wifi;
        }
        if (!client.isConnected()) {
            Serial.println("WebSocket desconectado");
            goto connect_to_host;
        }
    }
    Serial.println(" conectado!");
}

void publicarEstado(String topic, String mensaje) {
    mqtt.publish(topic, mensaje);
    Serial.println("Mensaje publicado: " + topic + " - " + mensaje);
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

    // Conexión WiFi y MQTT
    WiFi.begin(ssid, password);
    mqtt.begin(client);
    connect();

    // Suscribirse a un tema
    mqtt.subscribe([](const String& topic, const String& payload, const size_t size) {
        Serial.println("Mensaje recibido: " + topic + " - " + payload);
    });

    mqtt.subscribe(topicLCD, [](const String& payload, const size_t size) {
        Serial.print("Mensaje en LCD: ");
        Serial.println(payload);
        mostrarEnLCD("Mensaje:", payload);
    });
}

void loop() {
    mqtt.update();

    if (!mqtt.isConnected()) {
        connect();
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
        publicarEstado(String(baseTopic) + "amarillo", estadoLed1 ? "ON" : "OFF");
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
        publicarEstado(String(baseTopic) + "azul", estadoLed2 ? "ON" : "OFF");
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
        publicarEstado(String(baseTopic) + "rojo", estadoLed3 ? "ON" : "OFF");
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
        publicarEstado(String(baseTopic) + "verde", estadoLed4 ? "ON" : "OFF");
        estadoPulsador4A = true;
        delay(200);
    } else if (lectura4A == HIGH && estadoPulsador4A) {
        estadoPulsador4A = false;
    }
}
