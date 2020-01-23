#include "result.h"

Result::Result():Settings ()
{
    _isResultView = true;
}

Result::Result(const Settings &settings): Settings (settings)
{
    _isResultView = true;
}

void Result::addToSteps(QVector3D &step)
{
    steps.append(step);
}

void Result::addToCurrentPath(QVector3D &step)
{
    paths.last().push_front(step);
}

void Result::createNewPath()
{
    paths.append(QVector<QVector3D>());
}

void Result::setSettings(const Settings &settings)
{
    this->runMode = settings.runMode;
    this->neighberhood = settings.neighberhood;
    this->scene = *new Scene(settings.scene);
    this->_isSettingsSet = true;
}

bool Result::isDataSet() const
{
    return !steps.empty();
}

long long Result::getNumberOfExpandedStates() const
{
    return steps.size();
}
