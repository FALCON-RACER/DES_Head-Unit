#ifndef SMAFILTER_H
#define SMAFILTER_H

#include "ifilter.h"

#include <QObject>
#include <array>

const int MAX_BUFFER_SIZE = 20;

class SmaFilter : public IFilter
{
    Q_OBJECT
public:
    explicit SmaFilter(QObject *parent, int windowSize = 7);
    ~SmaFilter();

    void setFilterParameters(int length);
    double calculateFilteredOutput(double value) override;
    void reset() override;

private:
    int windowSize;
    std::array<double, MAX_BUFFER_SIZE> buffer;
    int currentIndex;
    int count;
    double sum;
};

#endif // SMAFILTER_H
