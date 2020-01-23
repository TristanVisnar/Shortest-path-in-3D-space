#include "scene.h"

Scene::Scene()
{
    this->name = "";
    this->path = "";
}

Scene::Scene(QString path)
{
    this->path = path;
    //this->name = filename;
}

Scene::Scene(const Scene &scene)
{
    this->name = scene.name;
    this->path = scene.path;
    this->startPoint = scene.startPoint;
    this->endPoint = scene.endPoint;
    this->voxelSpace = *new Vector3D<Voxel>(scene.voxelSpace);
    this->_isSet = scene._isSet;
}

Scene::~Scene()
{

}

void Scene::serialize(QTextStream &file)
{
    file << this->voxelSpace.getXSize() << ' '
         << this->voxelSpace.getYSize() << ' '
         << this->voxelSpace.getZSize() << '\n';

    file << this->startPoint.x() << ' '
         << this->startPoint.y() << ' '
         << this->startPoint.z() << '\n';

    file << this->endPoint.x() << ' '
         << this->endPoint.y() << ' '
         << this->endPoint.z() << '\n';

    for(unsigned int x=0; x<this->voxelSpace.getXSize(); x++){
        for(unsigned int y=0; y<this->voxelSpace.getYSize(); y++){
            for(unsigned int z=0; z<this->voxelSpace.getZSize(); z++){
                this->voxelSpace(x,y,z).serialize(file);
            }
        }
    }
    
}

void Scene::deserilaize(QTextStream &file)
{
    // Creates object from file
    //Save sizes
    unsigned int xSize, ySize, zSize;
    file >> xSize >> ySize >> zSize;

    int x,y,z;
    // Start point
    file >> x >> y >> z;
    startPointValue(x,y,z);
    // End point
    file >> x >> y >> z;
    endPointValue(x,y,z);

    Voxel *data = new Voxel[xSize*ySize*zSize]();

    for(unsigned int i=0; i<xSize*ySize*zSize; i++){
        data[i].deserialize(file);
    }

    this->voxelSpace = {xSize,ySize,zSize,data};

}

void Scene::startPointValue(int x, int y, int z)
{
    startPoint.setX(x);
    startPoint.setY(y);
    startPoint.setZ(z);
}

void Scene::endPointValue(int x, int y, int z)
{
    endPoint.setX(x);
    endPoint.setY(y);
    endPoint.setZ(z);
}

void Scene::load(QString path)
{   
    if(path!=""){
        this->path = path;
    }

    if(this->path == ""){
        //error
        return;
    }

    QFile file(this->path);

    if(!file.open(QIODevice::ReadOnly)){
        //error - file not opened
    }

    QTextStream textStream(&file);
    deserilaize(textStream);

    file.close();

    _isSet = true;
}

void Scene::save(QString path)
{
    if(path!=""){
        this->path = path;
    }

    if(this->path == ""){
        //error
        return;
    }

    QFile file(this->path);

    if(!file.open(QIODevice::WriteOnly)){
        //error - file not opened
    }


    QTextStream textStream(&file);
    serialize(textStream);

    file.close();
}

bool Scene::isSet()
{
    return _isSet;
}
