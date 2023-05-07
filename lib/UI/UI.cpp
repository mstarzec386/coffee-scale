#include "UI.h"

UI::UI(U8G2 &u8g2) : display(u8g2)
{
    this->lastUpdate = millis();
};

void UI::setDisplay(U8G2 &display)
{
    this->display = display;
}

void UI::initialScreen()
{
    display.firstPage();
    do
    {
        display.setFont(u8g2_font_ncenB24_tr);
        display.drawStr(40, 30, "Coffe Scale");
    } while (display.nextPage());
}

void UI::update()
{
    if (millis() - this->lastUpdate > 50)
    {
        this->lastUpdate = millis();

        String timeStr = this->getTimerStr();
        String filteredWeightStr = String(this->filteredWeight, 1) + String("");
        String flowStr;
        if (this->flowValue > 20)
        {
            flowStr += String(this->flowValue, 0);
        }
        else
        {
            flowStr += String(this->flowValue, 1);
        }

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

            // TODO optimize
            for (unsigned int i = 0; i < this->flowHistorySize; i++)
            {
                int round = (int)(this->flowHistory[i] * 10);
                int mapped = map(round, 0, 200, 0, 64);
                if (mapped > 60)
                {
                    mapped = 60;
                }

                if (mapped < 0)
                {
                    mapped = 0;
                }

                display.drawCircle(256 - this->flowHistorySize + i, 60 - mapped, 1);
            }
        } while (display.nextPage());
        // TODO move to draw weight

        // TODO move to drawFlow
    }
}

void UI::stopStartTimer()
{
    if (this->timerStarted)
    {
        this->additionalSeconds = this->getTimerSeconds();
        this->timerStarted = false;
    }
    else
    {
        this->timerStart = millis();
        this->timerStarted = true;
    }
}

void UI::resetTimer()
{
    this->additionalSeconds = 0;
    this->timerStart = millis();
    this->timerStarted = false;
}

void UI::setWeight(float rawWeight, float filteredWeight)
{
    this->rawWeight = rawWeight;
    this->filteredWeight = filteredWeight;
}

void UI::setFlow(float flowValue, float *flowHistory, unsigned int flowHistorySize)
{
    if (flowValue < 0)
    {
        this->flowValue = 0;
    }
    else
    {
        this->flowValue = flowValue;
    }

    this->flowHistory = flowHistory;
    this->flowHistorySize = flowHistorySize;
}

void UI::setBatteryVoltage(float batteryVoltage)
{
    this->batteryVoltage = batteryVoltage;
}

// Privates

int UI::getTimerSeconds()
{
    int seconds = this->additionalSeconds;

    if (this->timerStarted)
    {
        seconds += (millis() - this->timerStart) / 1000;
    }

    return seconds;
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