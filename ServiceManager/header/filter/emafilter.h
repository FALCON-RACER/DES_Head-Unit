#ifndef EMAFILTER_H
#define EMAFILTER_H

#include "ifilter.h"

class EmaFilter : public IFilter
{
public:
    explicit EmaFilter(QObject* parent, double alpha = 0.2);
    ~EmaFilter();

    double getEma() const;
    void setEma(double value);
    void setAlpha(double value);
    double calculateFilteredOutput(double value) override;
    void reset() override;

private:
    double alpha;
    double oneMinusAlpha;
    double ema;
};

#endif // EMAFILTER_H
