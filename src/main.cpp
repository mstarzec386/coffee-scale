
#include <U8g2lib.h>
#include <SPI.h>
#include "HX711.h"

#include "Weight.h"

#include "bitmaps.h"

const int LOADCELL_DOUT_PIN = D2;
const int LOADCELL_SCK_PIN = D1;

U8G2_SH1122_256X64_1_4W_HW_SPI display(U8G2_R0, /* cs=*/D8, /* dc=*/D4, /* reset=*/D0);

HX711 scale;
Weight weight;

long timeSinceLastUpdate = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  display.begin();

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

    String rawWeightStr = String(rawWeight, 2) + String(" g");
    String filteredWeightStr = String(filteredWeight, 1) + String("");
    String flowStr = String(flow, 1) + String("g/s");

    display.firstPage();
    do
    {
      display.setFont(u8g2_font_ncenB10_tr);
      display.drawStr(1, 25, rawWeightStr.c_str());
      display.drawStr(1, 40, flowStr.c_str());
      display.setFont(u8g2_font_ncenB18_te);
      display.drawStr(85, 30, filteredWeightStr.c_str());

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

        display.drawCircle(256 - size * 2 + i * 2, 60 - mapped, 2);
      }
    } while (display.nextPage());

    // TODO add timer
    // TODO move to draw weight

    // TODO move to drawFlow
  }
}
