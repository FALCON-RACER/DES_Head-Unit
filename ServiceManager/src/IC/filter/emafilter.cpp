#include "../../../header/filter/emafilter.h"

EmaFilter::EmaFilter(QObject* parent, double alpha)
    : IFilter(parent), alpha(alpha), oneMinusAlpha(1.0f - alpha), ema(0.0f)
{}

EmaFilter::~EmaFilter()
{}

double EmaFilter::getEma() const
{
    return this->ema;
}

void EmaFilter::setEma(double value)
{
    this->ema = value;
}

void EmaFilter::setAlpha(double value)
{
    this->alpha = value;
    this->oneMinusAlpha = 1 - value;
}

double EmaFilter::calculateFilteredOutput(double value)
{
    this->ema = this->alpha * value + this->oneMinusAlpha * ema;
    return this->ema;
}

void EmaFilter::reset()
{
    this->ema = 0.0f;
}
