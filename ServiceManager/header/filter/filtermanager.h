#ifndef FILTERMANAGER_H
#define FILTERMANAGER_H

#include "ifilter.h"

#include <QObject>
#include <list>
#include <vector>

class FilterManager : public QObject
{
    Q_OBJECT

public:
    explicit FilterManager(QObject *parent = nullptr);
    ~FilterManager();

    enum FilterType
    {
        SMA = 0, EMA = 1, KALMAN = 2
    };
    Q_ENUM(FilterType)

    double filter(double value);
    Q_INVOKABLE void addFilter(FilterType type);
    Q_INVOKABLE void removeFilter(FilterType type);
    void reset();

private:
    std::list<IFilter*> activeFilters;
    std::vector<IFilter*> filters;
};

#endif // FILTERMANAGER_H
