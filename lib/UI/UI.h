
#ifndef _UI_h_
#define _UI_h_

#include <SimpleKalmanFilter.h>
#include "Arduino.h"


class UI
{
public:
    UI();

    void update();

private:
    long lastUpdate;
};

#endif