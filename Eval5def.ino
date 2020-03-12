// Due to a problem with the OLED library and the sensors libraries, the OLED has been disabled.
#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Wire.h>
#include <BH1750FVI.h>
#include <Adafruit_BMP280.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


BH1750FVI LightSensor;

Adafruit_BMP280 bmp; // I2C

char auth[] = "qY6OonCOZI69rD0-dcJPC9xNgOcB3QEB";
char ssid[] = "WiFi-2.4-FE3E";
char pass[] = "D9A4F32A84";

BlynkTimer timer;

void myTimerEvent()
{
   // You can send any value at any time.
   // Please don't send more that 10 values per second.
   uint16_t lum = LightSensor.GetLightIntensity();// Get Lux value
   Blynk.virtualWrite(V2, bmp.readTemperature());
   Blynk.virtualWrite(V4, lum);
   
   Serial.print("Lumen: ");
   Serial.print(lum);
   Serial.println(" lux");
   Serial.print(F("Temperature = "));
   Serial.print(bmp.readTemperature());
   Serial.println(" *C");
   delay(1000);
}

// put your setup code here, to run once:
void setup()
{
   Serial.begin(9600); // Serial monitor.
   
   // Temperatuur.
   if (!bmp.begin())
   {
      Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
      while(1);
   }

   /* Default settings from datasheet. */
   bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                   Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                   Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                   Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                   Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

   // Lichtsensor.
   LightSensor.begin();
   LightSensor.SetAddress(Device_Address_L);//Address 0x5C
   LightSensor.SetMode(Continuous_H_resolution_Mode);

   // Blynk
   Blynk.begin(auth, ssid, pass);
   timer.setInterval(1000L, myTimerEvent);
}


void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
