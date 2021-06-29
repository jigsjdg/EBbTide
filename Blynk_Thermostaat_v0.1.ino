#include <SimpleTimer.h>           // Allows us to call functions without putting them in loop()

#define BLYNK_PRINT Serial         // Comment this out to disable prints and save space
#include <BlynkSimpleEsp8266.h> 

#include <OneWire.h>
#include <DallasTemperature.h> 
#define ONE_WIRE_BUS 2          // Your ESP8266 pin (ESP8266 GPIO 2 = WeMos D1 Mini pin D4)
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

char ssid[] = "***";
char pass[] = "***";
char auth[] = "***";

SimpleTimer timer;

float roomTemperature;            // Room temperature
float doelTemp;
WidgetLED led1(V3);

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  while (Blynk.connect() == false) {
    // Wait until connected
  }

  sensors.begin();                        // Starts the DS18B20 sensor(s).
  sensors.setResolution(9);              // More on resolution: http://www.homautomation.org/2015/11/17/ds18b20-how-to-change-resolution-9101112-bits/

  timer.setInterval(2000L, sendTemps);    // Temperature sensor read interval. 2000 (ms) = 2 seconds.
}

// Notice how there is very little in the loop()? Blynk works best that way.
void loop()
{
  Blynk.run();
  timer.run();
}

// Notice how there are no delays in the function below? Blynk works best that way.
void sendTemps()
{
  //zeker weten dat doelTemp nooit hoger is dan 25 en nooit lager dan 7
  if (doelTemp > 25) { doelTemp == 25; }
  if (doelTemp < 25) { doelTemp == 7; }
  
  sensors.requestTemperatures();                  // Polls the sensors.
  roomTemperature = sensors.getTempCByIndex(0);   // Stores temperature. Change to getTempCByIndex(0) for celcius.
  Blynk.virtualWrite(1, roomTemperature);         // Send temperature to Blynk app virtual pin 1.

  if (roomTemperature < doelTemp) {
    led1.on();
  } else {
    led1.off();
  }

  if (roomTemperature > 23) {
    Blynk.setProperty(V1, "color", "#ea3333");
  } else if (roomTemperature > 21) {
    Blynk.setProperty(V1, "color", "#ff8b3d");
  } else if (roomTemperature > 19) {
    Blynk.setProperty(V1, "color", "#ffc30b");
  } else if (roomTemperature > 17) {
    Blynk.setProperty(V1, "color", "#7db043");
  } else {
    Blynk.setProperty(V1, "color", "#26abff");
  }

}

BLYNK_WRITE(V2){  // This function gets called each time something changes on the widget
  doelTemp = param.asFloat();  // This gets the 'value' of the Widget as an integer
  Blynk.virtualWrite(4, doelTemp);
  
  if (doelTemp > 23) {
    Blynk.setProperty(V4, "color", "#ea3333");
  } else if (doelTemp > 21) {
    Blynk.setProperty(V4, "color", "#ff8b3d");
  } else if (doelTemp > 19) {
    Blynk.setProperty(V4, "color", "#ffc30b");
  } else if (doelTemp > 17) {
    Blynk.setProperty(V4, "color", "#7db043");
  } else if (doelTemp > 7) {
    Blynk.setProperty(V4, "color", "#26abff");
  } else {
    Blynk.setProperty(V4, "color", "#bebebe");
  }
}

BLYNK_WRITE(V5){
  int pinValueButton1 = param.asInt();
  if (pinValueButton1 == 1) {
    doelTemp = 7;
    Blynk.virtualWrite(4, doelTemp);
    Blynk.setProperty(V4, "color", "#bebebe");
    Blynk.virtualWrite(2, doelTemp);
  }
}

BLYNK_WRITE(V6){
  int pinValueButton2 = param.asInt();
  if (pinValueButton2 == 1) {
    doelTemp = 19;
    Blynk.virtualWrite(4, doelTemp);
    Blynk.setProperty(V4, "color", "#7db043");
    Blynk.virtualWrite(2, doelTemp);
  }
}
 
BLYNK_WRITE(V7){
  int pinValueButton3 = param.asInt();
  if (pinValueButton3 == 1) {
    doelTemp = 15;
    Blynk.virtualWrite(4, doelTemp);
    Blynk.setProperty(V4, "color", "#26abff");
    Blynk.virtualWrite(2, doelTemp);
  }
}
