#ifndef _Weight_h_
#define _Weight_h_

#include <SimpleKalmanFilter.h>
#include "Arduino.h"


class Weight
{
public:
    Weight();

    void update(float weight);
    float getWeight();
    float getFlow();
    float* getFlowHistory();
    unsigned int getFlowHistorySize();

private:
    long lastFlowCheck;
    float lastFlowWeight;
    float filteredResult;
    float flow;
    float *flowHistory;
    unsigned int flowHistorySize;
    SimpleKalmanFilter kalmanFilter;
    void updateFlowHistory(float weight);
};

#endif