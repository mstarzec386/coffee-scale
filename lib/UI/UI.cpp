#include "UI.h"

UI::UI(U8G2 &u8g2) : display(u8g2)
{
    this->lastUpdate = millis();
};

void UI::setDisplay(U8G2 &display)
{
    this->display = display;
}

void UI::update()
{
    if (millis() - this->lastUpdate > 50)
    {
        this->lastUpdate = millis();

        String rawWeightStr = String(this->rawWeight, 2) + String(" g");
        String filteredWeightStr = String(this->filteredWeight, 1) + String("");
        String flowStr = String(this->flowValue, 1);
        ;

        int timerTotalSeconds = this->getTimerSeconds();
        int timerMinutes = timerTotalSeconds / 60;
        int timerSeconds = timerTotalSeconds - 60 * timerMinutes;
        String timeStr = String(timerMinutes) + String(":") + String(timerSeconds);

        display.firstPage();
        do
        {
            display.setFont(u8g2_font_ncenB08_tr);

            // Raw Weight
            display.drawStr(80, 58, rawWeightStr.c_str());

            display.setFont(u8g2_font_ncenB24_tr);
            // Timer
            display.drawStr(1, 60, timeStr.c_str());

            // Weight
            display.drawStr(1, 25, filteredWeightStr.c_str());

            // Flow
            display.setFont(u8g2_font_ncenB18_tr);
            display.drawStr(120, 33, flowStr.c_str());
            display.setFont(u8g2_font_ncenB12_tr);
            display.drawStr(130, 48, String("g/s").c_str());

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

                display.drawCircle(256 - this->flowHistorySize * 2 + i * 2, 60 - mapped, 2);
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
    this->flowValue = flowValue;
    this->flowHistory = flowHistory;
    this->flowHistorySize = flowHistorySize;
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