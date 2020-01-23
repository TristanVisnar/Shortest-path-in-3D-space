#ifndef DSTARLITE_H
#define DSTARLITE_H

#include "isearchalgorithm.h"
#include <queue>
#include "math.h"
#include <list>
#include <unordered_map>
#include <iostream>


//using namespace __gnu_cxx;

class state{
public:
    int x,y,z;
    std::pair<double,double> k;

    bool operator == (const state &s2) const {
      return (x == s2.x)&&(y==s2.y) && (z==s2.z);
    }

    bool operator != (const state &s2) const {
      return (x != s2.x) || (y != s2.y) || (z != s2.z);
    }

    bool operator > (const state &s2) const {
      if (k.first-0.00001 > s2.k.first) return true;
      else if (k.first < s2.k.first-0.00001) return false;
      return k.second > s2.k.second;
    }

    bool operator <= (const state &s2) const {
      if (k.first < s2.k.first) return true;
      else if (k.first > s2.k.first) return false;
      return k.second < s2.k.second + 0.00001;
    }


    bool operator < (const state &s2) const {
      if (k.first + 0.000001 < s2.k.first) return true;
      else if (k.first - 0.000001 > s2.k.first) return false;
      return k.second < s2.k.second;
    }
};


struct cellInfo {
    double g = INFINITY;
    double rhs = INFINITY;
    double cost = 1;
};

class state_hash {
 public:
  size_t operator()(const state &s) const {
    return static_cast<size_t>(s.x) + 34245* static_cast<size_t>(s.y) + 4123213 * static_cast<size_t>(s.z);
  }
};

class DStarLite : public ISearchAlgorithm{
public:
    DStarLite(const Settings &settings): ISearchAlgorithm(settings) {}

    void run(Result& result);
    QString getName(){return "D* Lite";}
private:

    std::list<state> path;

    // Stores statest needed to research
    std::priority_queue<state, std::vector<state>, std::greater<state> > openList;
    // Stores info for state info
    std::unordered_map<state, cellInfo, state_hash, std::equal_to<state> > cellHash; //mapping state to its info with hash
    // Stores key hash of a state
    std::unordered_map<state, float, state_hash, std::equal_to<state> > openHash; //mapping state to key hash

    double C1 = 1;
    double k_m = 0;
    state org_start, org_goal;
    state s_start, s_goal, s_last;

    //
    void   init();

    //
    void   makeNewCell(state u);

    //
    double getG(state u);
    double getRHS(state u);
    double getCost(state u);

    void   setG(state u, double g);
    double setRHS(state u, double rhs);
    void   setCost(state u, double cost);

    //
    void   getSucc(state u, std::list<state> &s);
    void   getPred(state u, std::list<state> &s);


    // If numbers are in range 10E-5
    bool   close(double x, double y);
    //
    double heuristic(state a, state b); //reimplement
    //add heuristic methods for 6n and 26n

    double eightCondist(state a, state b);


    int    computeShortestPath(Result &result);
    void   updateVertex(state u);
    void   insert(state u);
    void   remove(state u);
    double trueDist(state a, state b);

    state  calculateKey(state u);


    double cost(state a, state b); //reimplement
    bool   occupied(state u);
    bool   isValid(state u);
    float  keyHashCode(state u);

    // my implementation
    void Initialize();
    void UpdateVertex(state u);
    void ComputeShortestPath();
    void Main();
};


void DStarLite::init()
{
    // clear all data
    cellHash.clear();
    path.clear();
    openHash.clear();
    while(!openList.empty()) openList.pop();

    k_m = 0;

    org_start.x = static_cast<int>(settings.scene.startPoint.x());
    org_start.y = static_cast<int>(settings.scene.startPoint.y());
    org_start.z = static_cast<int>(settings.scene.startPoint.z());
    org_goal.x = static_cast<int>(settings.scene.endPoint.x());
    org_goal.y = static_cast<int>(settings.scene.endPoint.y());
    org_goal.z = static_cast<int>(settings.scene.endPoint.z());

    s_start = org_start;
    s_goal = org_goal;

    // goal settings
    cellInfo tmp;
    tmp.g =    INFINITY;
    tmp.rhs =  0;
    tmp.cost = C1;
    cellHash[s_goal] = tmp;
    s_goal = calculateKey(s_goal);

    // start settings
    tmp.g = tmp.rhs = INFINITY;
    tmp.cost = C1;
    cellHash[s_start] = tmp;
    s_start = calculateKey(s_start);

    insert(s_goal);

}



int DStarLite::computeShortestPath(Result& result)
{
    std::list<state> s;
    std::list<state>::iterator i;

    if (openList.empty()) return 1;

    while ((!openList.empty()) &&
           ((openList.top() < (s_start = calculateKey(s_start))) ||
           (getRHS(s_start) != getG(s_start)))) {

      state u;

      bool test = (getRHS(s_start) != getG(s_start));

      // lazy remove
      while(1) {
        if (openList.empty()) return 1;
        u = openList.top();
        openList.pop();

        if (!isValid(u)) continue;
        if (!(u < s_start) && (!test)) return 2;
        break;
      }

      std::unordered_map<state, float, state_hash, std::equal_to<state>>::iterator cur = openHash.find(u);
      openHash.erase(cur);

      result.addToSteps(*new QVector3D(u.x,u.y,u.z));

      state k_old = u;

      if (k_old < calculateKey(u)) { // u is out of date
        insert(u);
      } else if (getG(u) > getRHS(u)) { // needs update (got better)
        setG(u,getRHS(u));
        getPred(u,s);
        for (i=s.begin();i != s.end(); i++) {
          updateVertex(*i);
        }
      } else {   // g <= rhs, state has got worse
        setG(u,INFINITY);
        getPred(u,s);
        for (i=s.begin();i != s.end(); i++) {
          updateVertex(*i);
        }
        updateVertex(u);
      }
    }
    return 0;
}

void DStarLite::run(Result &result)
{
    result.algorithmName = getName();

    QElapsedTimer timer;
    timer.start();

    result.paths.clear();
    result.paths.append(*new QVector<QVector3D>());

    init();
    s_last = s_start;


    int res = computeShortestPath(result);
    //printf("res: %d ols: %d ohs: %d tk: [%f %f] sk: [%f %f] sgr: (%f,%f)\n",res,openList.size(),openHash.size(),openList.top().k.first,openList.top().k.second, s_start.k.first, s_start.k.second,getRHS(s_start),getG(s_start));
    if (res < 0) {
      fprintf(stderr, "NO PATH TO GOAL\n");
      return;
    }
    std::list<state> n;
    std::list<state>::iterator i;


    if (isinf(getG(s_start))) {
      fprintf(stderr, "NO PATH TO GOAL\n");
      return;
    }

    result.paths.last().append(QVector3D(s_start.x,s_start.y,s_start.z));

    while(s_start != s_goal) {

        if(s_start != s_last)
            result.paths.last().append(QVector3D(s_start.x,s_start.y,s_start.z));


          // finds cheapest succesor
          getSucc(s_start, n);

          if (n.empty()) {
            fprintf(stderr, "NO PATH TO GOAL\n");
            return;
          }

          double cmin = INFINITY;
          double tmin;
          state smin;

          for (i=n.begin(); i!=n.end(); i++) {

            //if (occupied(*i)) continue;
            double val  = cost(s_start,*i);
            double val2 = heuristic(*i,s_goal) + heuristic(s_start,*i);
            val += getG(*i);

            if (close(val,cmin)) {
              if (tmin > val2) {
                tmin = val2;
                cmin = val;
                smin = *i;
              }
            } else if (val < cmin) {
              tmin = val2;
              cmin = val;
              smin = *i;
            }
          }
          n.clear();

          if(settings.runMode == RunModes::HIDDEN){
              // if next in path is a hidden wall == edge cost change
              if(settings.scene.voxelSpace(smin.x,smin.y,smin.z).getVoxelType() == VoxelType::HIDDEN_WALL){
                  settings.scene.voxelSpace(smin.x,smin.y,smin.z).setVoxelType(VoxelType::VISIBLE_HIDDEN_WALL);
                  k_m += heuristic(s_last, s_start);

                  s_start = calculateKey(s_start);
                  s_last = s_start;
                  updateVertex(s_start);

                  //
                  getSucc(smin, n);
                  for (i=n.begin(); i!=n.end(); i++) {
                      updateVertex(*i);
                  }



                  res = computeShortestPath(result);
                  //printf("res: %d ols: %d ohs: %d tk: [%f %f] sk: [%f %f] sgr: (%f,%f)\n",res,openList.size(),openHash.size(),openList.top().k.first,openList.top().k.second, s_start.k.first, s_start.k.second,getRHS(s_start),getG(s_start));
                  if (res < 0) {
                    fprintf(stderr, "NO PATH TO GOAL\n");
                    return;
                  }

                  if (isinf(getG(s_start))) {
                    fprintf(stderr, "NO PATH TO GOAL\n");
                    return;
                  }

                  continue;
              }
          }



          s_start = smin;

    }
    result.paths.last().append(QVector3D(s_goal.x,s_goal.y,s_goal.z));
    result.runTime = timer.elapsed();
    result.setSettings(settings);
    return;
}



/**************************************
 *  Gets/sets and new state creation  *
 **************************************/
void DStarLite::makeNewCell(state u)
{
    if (cellHash.find(u) != cellHash.end()) return;

    cellInfo tmp;
    tmp.g       = tmp.rhs = heuristic(u,s_goal);
    tmp.cost    = settings.scene.voxelSpace(u.x,u.y,u.z).getWeight();
    cellHash[u] = tmp;
}

double DStarLite::getG(state u)
{
    if (cellHash.find(u) == cellHash.end())
      return heuristic(u,s_goal);
    return cellHash[u].g;

}

double DStarLite::getRHS(state u)
{
    if (u == s_goal) return 0;

    if (cellHash.find(u) == cellHash.end())
      return heuristic(u,s_goal);
    return cellHash[u].rhs;
}

double DStarLite::getCost(state u)
{
    if (cellHash.find(u) == cellHash.end())
      return -1;
    return cellHash[u].cost;
}

void DStarLite::setG(state u, double g)
{
    makeNewCell(u);
    cellHash[u].g = g;
}

double DStarLite::setRHS(state u, double rhs)
{
    makeNewCell(u);
    cellHash[u].rhs = rhs;
}

void DStarLite::setCost(state u, double cost)
{
    makeNewCell(u);
    cellHash[u].cost = cost;
}



/**********************************
 * Gets succesors and predecesors *
 **********************************/

// from a to b
double DStarLite::cost(state a, state b)
{
    int xd = abs(a.x-b.x);
    int yd = abs(a.y-b.y);
    int zd = abs(a.z-b.z);
    double scale = 1;


    if (xd+yd+zd>2) scale = diagonalMove3d;
    else if(xd+yd+zd>1) scale = diagonalMove2d;
    else scale = straghtMove;

    if (cellHash.count(b) == 0) return scale;
    return scale * cellHash[b].cost;
}


void DStarLite::getSucc(state u, std::list<state> &s)
{
    s.clear();
    u.k.first = -1;
    u.k.second = -1;

    //if (occupied(u)) return;

    auto add = [&](state x){if(!occupied(x))s.push_front(x);};

    add({u.x+1,u.y,u.z,u.k}); //(+1, 0, 0)
    add({u.x-1,u.y,u.z,u.k}); //(-1, 0, 0)
    add({u.x,u.y+1,u.z,u.k}); //( 0,+1, 0)
    add({u.x,u.y-1,u.z,u.k}); //( 0,-1, 0)
    add({u.x,u.y,u.z+1,u.k}); //( 0, 0,+1)
    add({u.x,u.y,u.z-1,u.k}); //( 0, 0,-1)

    if(settings.neighberhood == Neighberhoods::NEIGHBERHOOD26){

        // x-axis 0
        add({u.x,u.y+1,u.z-1,u.k});    //( 0,+1,-1)
        add({u.x,u.y+1,u.z+1,u.k});    //( 0,+1,+1)
        add({u.x,u.y-1,u.z-1,u.k});    //( 0,-1,-1)
        add({u.x,u.y-1,u.z+1,u.k});    //( 0,-1,+1)

        // y-axis 0
        add({u.x+1,u.y,u.z-1,u.k});    //(+1, 0,-1)
        add({u.x+1,u.y,u.z+1,u.k});    //(+1, 0,+1)
        add({u.x-1,u.y,u.z-1,u.k});    //(-1, 0,-1)
        add({u.x-1,u.y,u.z+1,u.k});    //(-1, 0,+1)

        // z-axis 0
        add({u.x+1,u.y+1,u.z,u.k});    //(+1,+1, 0)
        add({u.x+1,u.y-1,u.z,u.k});    //(+1,-1, 0)
        add({u.x-1,u.y+1,u.z,u.k});    //(-1,+1, 0)
        add({u.x-1,u.y-1,u.z,u.k});    //(-1,-1, 0)

        //3d diagonals
        add({u.x+1,u.y+1,u.z+1,u.k});  //(+1,+1,+1)
        add({u.x+1,u.y+1,u.z-1,u.k});  //(+1,+1,-1)
        add({u.x+1,u.y-1,u.z+1,u.k});  //(+1,-1,+1)
        add({u.x+1,u.y-1,u.z-1,u.k});  //(+1,-1,-1)
        add({u.x-1,u.y+1,u.z+1,u.k});  //(-1,+1,+1)
        add({u.x-1,u.y+1,u.z-1,u.k});  //(-1,+1,-1)
        add({u.x-1,u.y-1,u.z+1,u.k});  //(-1,-1,+1)
        add({u.x-1,u.y-1,u.z-1,u.k});  //(-1,-1,-1)
    }

}



void DStarLite::getPred(state u, std::list<state> &s)
{
    s.clear();
    u.k.first  = -1;
    u.k.second = -1;

    auto add = [&](state x){if(!occupied(x))s.push_front(x);};

    add({u.x+1,u.y,u.z,u.k}); //(+1, 0, 0)
    add({u.x-1,u.y,u.z,u.k}); //(-1, 0, 0)
    add({u.x,u.y+1,u.z,u.k}); //( 0,+1, 0)
    add({u.x,u.y-1,u.z,u.k}); //( 0,-1, 0)
    add({u.x,u.y,u.z+1,u.k}); //( 0, 0,+1)
    add({u.x,u.y,u.z-1,u.k}); //( 0, 0,-1)

    if(settings.neighberhood == Neighberhoods::NEIGHBERHOOD26){

        // x-axis 0
        add({u.x,u.y+1,u.z-1,u.k});    //( 0,+1,-1)
        add({u.x,u.y+1,u.z+1,u.k});    //( 0,+1,+1)
        add({u.x,u.y-1,u.z-1,u.k});    //( 0,-1,-1)
        add({u.x,u.y-1,u.z+1,u.k});    //( 0,-1,+1)

        // y-axis 0
        add({u.x+1,u.y,u.z-1,u.k});    //(+1, 0,-1)
        add({u.x+1,u.y,u.z+1,u.k});    //(+1, 0,+1)
        add({u.x-1,u.y,u.z-1,u.k});    //(-1, 0,-1)
        add({u.x-1,u.y,u.z+1,u.k});    //(-1, 0,+1)

        // z-axis 0
        add({u.x+1,u.y+1,u.z,u.k});    //(+1,+1, 0)
        add({u.x+1,u.y-1,u.z,u.k});    //(+1,-1, 0)
        add({u.x-1,u.y+1,u.z,u.k});    //(-1,+1, 0)
        add({u.x-1,u.y-1,u.z,u.k});    //(-1,-1, 0)

        //3d diagonals
        add({u.x+1,u.y+1,u.z+1,u.k});  //(+1,+1,+1)
        add({u.x+1,u.y+1,u.z-1,u.k});  //(+1,+1,-1)
        add({u.x+1,u.y-1,u.z+1,u.k});  //(+1,-1,+1)
        add({u.x+1,u.y-1,u.z-1,u.k});  //(+1,-1,-1)
        add({u.x-1,u.y+1,u.z+1,u.k});  //(-1,+1,+1)
        add({u.x-1,u.y+1,u.z-1,u.k});  //(-1,+1,-1)
        add({u.x-1,u.y-1,u.z+1,u.k});  //(-1,-1,+1)
        add({u.x-1,u.y-1,u.z-1,u.k});  //(-1,-1,-1)
    }
}

// Returns true if x and y are within 10E-5, false otherwise
bool DStarLite::close(double x, double y) {

  if (isinf(x) && isinf(y)) return true;
  return (fabs(x-y) < 0.00001);
}


/**************
 * Heuristics *
 **************/

double DStarLite::trueDist(state a, state b)
{
    long long x = abs(a.x - b.x);
    long long y = abs(a.y - b.y);
    long long z = abs(a.z - b.z);
    return sqrt(x*x + y*y + z*z);
}

double DStarLite::heuristic(state a, state b)
{
    return ISearchAlgorithm::heuristic(QVector3D(a.x,a.y,a.z),QVector3D(b.x,b.y,b.z));
}

/*********************
 * D* Main functions *
 *********************/

state DStarLite::calculateKey(state u)
{
    double val = fmin(getRHS(u),getG(u));

    u.k.first  = val + heuristic(u,s_start) + k_m;
    u.k.second = val;

    return u;
}



void DStarLite::updateVertex(state u)
{
    std::list<state> s;
    std::list<state>::iterator i;

    if (u != s_goal) {
      getSucc(u,s);
      double tmp = INFINITY;
      double tmp2;

      for (i=s.begin();i != s.end(); i++) {
        tmp2 = getG(*i) + cost(u,*i);
        if (tmp2 < tmp) tmp = tmp2;
      }
      if (!close(getRHS(u),tmp)) setRHS(u,tmp);
    }

    if (!close(getG(u),getRHS(u))) insert(u);

}






/***********
 * Helpers *
 ***********/


// Returns the key hash code for the state u, this is used to compare
// a state that have been updated
float DStarLite::keyHashCode(state u) {

 return (float)(u.k.first + 1193*u.k.second);

}

void DStarLite::insert(state u)
{
    std::unordered_map<state, float, state_hash, std::equal_to<state>>::iterator cur;
    float csum;

    u    = calculateKey(u);
    cur  = openHash.find(u);
    csum = keyHashCode(u);


    openHash[u] = csum;
    openList.push(u);

}

void DStarLite::remove(state u)
{
    std::unordered_map<state, float, state_hash, std::equal_to<state>>::iterator cur = openHash.find(u);
    if (cur == openHash.end()) return;
    openHash.erase(cur);
}

// returns true if the cell is occupied (non-traversable), false
// otherwise. non-traversable are marked with a cost < 0.
bool DStarLite::occupied(state u)
{
    //check if u in bounds
    if(u.x < 0 || u.x >= settings.scene.voxelSpace.getXSize()) return true;
    if(u.y < 0 || u.y >= settings.scene.voxelSpace.getYSize()) return true;
    if(u.z < 0 || u.z >= settings.scene.voxelSpace.getZSize()) return true;

    //check if u is wall
    if(settings.scene.voxelSpace(u.x,u.y,u.z).getVoxelType() == VoxelType::WALL) return true;
    if(settings.scene.voxelSpace(u.x,u.y,u.z).getVoxelType() == VoxelType::VISIBLE_HIDDEN_WALL) return true;

    return false;
}

// Returns true if state u is on the open list or not by checking if
// it is in the hash table.
bool DStarLite::isValid(state u) {
    std::unordered_map<state, float, state_hash, std::equal_to<state> >::iterator cur = openHash.find(u);
    if (cur == openHash.end()) return false;
    if (!close(keyHashCode(u), cur->second)) return false; //if selected cur does not equal to hash of the key of u
    return true;
}


#endif // DSTARLITE_H
