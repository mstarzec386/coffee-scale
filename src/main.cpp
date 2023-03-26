
#include <Wire.h>        // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Wire.h" // legacy: #include "SSD1306.h"
#include "HX711.h"
#include "Weight.h"

#include "bitmaps.h"

const int LOADCELL_DOUT_PIN = D6;
const int LOADCELL_SCK_PIN = D7;

SSD1306Wire display(0x3c, SDA, SCL); // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h
HX711 scale;
Weight weight;

long timeSinceLastUpdate = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);

  display.drawXbm(0, 0, 128, 64, Nina_heart_bitmap);

  display.display();
  delay(1000);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  delay(500);
  scale.set_scale(104.5f);
  scale.tare();
}

void loop()
{
  weight.update(scale.get_units(1));

  if (millis() - timeSinceLastUpdate > 100)
  {
    timeSinceLastUpdate = millis();

    float filteredWeight = weight.getWeight();
    float flow = weight.getFlow();

    display.clear();

    display.setFont(ArialMT_Plain_24);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(10, 10, String(filteredWeight, 1) + String(" g"));
    display.setFont(ArialMT_Plain_10);
    display.drawString(10, 50, String(flow, 1) + String("g/s"));

    unsigned int size = weight.getFlowHistorySize();
    float *history = weight.getFlowHistory();

    for (unsigned int i = 0; i < size; i++)
    {
      int round = (int)(history[i]*10);
      int mapped = map(round, 0, 200, 0,64);
      if (mapped > 60)
      {
        mapped = 60;
      }

      if (mapped < 0)
      {
        mapped = 0;
      }

      display.drawCircle(128 - size * 2 + i * 2, 60 - mapped, 2);
    }

    display.display();
  }
}
