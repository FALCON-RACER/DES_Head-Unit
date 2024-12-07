#ifndef EMAFILTER_H
#define EMAFILTER_H

class EmaFilter
{
public:
    EmaFilter(double alpha);
    double Run(double input);

private:
    double m_alpha;
    double m_lastOutput;
};

#endif
