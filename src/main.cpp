
#include <U8g2lib.h>
#include <SPI.h>
#include "HX711.h"

#include "Weight.h"
#include "UI.h"

#include "bitmaps.h"

const int LOADCELL_DOUT_PIN = D2;
const int LOADCELL_SCK_PIN = D1;

U8G2_SH1122_256X64_1_4W_HW_SPI display(U8G2_R0, /* cs=*/D8, /* dc=*/D4, /* reset=*/D0);

HX711 scale;
Weight weight;
UI ui(display);

long timeSinceLastUpdate = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  display.begin();

  delay(1000);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  delay(2500);
  scale.set_scale(429.56f); // 5kg
  scale.tare();
}

void loop()
{
  weight.update(scale.get_units(1));
  ui.setWeight(weight.getRawWeight(), weight.getWeight());
  ui.setFlow(weight.getFlow(), weight.getFlowHistory(), weight.getFlowHistorySize());
  ui.update();
}
