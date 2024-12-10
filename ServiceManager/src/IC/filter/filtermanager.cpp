#include "../../../header/filter/filtermanager.h"
#include "smafilter.h"
#include "emafilter.h"
#include "kalmanfilter.h"

FilterManager::FilterManager(QObject *parent)
    : QObject{parent}
{
    this->filters.push_back(new SmaFilter(this, 10));
    this->filters.push_back(new EmaFilter(this, 0.3f));
    this->filters.push_back(new KalmanFilter(this));
}

FilterManager::~FilterManager()
{}

double FilterManager::filter(double value)
{
    double filteredValue = value;

    for (auto filterPtr : this->activeFilters)
    {
        filteredValue = filterPtr->calculateFilteredOutput(filteredValue);
    }

    return filteredValue;
}

void FilterManager::addFilter(FilterType type)
{
    IFilter* toAdd = this->filters[type];

    if (toAdd && std::find(activeFilters.begin(), activeFilters.end(), toAdd) == activeFilters.end())
    {
        this->activeFilters.push_back(toAdd);
    }
    this->reset();
}

void FilterManager::removeFilter(FilterType type)
{
    IFilter* toRemove = this->filters[type];

    if (toRemove)
    {
        activeFilters.remove(toRemove);
    }
    this->reset();
}

void FilterManager::reset()
{
    for (auto filterPtr : this->filters)
    {
        filterPtr->reset();
    }
}
