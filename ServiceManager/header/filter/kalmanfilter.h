#ifndef KALMANFILTER_H
#define KALMANFILTER_H

#include "ifilter.h"

class KalmanFilter : public IFilter
{
public:
    explicit KalmanFilter(QObject* parent
                          , double processNoise = 1.0f
                          , double measurementNoise = 10.0f
                          , double estimationError = 100.0f);
    ~KalmanFilter();

    void setProcessNoise(double valueP, double valueM, double valueR);
    double calculateFilteredOutput(double measurement) override;
    void reset() override;

private:
    double processNoise;
    double measurementNoise;
    double estimationError;
    double kalmanGain;
    double currentEstimate;
    double initialEstimationError;
};

#endif // KALMANFILTER_H
