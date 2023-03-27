
#include <Wire.h>
#include "SSD1306Wire.h"
#include "HX711.h"
#include "Weight.h"

#include "bitmaps.h"

const int LOADCELL_DOUT_PIN = D6;
const int LOADCELL_SCK_PIN = D7;

SSD1306Wire display(0x3c, SDA, SCL);
HX711 scale;
Weight weight;

long timeSinceLastUpdate = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

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

  // TODO move to UI class
  if (millis() - timeSinceLastUpdate > 50)
  {
    timeSinceLastUpdate = millis();

    float rawWeight = weight.getRawWeight();
    float filteredWeight = weight.getWeight();
    float flow = weight.getFlow();

    display.clear();

    // TODO add timer
    // TODO move to draw weight
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(1, 1, String(rawWeight, 2) + String(" g"));
    display.setFont(ArialMT_Plain_24);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.drawString(10, 10, String(filteredWeight, 1) + String(" g"));
    display.setFont(ArialMT_Plain_10);
    display.drawString(10, 50, String(flow, 1) + String("g/s"));

    // TODO move to drawFlow
    unsigned int size = weight.getFlowHistorySize();
    float *history = weight.getFlowHistory();

    for (unsigned int i = 0; i < size; i++)
    {
      int round = (int)(history[i] * 10);
      int mapped = map(round, 0, 200, 0, 64);
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
