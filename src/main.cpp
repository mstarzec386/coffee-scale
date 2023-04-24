
#include <U8g2lib.h>
#include "HX711.h"
#include "OneButton.h"

#include "Weight.h"
#include "UI.h"

#include "bitmaps.h"

const int LOADCELL_DOUT_PIN = D2; // d6 can be used instead, d3 can't
const int LOADCELL_SCK_PIN = D1;
const int TARE_BUTTON_PIN = 10; // can't boot when connected to gnd :joy:
const int TIME_BUTTON_PIN = D3; // external pull-up

OneButton timeButton(TIME_BUTTON_PIN);

U8G2_SH1122_256X64_1_4W_HW_SPI display(U8G2_R0, /* cs=*/D8, /* dc=*/D4, /* reset=*/D0);
// U8G2_SH1122_256X64_1_3W_SW_SPI display(U8G2_R0, /* cs=*/D8, /* dc=*/D4, /* reset=*/D0);

HX711 scale;
Weight weight;
UI ui(display);

long timeSinceLastUpdate = 0;

void timeButtonSinglePressed() {
  Serial.printf("timeButtonPressed\n");
  ui.stopStartTimer();
}

void timeButtonLongPressed() {
  Serial.printf("timeButtonPressed\n");
  ui.resetTimer();
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  pinMode(TARE_BUTTON_PIN, INPUT_PULLUP);
  // pinMode(TIME_BUTTON_PIN, INPUT_PULLUP);


  timeButton.attachClick(timeButtonSinglePressed);
  timeButton.attachLongPressStart(timeButtonLongPressed);
  display.begin();

  delay(500);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  delay(500);
  scale.set_scale(429.56f); // 5kg
  scale.tare();
  ui.resetTimer();
}

void loop()
{
  timeButton.tick();

  if (digitalRead(TARE_BUTTON_PIN) == LOW) {
    scale.tare();
  }

  // if (digitalRead(TIME_BUTTON_PIN) == LOW) {
  //   ui.stopStartTimer();
  // }

  Serial.println(timeButton.getNumberClicks());

  weight.update(scale.get_units(1));
  ui.setWeight(weight.getRawWeight(), weight.getWeight());
  ui.setFlow(weight.getFlow(), weight.getFlowHistory(), weight.getFlowHistorySize());
  ui.update();
}
