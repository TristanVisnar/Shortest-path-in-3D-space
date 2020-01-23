#ifndef ASTAR_H
#define ASTAR_H

#include "isearchalgorithm.h"
#include <unordered_set>
#include <QVector3D>
#include <queue>
#include <utility>
#include <iterator>
#include <QElapsedTimer>
#include "math.h"

class AStar : public ISearchAlgorithm{
public:
    AStar(const Settings &settings): ISearchAlgorithm(settings) {}

    void run(Result & result);
    QString getName(){return "A*";}

private:
    void runAlgorithm(Result & result);
    float h(const QVector3D& loc);
};

struct AState : public ISearchAlgorithm::State{
    float fVal;


    AState(float gVal, QVector3D loc, AState* aState, float fVal):ISearchAlgorithm::State (gVal,loc,aState), fVal(fVal){};
    AState(const State& state,float fVal):State(state),fVal(fVal) {}

    bool operator< (const AState& st){
        return fVal > st.fVal;
    }

    friend bool operator< (const AState& l,const AState& r){
        return l.fVal > r.fVal;
    }

};

namespace std {
template<>
struct hash<AState>{
    size_t operator()(AState const & state) const noexcept{
        return qHash( QString( "%1x%2x%3" ).arg(state.loc.x()).arg(state.loc.y()).arg(state.loc.z())) ;
    }
};
}

void AStar::run(Result &result){
    result.algorithmName = getName();
    QElapsedTimer timer;
    timer.start();

    if(settings.runMode == RunModes::NORMAL){
        result.createNewPath();
        runAlgorithm(result);
    }else{
    // with hidden walls
        bool isPathBlocked = true;

        while(isPathBlocked){
            result.createNewPath();
            runAlgorithm(result);
            isPathBlocked = false;
            for(auto state: result.paths.last()){
                if(settings.scene.voxelSpace(state.x(),state.y(),state.z()).getVoxelType() == VoxelType::HIDDEN_WALL){
                    settings.scene.voxelSpace(state.x(),state.y(),state.z()).setVoxelType(VoxelType::VISIBLE_HIDDEN_WALL);
                    isPathBlocked = true;
                    break;
                }
            }
        }
    }
    result.runTime = timer.elapsed();
    result.setSettings(settings);
    return;
}

void AStar::runAlgorithm(Result & result)
{
    std::unordered_set<AState,std::hash<AState>> S;
    std::priority_queue<AState> Q;
    Q.push({0,settings.scene.startPoint,nullptr,h(settings.scene.startPoint)});
    std::pair<std::unordered_set<AState>::iterator,bool> el;
    while(!Q.empty()){
        if(Q.top().loc == settings.scene.endPoint){
            el = S.emplace(Q.top());
            break; // found the path
        }
        if(S.find(Q.top()) != S.end() ){ // already exists in S
            Q.pop();
            continue;
        }
        el = S.emplace(Q.top());
        Q.pop();
        QVector3D currentLoc(el.first->loc);
        result.addToSteps(currentLoc);

        auto neighbours = getNeighbours(*el.first);

        for (auto neighbour:neighbours) {
            Q.push({neighbour, neighbour.gVal + h(neighbour.loc)});
        }
    }
    // go from last to first element and push front on path
    const AState *currentState = &(*el.first);

    while (currentState != nullptr) {
       QVector3D pathStateLocation(currentState->loc);
       result.addToCurrentPath(pathStateLocation);
       currentState = static_cast<const AState*>(currentState->prevState);
    }

}

float AStar::h(const QVector3D& loc)
{
    return heuristic(loc,settings.scene.endPoint);
}

#endif // ASTAR_H
