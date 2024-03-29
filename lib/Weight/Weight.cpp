
#include "Weight.h"

Weight::Weight()
    : kalmanFilter(0.1, 0.1, 0.1)
{
    this->lastFlowWeight = 0;
    this->lastFlowCheck = millis();
    this->rawWeight = 0;
    this->filteredWeight = 0;
    this->flow = 0;
    this->flowHistorySize = 110;
    this->flowHistory = new float[this->flowHistorySize];

    for (unsigned int i = 0; i < this->flowHistorySize; i++)
    {
        this->flowHistory[i] = 0;
    }
};

void Weight::update(float weight)
{
    this->rawWeight = weight;
    this->filteredWeight = this->kalmanFilter.updateEstimate(weight);

    // TODO move 500ms to variable, '* 2' should base on this variable
    if (millis() - this->lastFlowCheck > 500)
    {
        this->prevFlow = this->flow;
        this->flow = (this->filteredWeight - this->lastFlowWeight) * 2; // because we messure in 500ms
        this->lastFlowWeight = this->filteredWeight;
        this->lastFlowCheck = millis();
        this->updateFlowHistory(this->flow);
    }
}

// TODO
void Weight::setTare()
{
}

float Weight::getRawWeight()
{
    return this->rawWeight;
}

float Weight::getWeight()
{
    return this->filteredWeight;
}

float Weight::getFlow()
{
    return this->flow;
}

float *Weight::getFlowHistory()
{
    return this->flowHistory;
}

unsigned int Weight::getFlowHistorySize()
{
    return this->flowHistorySize;
}

void Weight::updateFlowHistory(float weight)
{
    for (unsigned int i = 0; i < this->flowHistorySize - 1; i++)
    {
        this->flowHistory[i] = this->flowHistory[i + 1];
    }

    this->flowHistory[this->flowHistorySize - 1] = weight;
}
