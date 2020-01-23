#ifndef SCENE_H
#define SCENE_H

#include <QVector3D>
#include <QVector>
#include "voxel.h"
#include <QFile>
#include <QTextStream>
#include "vector3d.h"
#include <memory>

class Scene
{
public:
    Scene();
    Scene(QString path);
    Scene(const Scene &scene);
    ~Scene();

    Scene& operator=(const Scene&) = default;

public:
    QString name;
    QString path;
    QVector3D startPoint;
    QVector3D endPoint;
    Vector3D<Voxel> voxelSpace;

private:
    void serialize(QTextStream &file);
    void deserilaize(QTextStream &file);

    void startPointValue(int x, int y, int z);
    void endPointValue(int x, int y, int z);


public:
    void load(QString path);
    void save(QString path);

    bool isSet();

private:
    bool _isSet = false;
};

#endif // SCENE_H
