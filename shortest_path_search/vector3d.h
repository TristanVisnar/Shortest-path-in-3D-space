#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <memory>
#include <algorithm>
#include <QVector3D>

template<class T>
class Vector3D
{
public:
    Vector3D();
    Vector3D(unsigned int xSize,unsigned int ySize,unsigned int zSize, T *data);
    Vector3D(const Vector3D &vector3D);
    ~Vector3D();

public:
    T* data;
    unsigned int xSize;
    unsigned int ySize;
    unsigned int zSize;

public:

    // Returns element by reference
    T& operator()(unsigned int x,unsigned int y,unsigned int z);
    T& operator()(const QVector3D& loc);

    unsigned int getXSize() const;
    void setXSize(unsigned int value);
    unsigned int getYSize() const;
    void setYSize(unsigned int value);
    unsigned int getZSize() const;
    void setZSize(unsigned int value);

    unsigned long long getSize() const;
};

template<class T>
Vector3D<T>::Vector3D()
{

}

template<class T>
Vector3D<T>::Vector3D(unsigned int xSize,unsigned int ySize,unsigned int zSize,T *data)
{
    this->xSize = xSize;
    this->ySize = ySize;
    this->zSize = zSize;
    this->data = data;
}

template<class T>
Vector3D<T>::Vector3D(const Vector3D<T> &vector3D)
{
    this->xSize = vector3D.getXSize();
    this->ySize = vector3D.getYSize();
    this->zSize = vector3D.getZSize();

    this->data = new T[vector3D.getSize()];
    std::copy (vector3D.data, vector3D.data + vector3D.getSize(), this->data );
}

template<class T>
Vector3D<T>::~Vector3D()
{
    //delete []data;
}

template<class T>
T &Vector3D<T>::operator()(unsigned int x, unsigned int y, unsigned int z)
{
    return data[x * ySize * zSize + y * zSize + z];
}

template<class T>
T &Vector3D<T>::operator()(const QVector3D &loc)
{
    unsigned int x = static_cast<unsigned int>(loc.x());
    unsigned int y = static_cast<unsigned int>(loc.y());
    unsigned int z = static_cast<unsigned int>(loc.z());

    return data[x * ySize * zSize + y * zSize + z];
}

template<class T>
unsigned int Vector3D<T>::getZSize() const
{
    return zSize;
}

template<class T>
void Vector3D<T>::setZSize(unsigned int value)
{
    zSize = value;
}

template<class T>
unsigned long long Vector3D<T>::getSize() const
{
    return xSize*ySize*zSize;
}

template<class T>
unsigned int Vector3D<T>::getYSize() const
{
    return ySize;
}

template<class T>
void Vector3D<T>::setYSize(unsigned int value)
{
    ySize = value;
}

template<class T>
unsigned int Vector3D<T>::getXSize() const
{
    return xSize;
}

template<class T>
void Vector3D<T>::setXSize(unsigned int value)
{
    xSize = value;
}


#endif // VECTOR3D_H
