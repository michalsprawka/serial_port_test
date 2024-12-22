#include "arduino_secrets.h"
#include <Modulino.h>
#include "thingProperties.h"
#include <ArduinoGraphics.h>
#include <Arduino_LED_Matrix.h>
#include <Arduino_CloudConnectionFeedback.h>

// Create object instances
ModulinoThermo thermo;
ArduinoLEDMatrix matrix;
ModulinoPixels leds;

String readings[10] = {"0","0","0","0","0","0","0","0","0","0"};

const float CORRECTION =  -1.0;

void setup() {
  Serial.begin(9600);
  delay(1500);

  matrix.begin();

  //Set up Arduino Cloud
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  waitForArduinoCloudConnection(matrix);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  // Call all necessary .begin() functions
  Modulino.begin();
  thermo.begin();
  leds.begin();
}
//comment
void loop() {
  ArduinoCloud.update();

  // Assign environmental reading to variables. 'temperature' is a cloud variable.
  temperature = thermo.getTemperature() + CORRECTION;
  humidity = thermo.getHumidity();

  // Print the temperature and humidity values to Serial
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.print(" °C");

  Serial.print ("          ");

  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.print(" %");

  Serial.print ("          ");

  Serial.print("Min Temp:  ");
  Serial.print(minTemp);

  Serial.print ("          ");

  Serial.print("Max Temp:  ");
  Serial.println(maxTemp);

  // Convert numbers and assign them to strings
  String temperatureText = "Temp: " + String(temperature, 2) + "°C";
  String humidityText    = "Humid: " + String(humidity, 2) + "%";
  String temperatureOutText = "TOut: " + readings[0] + "°C";

  // Settings for the LED Matrix
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(50);

  // Add the text
  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println("    " + temperatureText + "    " + humidityText+ "    " + temperatureOutText);
  matrix.endText(SCROLL_LEFT);
  matrix.endDraw();

  //Dodałem
   if(Serial.available()) {
    String data = "";
    delay(30);
    while(Serial.available()) {
      data += char(Serial.read());
    }

   if(data == "ON") {
     lightSwitch = true;
   }
  else if(data == "OFF"){
    lightSwitch = false;
  }
  }


  // If 'lightswitch' is true, turn on the LEDs with a brightness of 5
  if (lightSwitch == true) {

    //Declare color variable
    ModulinoColor myColor = WHITE;

    // If the temperature is equal to or smaller than the minimum temperature turn all LEDs to blue
    if (temperature <= minTemp) {
      myColor = BLUE;
    // If the temperature is below the optimal but not at the minimum temperature, turn all LEDs to VIOLET
    } else if (temperature > minTemp && temperature < maxTemp) {
      myColor = GREEN;
    // If the temperature is within the optimal range, turn all LEDs to green
    } else if (temperature >= maxTemp) {
      myColor = RED;
    }

    for (int i = 0; i < 8; i++) {
      leds.set(i, myColor, 15);
    }
    leds.show();
  // If 'lightswitch' is false, set the brightness to 0 (turn them off)
  } else {
    for (int i = 0; i < 8; i++) {
      leds.set(i, WHITE, 0);
    }
    leds.show();
  }
}

//The functions below are called callback functions but we don't need them for this project.

void onMaxTempChange()  {}

void onMinTempChange()  {}

void onLightSwitchChange()  {}
/*
  Since Txtmessage is READ_WRITE variable, onTxtmessageChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onTxtmessageChange()  {
  Serial.println('#'+ txtmessage);
}
/*
  Since TempWardrobe2 is READ_WRITE variable, onTempWardrobe2Change() is
  executed every time a new value is received from IoT Cloud.
*/
void onTempWardrobe2Change()  {
  // Add your code here to act upon TempWardrobe2 change
}
/*
  Since TempSaloon is READ_WRITE variable, onTempSaloonChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onTempSaloonChange()  {
  // Add your code here to act upon TempSaloon change
}
/*
  Since XbeeStr is READ_WRITE variable, onXbeeStrChange() is
  executed every time a new value is received from IoT Cloud.
*/
void onXbeeStrChange()  {
  //message has structure like this: "23.5,48.4,1000.01,22,2,45.5,2022-03....,"
  //message - Temperature OUT, Humidity OUt, Pressure, Temperature attic, Hum attic, datetime,  tempWardrobe, HUM wardobe, source signature
  //coma is separator
  int next_coma = 0;
  int next_reading = 0;
  for(int i=0; i<xbee_str.length(); i++){
    if(xbee_str[i]  == ','){
      readings[next_reading] = xbee_str.substring(next_coma, i);
      next_coma = i+1;
      next_reading++;
    }
  }
  Serial.println('$'+ readings[0]);//temperature out
  Serial.println('&'+ readings[2]);//pressure
}