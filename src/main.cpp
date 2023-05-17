
#include <U8g2lib.h>
#include "OneButton.h"
#include <Wire.h>
#include "SparkFun_Qwiic_Scale_NAU7802_Arduino_Library.h"

#include "Weight.h"
#include "UI.h"

#include "bitmaps.h"

const int LOADCELL_DOUT_PIN = D2;
const int LOADCELL_SCK_PIN = D1;
const int TARE_BUTTON_PIN = 10;
const int TIME_BUTTON_PIN = D3;
const int BATTERY_ADC_PIN = A0;

OneButton timeButton(TIME_BUTTON_PIN);
OneButton tareButton(TARE_BUTTON_PIN);

U8G2_SH1122_256X64_1_4W_HW_SPI display(U8G2_R0, /* cs=*/D8, /* dc=*/D4, /* reset=*/D0);

NAU7802 scale;
Weight weight;
UI ui(display);

long timeSinceLastUpdate = 0;

void timeButtonSinglePressed()
{
  ui.stopStartTimer();
}

void timeButtonLongPressed()
{
  ui.resetTimer();
}

void tareButtonSinglePressed()
{
    scale.calculateZeroOffset();
}

void calibrateScale(void)
{
  Serial.println();
  Serial.println();
  Serial.println(F("Scale calibration"));

  Serial.println(F("Setup scale with no weight on it. Press a key when ready."));
  while (Serial.available())
    Serial.read(); // Clear anything in RX buffer
  while (Serial.available() == 0)
    delay(10); // Wait for user to press key

  scale.calculateZeroOffset(64); // Zero or Tare the scale. Average over 64 readings.
  Serial.print(F("New zero offset: "));
  Serial.println(scale.getZeroOffset());

  Serial.println(F("Place known weight on scale. Press a key when weight is in place and stable."));
  while (Serial.available())
    Serial.read(); // Clear anything in RX buffer
  while (Serial.available() == 0)
    delay(10); // Wait for user to press key

  Serial.print(F("Please enter the weight, without units, currently sitting on the scale (for example '4.25'): "));
  while (Serial.available())
    Serial.read(); // Clear anything in RX buffer
  while (Serial.available() == 0)
    delay(10); // Wait for user to press key

  // Read user input
  float weightOnScale = Serial.parseFloat();
  Serial.println();

  scale.calculateCalibrationFactor(weightOnScale, 64); // Tell the library how much weight is currently on it
  Serial.print(F("New cal factor: "));
  Serial.println(scale.getCalibrationFactor(), 2);

  Serial.print(F("New Scale Reading: "));
  Serial.println(scale.getWeight(), 2);
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  Wire.begin();
  if (scale.begin() == false)
  {
    Serial.println("Scale not detected. Please check wiring. Freezing...");
    while (1)
      ;
  }
  Serial.println("Scale detected!");

  // TODO EEPROM
  scale.calculateZeroOffset(64);
  scale.setCalibrationFactor(981.44);

  timeButton.attachClick(timeButtonSinglePressed);
  timeButton.attachLongPressStart(timeButtonLongPressed);
  tareButton.attachClick(tareButtonSinglePressed);
  display.begin();
  delay(500);

  int batteryRaw = analogRead(BATTERY_ADC_PIN);
  float batteryVoltage = (float)map(batteryRaw, 0, 894, 0, 408)/100.0;
  Serial.println(batteryVoltage);
  ui.resetTimer();
  ui.initialScreen(batteryVoltage);
  delay(500);
}

void loop()
{
  timeButton.tick();
  tareButton.tick();


  if (Serial.available())
  {
    byte incoming = Serial.read();

    if (incoming == 't') // Tare the scale
      scale.calculateZeroOffset();
    else if (incoming == 'c') // Calibrate
    {
      calibrateScale();
    }
  }

  if (scale.available() == true)
  {
    float currentWeight = scale.getWeight();
    weight.update(currentWeight);
  }

  int batteryRaw = analogRead(BATTERY_ADC_PIN);
  float batteryVoltage = (float)map(batteryRaw, 0, 894, 0, 408)/100.0;
  Serial.println(batteryVoltage);
  ui.setBatteryVoltage(batteryVoltage);
  ui.setWeight(weight.getRawWeight(), weight.getWeight());
  ui.setFlow(weight.getFlow(), weight.getFlowHistory(), weight.getFlowHistorySize());
  ui.update();
}
