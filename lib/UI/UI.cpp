#include "UI.h"

UI::UI()
{
    this->lastUpdate = millis();

};

void UI::update()
{
    if (millis() - this->lastUpdate > 1000)
    {
        this->lastUpdate = millis();
    }
}