#include "voxel.h"

Voxel::Voxel()
{
    this->weight = MIN_WEIGHT;
    this->voxelType = VoxelType::NORMAL;
    _color = QVector4D(ColorNormal);
}

Voxel::Voxel(float weight, VoxelType::VoxelType voxelType){
    this->weight = weight;
    this->voxelType = voxelType;
    _color = (voxelType == VoxelType::WALL)? QVector4D(ColorWall) : QVector4D(ColorNormal);
}

Voxel::Voxel(const Voxel &voxel)
{
    this->weight = voxel.weight;
    this->voxelType = voxel.voxelType;
    _color = (voxelType == VoxelType::WALL)? QVector4D(ColorWall) : QVector4D(ColorNormal);
}

Voxel::~Voxel()
{

}

float Voxel::getWeight() const
{
    return weight;
}

void Voxel::setWeight(float value)
{
    if(value<MIN_WEIGHT){
        weight=MIN_WEIGHT;
        return;
    }
    if(value>MAX_WEIGHT){
        weight=MAX_WEIGHT;
        return;
    }
    weight = value;
}

VoxelType::VoxelType Voxel::getVoxelType() const
{
    return voxelType;
}

void Voxel::setVoxelType(VoxelType::VoxelType value)
{
    voxelType = value;
}

void Voxel::serialize(QTextStream &file)
{
    file << this->weight << ' '
         << this->voxelType << '\n';
}

void Voxel::deserialize(QTextStream &file)
{
    float weight;
    int voxelType;
    file >> weight >> voxelType;
    setWeight(weight);
    setVoxelType(VoxelType::VoxelType(voxelType));
}

QVector4D Voxel::getColor()
{
    if(_color.isNull())
        setColorToDefault();
    return _color;
}

void Voxel::setColor(const QVector4D &color)
{
    _color = color;
}

void Voxel::setColorToDefault()
{
    switch(voxelType){
    case VoxelType::WALL:
        _color = QVector4D(ColorWall);
        return;
    case VoxelType::VISIBLE_HIDDEN_WALL:
        _color = QVector4D(ColorVisibleHiddenWall);
        return;
    case VoxelType::HIDDEN_WALL:
        _color = QVector4D(ColorHiddenWall);
        return;
    case VoxelType::NORMAL:
        _color = QVector4D(ColorNormal) / weight; // from 1.0 to 0.2
        _color.setW(NORMAL_ALPHA); // reseting alfa
        return;
    }

}
