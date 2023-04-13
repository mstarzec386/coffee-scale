
#include <U8g2lib.h>
#include <SPI.h>
#include "HX711.h"

#include "Weight.h"
#include "UI.h"

#include "bitmaps.h"

const int LOADCELL_DOUT_PIN = D2;
const int LOADCELL_SCK_PIN = D1;
const int TARE_BUTTON_PIN = D6; // pullup dont work
const int TIME_BUTTON_PIN = D3; // can't boot when connected to gnd :joy:

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
  pinMode(TARE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(TIME_BUTTON_PIN, INPUT_PULLUP);

  display.begin();

  delay(500);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  delay(500);
  scale.set_scale(429.56f); // 5kg
  scale.tare();
}

void loop()
{
  if (digitalRead(TIME_BUTTON_PIN) == LOW) {
    scale.tare();
  }

  weight.update(scale.get_units(1));
  ui.setWeight(weight.getRawWeight(), weight.getWeight());
  ui.setFlow(weight.getFlow(), weight.getFlowHistory(), weight.getFlowHistorySize());
  ui.update();
}
