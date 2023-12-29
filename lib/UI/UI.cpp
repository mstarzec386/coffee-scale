#include "UI.h"

UI::UI(U8G2 &u8g2, Weight &weight) : display(u8g2), weight(weight)
{
    this->lastUpdate = millis();
    this->espressoMode = false;
    this->flowScaleMax = 200;
    this->autoTimerEnabled = false;
};

void UI::initialScreen(float batteryVoltage)
{
    String batteryVoltageStr = String(batteryVoltage, 2);
    display.firstPage();
    do
    {
        display.setFont(u8g2_font_ncenB24_tr);
        display.drawStr(40, 30, "Coffe Scale");
        display.setFont(u8g2_font_ncenB12_tr);
        display.drawStr(120, 55, batteryVoltageStr.c_str());
    } while (display.nextPage());
}

void UI::update()
{
    // TODO create const for this magic number
    float flow = this->weight.getFlow();
    float filteredWeight = this->weight.getWeight();

    if (millis() - this->lastUpdate > 50)
    {
        this->lastUpdate = millis();

        // TODO create const for this magic number
        if (this->autoTimerEnabled && filteredWeight > 0.3)
        {
            this->startTimer();
        }

        String timeStr = this->getTimerStr();
        String filteredWeightStr = String(filteredWeight, 1) + String("");
        String flowStr;
        String modeStr;

        if (this->espressoMode)
        {
            modeStr = "E";

            this->stopOnFinish();
        }
        else
        {
            modeStr = "D";
        }
        if (this->autoTimerEnabled)
        {
            modeStr += "A";
        }

        if (flow > 20)
        {
            flowStr += String(flow, 0);
        }
        else
        {
            flowStr += String(flow, 1);
        }

        this->draw(timeStr, filteredWeightStr, flowStr, modeStr);
    }
}

void UI::stopStartTimer()
{
    if (this->timerStarted)
    {
        this->stopTimer();
    }
    else if (!this->autoTimerEnabled && this->additionalSeconds == 0)
    {
        this->autoTimerEnabled = true;
    }
    else
    {
        this->startTimer();
    }
}

void UI::switchMode()
{
    this->espressoMode ^= 1;
    this->autoTimerEnabled = false;

    if (this->espressoMode)
    {
        this->flowScaleMax = 50;
    }
    else
    {
        this->flowScaleMax = 200;
    }
}
void UI::resetTimer()
{
    this->additionalSeconds = 0;
    this->timerStart = millis();
    this->timerStarted = false;
}

void UI::setBatteryVoltage(float batteryVoltage)
{
    this->batteryVoltage = batteryVoltage;
}

// Privates

void UI::draw(String timeStr, String filteredWeightStr, String flowStr, String modeStr)
{
    display.firstPage();
    do
    {
        display.setFont(u8g2_font_ncenB24_tr);
        // Timer
        display.drawStr(1, 60, timeStr.c_str());

        // Weight
        // TODO fixed size -> getFilteredWeightStr()!
        display.drawStr(1, 25, filteredWeightStr.c_str());

        // Flow
        display.setFont(u8g2_font_ncenB18_tr);
        display.drawStr(90, 60, flowStr.c_str());

        // Mode
        display.setFont(u8g2_font_ncenB08_tr);
        display.drawStr(120, 10, modeStr.c_str());

        float *flowHistory = this->weight.getFlowHistory();
        unsigned int flowHistorySize = this->weight.getFlowHistorySize();
        // TODO optimize  && can be moved to separate function
        for (unsigned int i = 0; i < flowHistorySize; i++)
        {
            int round = (int)(flowHistory[i] * 10);
            int mapped = map(round, 0, this->flowScaleMax, 0, 60);
            if (mapped > 60)
            {
                mapped = 60;
            }

            if (mapped < 0)
            {
                mapped = 0;
            }

            display.drawCircle(256 - flowHistorySize + i, 62 - mapped, 1);
        }
    } while (display.nextPage());
}

int UI::getTimerSeconds()
{
    int seconds = this->additionalSeconds;

    if (this->timerStarted)
    {
        seconds += (millis() - this->timerStart) / 1000;
    }

    return seconds;
}

void UI::stopOnFinish()
{
    float *flowHistory = this->weight.getFlowHistory();
    unsigned int flowHistorySize = this->weight.getFlowHistorySize();

    if (this->getTimerSeconds() > 10 && (flowHistory[flowHistorySize - 1] + flowHistory[flowHistorySize - 2]) < 0.2)
    {
        this->stopTimer();
    }
}

void UI::startTimer()
{
    this->timerStart = millis();
    this->timerStarted = true;
    this->autoTimerEnabled = false;
}

void UI::stopTimer()
{
    this->additionalSeconds = this->getTimerSeconds();
    this->timerStarted = false;
}

String UI::getTimerStr()
{
    int timerTotalSeconds = this->getTimerSeconds();
    int timerMinutes = timerTotalSeconds / 60;
    int timerSeconds = timerTotalSeconds - 60 * timerMinutes;
    String timeStr = String(timerMinutes) + String(":");

    if (timerSeconds < 10)
    {
        timeStr += String("0") + String(timerSeconds);
    }
    else
    {
        timeStr += String(timerSeconds);
    }

    return timeStr;
}