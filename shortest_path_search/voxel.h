#ifndef VOXEL_H
#define VOXEL_H

#include <QTextStream>
#include <QVector3D>
#include <QVector4D>

#define MIN_WEIGHT 1.0
#define MAX_WEIGHT 5.0
#define NORMAL_ALPHA 0.5

namespace VoxelType{
    enum VoxelType{NORMAL, WALL, HIDDEN_WALL, VISIBLE_HIDDEN_WALL};
}


class Voxel{
public:
    // Color settings
    Q_DECL_CONSTEXPR static QVector4D ColorNormal{1.0f,1.0f,1.0f,NORMAL_ALPHA};
    Q_DECL_CONSTEXPR static QVector4D ColorWall{0.0f,0.0f,0.0f,0.0f};
    Q_DECL_CONSTEXPR static QVector4D ColorHiddenWall{0.7f,0.3f,0.3f, 1.0f};
    Q_DECL_CONSTEXPR static QVector4D ColorVisibleHiddenWall{0.8f,0.2f,0.2f, 1.0f}; // change
    Q_DECL_CONSTEXPR static QVector4D ColorInQueue{0.0f,0.0f,1.0f, 0.1f};
    Q_DECL_CONSTEXPR static QVector4D ColorProcessNext{1.0f,0.0f,1.0f, 0.5f};
    Q_DECL_CONSTEXPR static QVector4D ColorProcessed{1.0f, 1.0f, 0.5f, 0.1f};
    Q_DECL_CONSTEXPR static QVector4D ColorBestPath{0.0, 1.0, 0.0, 0.1f};
    Q_DECL_CONSTEXPR static QVector4D ColorStartPoint{0.8f, 0.0f, 1.0f, 1.f};
    Q_DECL_CONSTEXPR static QVector4D ColorEndPoint{0.8f, 0.0f, 1.0f, 1.f};
public:
    Voxel();
    Voxel(float weight, VoxelType::VoxelType voxelType);
    Voxel(const Voxel &voxel);
    ~Voxel();

private:
    VoxelType::VoxelType voxelType;
    float weight;
    QVector4D _color;

public:
    void serialize(QTextStream &file);
    void deserialize(QTextStream &file);

    QVector4D getColor();
    void setColor(const QVector4D &color);
    void setColorToDefault();


    VoxelType::VoxelType getVoxelType() const;
    void setVoxelType(VoxelType::VoxelType value);
    float getWeight() const;
    void setWeight(float value);
};
#endif // VOXEL_H
