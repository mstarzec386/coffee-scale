
#ifndef _UI_h_
#define _UI_h_

#include <SimpleKalmanFilter.h>
#include "Arduino.h"
#include <U8g2lib.h>
#include <string.h>


class UI
{
public:
    UI(U8G2& u8g2);

    void update();
    void initialScreen(float batteryVoltage);
    void setDisplay(U8G2& display);
    void stopStartTimer();
    void resetTimer();
    void setWeight(float rawWeight, float filteredWeight);
    void setFlow(float flowValue, float *flowHistory, unsigned int flowHistorySize);
    void setBatteryVoltage(float batteryVoltage);
    void switchMode();

private:
    U8G2& display;
    unsigned long lastUpdate;
    unsigned long timerStart;
    int additionalSeconds;
    bool timerStarted;
    bool autoTimerEnabled;
    bool espressoMode;
    float rawWeight;
    float filteredWeight;
    float flowValue;
    int flowScaleMax;
    float *flowHistory;
    float batteryVoltage;
    unsigned int flowHistorySize;

    int getTimerSeconds();
    String getTimerStr();
};

#endif