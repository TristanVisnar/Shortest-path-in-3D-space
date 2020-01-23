#ifndef DJIKSTRA_H
#define DJIKSTRA_H

#include "isearchalgorithm.h"
#include <unordered_set>
#include <QVector3D>
#include <queue>
#include <utility>
#include <iterator>
#include <QElapsedTimer>

class Djikstra : public ISearchAlgorithm{
public:
    Djikstra(const Settings &settings): ISearchAlgorithm(settings) {}

    void run(Result& result);
    QString getName(){return "Djikstra";}

private:
    void runAlgorithm(Result & result);
};

void Djikstra::run(Result& result){
    result.algorithmName = getName();
    QElapsedTimer timer;
    timer.start();

    if(settings.runMode == RunModes::NORMAL){
        result.createNewPath();
        runAlgorithm(result);
    }else {
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

void Djikstra::runAlgorithm( Result & result)
{
    std::unordered_set<State,std::hash<State>> S;
    std::priority_queue<State> Q;
    Q.push({0,settings.scene.startPoint,nullptr});
    std::pair<std::unordered_set<State>::iterator,bool> el;
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
        QVector3D currentLoc(el.first->loc) ;
        result.addToSteps(currentLoc);

        auto neighbours = getNeighbours(*el.first);

        for (auto neighbour:neighbours) {
            Q.push(neighbour);
        }
    }
    // go from last to first element and push front on path
    const State *currentState = &(*el.first);

    while (currentState != nullptr) {
       QVector3D pathStateLocation(currentState->loc);
       result.addToCurrentPath(pathStateLocation);
       currentState = currentState->prevState;
    }

}

#endif // DJIKSTRA_H
