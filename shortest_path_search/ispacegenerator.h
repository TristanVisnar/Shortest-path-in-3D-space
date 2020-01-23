#ifndef ISPACEGENERATOR_H
#define ISPACEGENERATOR_H

#include "scene.h"
#include "vector3d.h"
#include "voxel.h"
#include <random>
#include <chrono>

class ISpaceGenerator{
public:
    Q_DECL_CONSTEXPR static int NONE = -1;
    Q_DECL_CONSTEXPR static int RANDOM_SPACE = 0;
    Q_DECL_CONSTEXPR static int UNIFORMN_WEIGHTS_SPACE = 1;



public:
    ~ISpaceGenerator(){}

    virtual Scene run(unsigned int x, unsigned int y, unsigned int z, float obsticleOccurance, float hiddenObsticleOccurance ) = 0;

    static ISpaceGenerator* getGenerator(int id);

};



class RandomSpace : public ISpaceGenerator{
public:

    RandomSpace(int weightGenerationSettings): weightGenerationSettings(weightGenerationSettings){};

    Scene run(unsigned int x, unsigned int y, unsigned int z, float obsticleOccurance, float hiddenObsticleOccurance);

private:
    VoxelType::VoxelType calculateType(float randomeValue, const float& obsticleOccurance, const float& hiddenObsticleOccurance);
    int weightGenerationSettings;
};

VoxelType::VoxelType RandomSpace::calculateType(float randomeValue,const float &obsticleOccurance, const float &hiddenObsticleOccurance)
{
    if(randomeValue>obsticleOccurance)
        return VoxelType::NORMAL;

    if(randomeValue>hiddenObsticleOccurance*obsticleOccurance)
        return VoxelType::WALL;

    return VoxelType::HIDDEN_WALL;
}

Scene RandomSpace::run(unsigned int x,unsigned int y,unsigned int z, float obsticleOccurance, float hiddenObsticleOccurance){
    //randomizer
    std::random_device rd;
    std::mt19937 mt(rd());
    std::normal_distribution<float> normal(1.0f, 2.0f);
    std::uniform_real_distribution<float> uniform(0.0f,1.0f);

    Scene *scene = new Scene();
    unsigned long long size = x*y*z;
    Voxel *data = new Voxel[size]();
    for(unsigned long long i=1; i<size-1; i++){
        if(weightGenerationSettings == RANDOM_SPACE){
            data[i].setWeight(fmod(std::abs(normal(mt)),MAX_WEIGHT));
        }
        else if(weightGenerationSettings == UNIFORMN_WEIGHTS_SPACE){
            data[i].setWeight(1.0);
        }
        data[i].setVoxelType(calculateType(uniform(mt), obsticleOccurance,hiddenObsticleOccurance));
    }

    // First and last element are start and end points

    data[0].setWeight(1.0f);
    data[0].setVoxelType(VoxelType::NORMAL);
    data[size-1].setWeight(1.0f);
    data[size-1].setVoxelType(VoxelType::NORMAL);
    scene->startPoint = QVector3D(0,0,0);
    scene->endPoint = QVector3D(x-1,y-1,z-1);

    scene->voxelSpace = Vector3D<Voxel>(x,y,z,data);
    return *scene;
}



ISpaceGenerator* ISpaceGenerator::getGenerator(int id)
{
    return new RandomSpace(id);
}

#endif // ISPACEGENERATOR_H

