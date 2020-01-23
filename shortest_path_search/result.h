#ifndef RESULT_H
#define RESULT_H

#include <QVector>
#include <QVector3D>
#include "settings.h"
#include <QElapsedTimer>

class Result : public Settings
{
public:

    QVector<QVector3D> steps;
    QVector<QVector<QVector3D>> paths;

    double runTime = 0.0;
    QString algorithmName="";

    Result();
    Result(const Settings &settings);

    void addToSteps(QVector3D &step);
    void addToCurrentPath(QVector3D &step);
    void createNewPath();
    void setSettings(const Settings &settings);

    bool isDataSet() const;

    long long getNumberOfExpandedStates() const;

};

#endif // RESULT_H
