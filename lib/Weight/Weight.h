#ifndef _Weight_h_
#define _Weight_h_

#include <SimpleKalmanFilter.h>
#include "Arduino.h"


class Weight
{
public:
    Weight();

    void update(float weight);
    void setTare();
    float getRawWeight();
    float getWeight();
    float getFlow();
    float* getFlowHistory();
    unsigned int getFlowHistorySize();

private:
    long lastFlowCheck;
    float lastFlowWeight;
    float rawWeight;
    float filteredWeight;
    float flow;
    float *flowHistory;
    unsigned int flowHistorySize;
    SimpleKalmanFilter kalmanFilter;
    void updateFlowHistory(float weight);
};

#endif