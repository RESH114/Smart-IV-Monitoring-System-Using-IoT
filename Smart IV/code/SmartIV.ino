#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
#include <SimpleDHT.h>
#include "HX711.h"
#include <EEPROM.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID "user"
#define WLAN_PASS "password"
// Adafruit IO
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "User"
#define AIO_KEY "KEY"
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish Temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature");
Adafruit_MQTT_Publish Humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity");
Adafruit_MQTT_Publish Weight = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Weight");

#define tarebutton A0
#define DOUT D6  // DATA PIN IN LOAD CELL
#define CLK D7   // CLK PIN IN LOAD CELL

HX711 scale;
Servo myservo;

int pinDHT11 = D5;
SimpleDHT11 dht11(pinDHT11);
byte humidity = 0;               //Stores humidity value
byte temperature = 0;            //Stores temperature value
float calibration_factor = 991;  // this calibration factor must be adjusted according to your load cell//991
float units;
int set = 0;
int alert = 0;
int menu = 0;
int buttonvalue = 0;
int s1 = 0;

LiquidCrystal_I2C lcd(0x27, 20, 4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

boolean B_1, B_2, B_3, B_4, B_1_old = 0;  // B_1-Set  B_2-Inc  B_3-Dec  B_4-Esc

void setup() {

  EEPROM.begin(512);
  Serial.begin(9600);
  lcd.init();  // initialize the lcd
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4, 0);
  //lcd.print("WELCOME");
  lcd.setCursor(2, 1);
  lcd.print("INITIALIZING.");
  Serial.print(F("Connecting to "));
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
  pinMode(tarebutton, INPUT);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(WiFi.localIP());
  scale.begin(DOUT, CLK);
  scale.set_scale(calibration_factor);
  scale.tare();
  myservo.attach(D4);
  myservo.write(0);
  connect();
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("INITIALIZING");
  lcd.setCursor(6, 1);
  lcd.print("DONE.....");
  delay(1000);
  set = EEPROM.read(0);
  alert = EEPROM.read(1);
}

void loop() {
  //Serial.println("BUTTON call IN");
  buttonvalue = analogRead(tarebutton);
  //Serial.print("BUTTON value");
  //Serial.println(buttonvalue);
  if (buttonvalue <= 300)  // SET
  {
    B_1 = 1;
  } else {
    B_1 = 0;
  }
  if (buttonvalue > 300 && buttonvalue < 650)  // Increament
  {
    B_2 = 1;
    Serial.println("BUTTON 2 IN");
  } else {
    B_2 = 0;
  }
  if (buttonvalue > 650 && buttonvalue < 780)  // Decreament
  {
    B_3 = 1;
  } else {
    B_3 = 0;
  }
  if (buttonvalue > 780 && buttonvalue < 900)  // Escape
  {
    B_4 = 1;
    Serial.println("BUTTON 4 IN");
  } else {
    B_4 = 0;
  }


  if (B_1 == HIGH && B_1 != B_1_old)  ////SET////
  {
    menu = menu + 1;
    Serial.print("IN");
  }
  B_1_old = B_1;
  if (menu > 0) {
    Serial.print("menu in");

    if ((B_4 == HIGH || menu > 2))  /////ESCAPE////
    {
      menu = 3;
      Serial.println("ESC");
    }
    switch (menu) {
      case 1:  // Set weight
        if (B_2 == HIGH) {
          set = set + 1;
          Serial.println("BUTTON 2");
        }
        if (B_3 == HIGH) {
          set = set - 1;
        }
        Serial.println("SET Weight");
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("SET Weight");
        lcd.setCursor(6, 1);
        lcd.print(set, 1);
        lcd.print("g");
        break;
      case 2:  // SUB UNDERLOAD AMP
        if (B_2 == HIGH) {
          alert = alert + 1;
        }
        if (B_3 == HIGH) {
          alert = alert - 1;
        }
        Serial.println("alert");
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("Alert Weight");
        lcd.setCursor(6, 1);
        lcd.print(alert, 1);
        lcd.print("g");
        break;
      case 3:  // ESC
        Serial.println("ESC");
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("End");
        EEPROM.put(0, set);
        EEPROM.put(1, alert);
        s1 = 0;
        menu = 0;
        delay(2000);
        break;
    }
  } else {
    if (!mqtt.ping(3)) {
      // reconnect to adafruit io
      if (!mqtt.connected())
        connect();
    }
    dht11.read(&temperature, &humidity, NULL);
    Serial.print("Reading");
    units = scale.get_units(), 5;
    if (units < 0) {
      units = 0.00;
    }
    if (!Temperature.publish(temperature)) { Serial.println(F("Failed")); }
    if (!Humidity.publish(humidity)) { Serial.println(F("Failed")); }
    if (!Weight.publish(units)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("Sent!"));
    }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("T-");
    lcd.print(temperature, 1);
    lcd.print("C");
    lcd.setCursor(8, 0);
    lcd.print("H-");
    lcd.print(humidity, 1);
    lcd.print("%");
    lcd.setCursor(0, 1);
    lcd.print("Weight-");
    lcd.print(units, 1);
    lcd.print("g");
    delay(5000);
    if ((analogRead(tarebutton) > 800) && (analogRead(tarebutton) < 900)) {
      scale.tare();
      Serial.println("Tare");
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Alert Weight");
      myservo.write(0);
      delay(1000);
    }
    if (units <= set) {
      myservo.write(180);
      Serial.println("LOW WEIGHT");
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("LOW WEIGHT");
      delay(1000);
    }
    if (units <= alert) {
      Serial.println("ALERT");
      lcd.clear();
      lcd.setCursor(6, 0);
      lcd.print("ALERT");
      delay(1000);
    }
  }
  delay(2000);
}

void connect() {
  Serial.print(F("Connecting to Adafruit IO... "));
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if (ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(10000);
  }
  Serial.println(F("Adafruit IO Connected!"));
}