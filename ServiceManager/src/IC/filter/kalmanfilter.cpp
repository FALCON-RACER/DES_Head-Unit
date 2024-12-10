#include "../../../header/filter/kalmanfilter.h"

KalmanFilter::KalmanFilter(QObject* parent, double processNoise, double measurementNoise, double estimationError)
    : IFilter(parent)
    , processNoise(processNoise)
    , measurementNoise(measurementNoise)
    , estimationError(estimationError)
    , kalmanGain(0.0f)
    , currentEstimate(0.0f)
    , initialEstimationError(estimationError)
{
}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::setProcessNoise(double valueP, double valueM, double valueR)
{
    this->processNoise = valueP;
    this->measurementNoise = valueM;
    this->estimationError = valueR;
    this->initialEstimationError = valueR;
}

double KalmanFilter::calculateFilteredOutput(double measurement)
{
    // Estimation step
    this->estimationError += this->processNoise;

    // Correction step
    this->kalmanGain = this->estimationError / (this->estimationError + this->measurementNoise);
    this->currentEstimate = this->currentEstimate + this->kalmanGain * (measurement - this->currentEstimate);
    this->estimationError = (1.0f - this->kalmanGain) * this->estimationError + this->processNoise;

    return currentEstimate;
}

void KalmanFilter::reset()
{
    this->currentEstimate = 0.0f;
    this->estimationError = this->initialEstimationError;
}
