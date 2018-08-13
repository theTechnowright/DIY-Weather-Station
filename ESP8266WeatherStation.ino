/*
 * theTechnowright 
 * Complete Project Details www.youtube.com/thetechnowright
 */
 
// Include the correct display library
// For a connection via I2C using Wire include
#include <Wire.h> 
#include "SSD1306.h"
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "XXXX";
char pass[] = "XXXXXX";

#include <DHT.h>

#define DHTPIN 4     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11

// Initialize the OLED display i2C
// D3 -> SDA
// D5 -> SCL

// Initialize the OLED display using Wire library
SSD1306  display(0x3C, D3, D5);

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
void setup(){
  Serial.begin(115200);
  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  dht.begin(); // initialize dht
 
  Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 8080);
  timer.setInterval(1000L, sendSensor);
}

void displayWeather(){ //Creating a function to read and display temperature and humidity on OLED display
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)){
    display.clear(); // clearing the display
    display.drawString(5,0, "DHT Failed!");
    return;
  }
  display.clear();
  display.drawString(30, 0, "Weather");
  display.drawString(0, 20, "Humidity: " + String(h) + "%\t"); 
  display.drawString(0, 40, "Temp: " + String(t) + "°C"); 
  //Uncomment to get temperature in farenheit
  //display.drawString(0, 40, "Temp: " + String(f) + "°F"); 

}
void loop(){
  Blynk.run(); // Running the blynk code
  displayWeather(); //Calling back the displayWeather function
  display.display();
  timer.run();
}

void sendSensor()
{
  float h = dht.readHumidity();  //Read the humidity
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}

