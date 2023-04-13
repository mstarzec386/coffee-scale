#include "UI.h"

UI::UI(U8G2& u8g2): display(u8g2)
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
        String flowStr = String(this->flowValue, 1) + String("g/s");

        display.firstPage();
        do
        {
            display.setFont(u8g2_font_ncenB10_tr);
            display.drawStr(1, 25, rawWeightStr.c_str());
            display.drawStr(1, 40, flowStr.c_str());
            display.setFont(u8g2_font_ncenB18_te);
            display.drawStr(85, 30, filteredWeightStr.c_str());

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

        // TODO add timer
        // TODO move to draw weight

        // TODO move to drawFlow
    }
}

void UI::stopStartTimer() {}
void UI::resetTimer() {}
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