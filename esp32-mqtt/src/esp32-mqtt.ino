#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
// Inicializa LCD 16x2 en dirección 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Configuracion wifi
const char* ssid     = "Wokwi-GUEST";
const char* password = "";

//Configuracion mqtt
const char* mqttBroker = "test.mosquitto.org";
const int mqttPort = 1883;
const char* baseTopic = "dap/test/";
const char* topicLCD = "dap/test/esp";

// Pines de LEDs
int LED1 = 23;
int LED2 = 19;
int LED3 = 18;
int LED4 = 5;

// Pines de pulsadores
int PULSADOR1 = 32;
int PULSADOR2 = 33;
int PULSADOR3 = 34;
int PULSADOR4_A = 35;
int PULSADOR4_B = 25;

// Estados de LEDs
bool estadoLed1 = false;
bool estadoLed2 = false;
bool estadoLed3 = false;
bool estadoLed4 = false;

// Estados de pulsadores
bool estadoPulsador1 = false;
bool estadoPulsador2 = false;
bool estadoPulsador3 = false;
bool estadoPulsador4A = false;
bool estadoPulsador4B = false;

void setup() {
  // Inicialización de LEDs y pulsadores
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  pinMode(PULSADOR1, INPUT_PULLUP);
  pinMode(PULSADOR2, INPUT_PULLUP);
  pinMode(PULSADOR3, INPUT_PULLUP);
  pinMode(PULSADOR4_A, INPUT_PULLUP);
  pinMode(PULSADOR4_B, INPUT_PULLUP);

  // Inicializar LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Sistema listo");

  Serial.begin(115200);
}

void loop() {
  // Lecturas de pulsadores
  bool lectura1 = digitalRead(PULSADOR1);
  bool lectura2 = digitalRead(PULSADOR2);
  bool lectura3 = digitalRead(PULSADOR3);
  bool lectura4A = digitalRead(PULSADOR4_A);
  bool lectura4B = digitalRead(PULSADOR4_B);

  // PULSADOR 1
  if (lectura1 == LOW && estadoPulsador1 == false) {
    estadoLed1 = !estadoLed1;
    digitalWrite(LED1, estadoLed1);
    mostrarEnLCD("LED 1", estadoLed1, "Amarillo");
    estadoPulsador1 = true;
    delay(200);
  }
  if (lectura1 == HIGH && estadoPulsador1 == true) {
    estadoPulsador1 = false;
  }

  // PULSADOR 2
  if (lectura2 == LOW && estadoPulsador2 == false) {
    estadoLed2 = !estadoLed2;
    digitalWrite(LED2, estadoLed2);
    mostrarEnLCD("LED 2", estadoLed2, "Azul");
    estadoPulsador2 = true;
    delay(200);
  }
  if (lectura2 == HIGH && estadoPulsador2 == true) {
    estadoPulsador2 = false;
  }

  // PULSADOR 3
  if (lectura3 == LOW && estadoPulsador3 == false) {
    estadoLed3 = !estadoLed3;
    digitalWrite(LED3, estadoLed3);
    mostrarEnLCD("LED 3", estadoLed3, "Rojo");
    estadoPulsador3 = true;
    delay(200);
  }
  if (lectura3 == HIGH && estadoPulsador3 == true) {
    estadoPulsador3 = false;
  }

  // PULSADOR 4A
  if (lectura4A == LOW && estadoPulsador4A == false) {
    estadoLed4 = !estadoLed4;
    digitalWrite(LED4, estadoLed4);
    mostrarEnLCD("LED 4", estadoLed4, "Verde");
    estadoPulsador4A = true;
    delay(200);
  }
  if (lectura4A == HIGH && estadoPulsador4A == true) {
    estadoPulsador4A = false;
  }

  // PULSADOR 4B
  if (lectura4B == LOW && estadoPulsador4B == false) {
    estadoLed4 = !estadoLed4;
    digitalWrite(LED4, estadoLed4);
    mostrarEnLCD("LED 4", estadoLed4, "Verde");
    estadoPulsador4B = true;
    delay(200);
  }
  if (lectura4B == HIGH && estadoPulsador4B == true) {
    estadoPulsador4B = false;
  }
}

// Función para mostrar en LCD
void mostrarEnLCD(String led, bool estado, String color) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(led + ": " + (estado ? "ON" : "OFF"));
  lcd.setCursor(0, 1);
  lcd.print("Color: " + color);
}
