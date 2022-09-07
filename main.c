#include <Arduino.h>
#include <dummy.h>
#include <Wire.h> //for ESP8266 use bug free i2c driver https://github.com/enjoyneering/ESP8266-I2C-Driver
#include <LiquidCrystal_I2C.h>

int Gas_analog = 4;  // used for ESP32
int Gas_digital = 2; // used for ESP32

int pin_led = 13;
int pin_led_G = 12;

int ENA = 14;
int IN1 = 17; // pin GPIO27
int IN2 = 5;  // pin GPIO26

int ENB = 15;
int IN3 = 18; // pin GPIO25
int IN4 = 19; // pin GPIO33

#define COLUMS 20
#define ROWS 4

#define PAGE ((COLUMS) * (ROWS))

// LiquidCrystal_I2C lcd (0x27,lcdColumns,lcdRows);
LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);
void Estractor_ON()
{
  // Direccion motor A
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(ENA, 100); // Velocidad motor A

  delay(1000);
  // Direccion motor B
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  digitalWrite(ENB, 100); // Velocidad motor B
}

void Estractor_OFF()
{
  // Direccion motor A
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(ENA, 0); // Velocidad motor B

  delay(1000);
  // Direccion motor B
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(ENB, 0); // Velocidad motor B
}
void setup()
{
  Serial.begin(115200);
  pinMode(Gas_digital, INPUT);
  pinMode(ENA, OUTPUT);
  digitalWrite(ENA, 0); // Velocidad motor B
  pinMode(ENB, OUTPUT);
  digitalWrite(ENB, 0); // Velocidad motor B
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(pin_led, OUTPUT);
  pinMode(pin_led_G, OUTPUT);
  
  while (lcd.begin(COLUMS, ROWS) != 1) // colums - 20, rows - 4
  {
    Serial.println(F("PCF8574 is not connected or lcd pins declaration is wrong. Only pins numbers: 4,5,6,16,11,12,13,14 are legal."));
    delay(5000);
  }

  lcd.print(F("PCF8574 is OK...")); //(F()) saves string to flash & keeps dynamic memory free
  delay(2000);

  lcd.clear();
}

void loop()
{
  int gassensorDigital = digitalRead(Gas_digital);
  Serial.print("\t");
  Serial.print("Gas Class: ");
  Serial.print(gassensorDigital);
  Serial.print("\t");
  Serial.print("\t");
  delay(1000);

  digitalWrite(pin_led, LOW);
  digitalWrite(pin_led_G, LOW);

  //

  if (gassensorDigital == 0)
  {
    lcd.setCursor(0,0);
    Serial.println("Gas");
    lcd.setCursor(0,1);
    lcd.print(F("AIRE SUCIO"));
    digitalWrite(pin_led_G, LOW);
    digitalWrite(pin_led, HIGH);
    Estractor_ON();
  }
  else
  {
    Estractor_OFF();
    lcd.setCursor(0,0);
    Serial.println("No Gas");
    lcd.setCursor(0,1);
    lcd.print(F("AIRE LIMPIO"));
    digitalWrite(pin_led, LOW);
    digitalWrite(pin_led_G, HIGH);
  }
}