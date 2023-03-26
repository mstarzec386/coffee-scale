
#include "Weight.h"

Weight::Weight()
    : kalmanFilter(1, 1, 0.1)
{
    this->lastFlowWeight = 0;
    this->lastFlowCheck = millis();
    this->filteredResult = 0;
    this->flow = 0;
    this->flowHistorySize = 20;
    this->flowHistory = new float[this->flowHistorySize];

    for (unsigned int i = 0; i < this->flowHistorySize; i++)
    {
        this->flowHistory[i] = 0;
    }
};

void Weight::update(float weight)
{
    this->filteredResult = this->kalmanFilter.updateEstimate(weight);

    if (millis() - this->lastFlowCheck > 1000)
    {
        this->flow = this->filteredResult - this->lastFlowWeight;
        this->lastFlowWeight = this->filteredResult;
        this->lastFlowCheck = millis();
        this->updateFlowHistory(this->flow);
    }
}

float Weight::getWeight()
{
    return this->filteredResult;
}

float Weight::getFlow()
{
    return this->flow;
}

float* Weight::getFlowHistory()
{
    return this->flowHistory;
}

unsigned int Weight::getFlowHistorySize()
{
    return this->flowHistorySize;
}

void Weight::updateFlowHistory(float weight)
{
    for (int i = 0; i < this->flowHistorySize - 1; i++)
    {
        this->flowHistory[i] = this->flowHistory[i + 1];
    }

    this->flowHistory[this->flowHistorySize - 1] = weight;
}