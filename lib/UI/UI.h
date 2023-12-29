
#ifndef _UI_h_
#define _UI_h_

#include <SimpleKalmanFilter.h>
#include "Arduino.h"
#include <string.h>
#include <U8g2lib.h>
#include "../Weight/Weight.h"

class UI
{
public:
    UI(U8G2 &u8g2, Weight &weight);

    void update();
    void initialScreen(float batteryVoltage);
    void stopStartTimer();
    void resetTimer();
    void setBatteryVoltage(float batteryVoltage);
    void switchMode();

private:
    U8G2 &display;
    Weight &weight;
    unsigned long lastUpdate;
    unsigned long timerStart;
    int additionalSeconds;
    bool timerStarted;
    bool autoTimerEnabled;
    bool espressoMode;
    int flowScaleMax;
    float batteryVoltage;

    void draw(String timeStr, String filteredWeightStr, String flowStr, String modeStr);
    int getTimerSeconds();
    void stopOnFinish();
    void startTimer();
    void stopTimer();
    String getTimerStr();
};

#endif