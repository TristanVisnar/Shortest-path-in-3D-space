#ifndef ISEARCHALGORITHM_H
#define ISEARCHALGORITHM_H

#include "settings.h"
#include "result.h"
#include "vector"
#include <QVector3D>
#include <iostream>
#include <math.h>

namespace SearchAlgorithmNames {
enum SearchAlgorithmNames{DJIKSTRA_ALGORITHM, A_STAR_ALGORITHM, D_STAR_ALGORITHM};
}

class ISearchAlgorithm
{

public:
    Settings settings;

    inline ISearchAlgorithm(const Settings& settings): settings(settings){}
    virtual ~ISearchAlgorithm(){}

    virtual void run(Result& result) = 0;
    virtual QString getName() = 0;

protected:
    static constexpr float straghtMove = 1.000f;
    static constexpr float diagonalMove2d = 1.414f;
    static constexpr float diagonalMove3d = 1.732f;

    Q_DECL_CONSTEXPR static  QVector3D _moveX{1,0,0};
    Q_DECL_CONSTEXPR static  QVector3D _moveY{0,1,0};
    Q_DECL_CONSTEXPR static  QVector3D _moveZ{0,0,1};

    inline float manhattanDistance6n(const QVector3D &loc1,const QVector3D &loc2);
    inline float euclidianDistance26n(const QVector3D &loc1,const QVector3D &loc2);
public:
    struct State{
        float gVal = 100000000.f;
        QVector3D loc;
        const State *prevState = nullptr;

        State(float gVal, QVector3D loc, const State * prevState): gVal(gVal), loc(loc), prevState(prevState){}

        bool operator< (const State& st){
            return gVal > st.gVal;
        }

        bool operator== (const State& st){
            return loc == st.loc;
        }

        friend bool operator< (const State& l,const State& r){
            return l.gVal > r.gVal;
        }

        friend bool operator== (const State& l,const State& r){
            return l.loc == r.loc;
        }
    };


    inline std::vector<State> getNeighbours(const State& st);
    inline float heuristic(QVector3D loc1, QVector3D loc2);
private:
    inline float getWeight(QVector3D from, QVector3D to);
    inline void addNeighbour(std::vector<ISearchAlgorithm::State> &neighbours, QVector3D loc, const State &state);
    inline bool isMovable(QVector3D loc);
};

float ISearchAlgorithm::getWeight(QVector3D from, QVector3D to){
    int val = 0;
    val += abs(from.x()-to.x());
    val += abs(from.y()-to.y());
    val += abs(from.z()-to.z());

    float weight = settings.scene.voxelSpace(to).getWeight();

    if(val >= 3) weight *= diagonalMove3d;
    else if(val >= 2) weight *= diagonalMove2d;
    else if(val >= 1) weight *= straghtMove;

    return weight;
}

bool ISearchAlgorithm::isMovable(QVector3D loc){
    if(loc.x() < 0 || loc.x()>=settings.scene.voxelSpace.getXSize())return false;
    if(loc.y() < 0 || loc.y()>=settings.scene.voxelSpace.getYSize())return false;
    if(loc.z() < 0 || loc.z()>=settings.scene.voxelSpace.getZSize())return false;
    if(settings.scene.voxelSpace(loc).getVoxelType() == VoxelType::WALL) return false;
    if(settings.scene.voxelSpace(loc).getVoxelType() == VoxelType::VISIBLE_HIDDEN_WALL) return false;
    return true;
}

std::vector<ISearchAlgorithm::State> ISearchAlgorithm::getNeighbours(const State& state){
    std::vector<ISearchAlgorithm::State> neighbours;

    auto moveX = _moveX;
    auto moveY = _moveY;
    auto moveZ = _moveZ;

    auto addNeighbour = [&](QVector3D loc){if(isMovable(loc))neighbours.push_back({state.gVal + getWeight(state.loc,loc), loc, &state });};

    // x-axis movement
    addNeighbour(state.loc - moveX);
    addNeighbour(state.loc + moveX);

    // y-axis movement
    addNeighbour(state.loc - moveY);
    addNeighbour(state.loc + moveY);

    // z-axis movement
    addNeighbour(state.loc - moveZ);
    addNeighbour(state.loc + moveZ);


    if(settings.neighberhood == Neighberhoods::NEIGHBERHOOD26){

        // x = 0 - axis movement

        addNeighbour(state.loc - moveY - moveZ);
        addNeighbour(state.loc + moveY + moveZ);
        addNeighbour(state.loc - moveY + moveZ);
        addNeighbour(state.loc + moveY - moveZ);

        // y = 0 - axis movement

        addNeighbour(state.loc - moveX - moveZ);
        addNeighbour(state.loc - moveX + moveZ);
        addNeighbour(state.loc + moveX - moveZ);
        addNeighbour(state.loc + moveX + moveZ);

        // z = 0 - axis movement

        addNeighbour(state.loc + moveX - moveY);
        addNeighbour(state.loc + moveX + moveY);
        addNeighbour(state.loc - moveX - moveY);
        addNeighbour(state.loc - moveX + moveY);

        // x,y,z diagonal

        addNeighbour(state.loc + moveX - moveY - moveZ);
        addNeighbour(state.loc + moveX - moveY + moveZ);

        addNeighbour(state.loc - moveX - moveY - moveZ);
        addNeighbour(state.loc - moveX - moveY + moveZ);

        addNeighbour(state.loc - moveX + moveY - moveZ);
        addNeighbour(state.loc - moveX + moveY + moveZ);

        addNeighbour(state.loc + moveX + moveY - moveZ);
        addNeighbour(state.loc + moveX + moveY + moveZ);

    }
    return neighbours;
}

// manhatan distance
float ISearchAlgorithm::manhattanDistance6n(const QVector3D &loc1,const QVector3D &loc2){
    float dx = abs(loc1.x() - loc2.x());
    float dy = abs(loc1.y() - loc2.y());
    float dz = abs(loc1.z() - loc2.z());

    return straghtMove * (dx+dy+dz);
}

float ISearchAlgorithm::euclidianDistance26n(const QVector3D &loc1,const QVector3D &loc2){
    float dx = abs(loc1.x() - loc2.x());
    float dy = abs(loc1.y() - loc2.y());
    float dz = abs(loc1.z() - loc2.z());

    return sqrt(dx*dx + dy*dy + dz*dz);
}


float ISearchAlgorithm::heuristic(QVector3D loc1, QVector3D loc2)
{
    if(settings.neighberhood == Neighberhoods::NEIGHBERHOOD6){
        return manhattanDistance6n(loc1,loc2);
    }
    else{
        return euclidianDistance26n(loc1,loc2);
    }
}



namespace std {
template<>
struct hash<ISearchAlgorithm::State>{
    size_t operator()(ISearchAlgorithm::State const & state) const noexcept{
        return qHash( QString( "%1x%2x%3" ).arg(state.loc.x()).arg(state.loc.y()).arg(state.loc.z())) ;
    }
};
}

#endif // ISEARCHALGORITHM_H
