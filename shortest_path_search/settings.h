#ifndef SETTINGS_H
#define SETTINGS_H

#include "scene.h"

namespace Neighberhoods{
    enum Neighberhoods{NEIGHBERHOOD6, NEIGHBERHOOD26};
}
namespace RunModes {
    enum RunModes{NORMAL, HIDDEN};
}

class Settings
{
public:
    Settings();
    Settings(const Settings &settings);
    ~Settings();

    bool isResultView(){return _isResultView;}
    bool isSettingsSet(){return _isSettingsSet;}

    QString getNeighberhoodName();
    QString getRunModeName();

    Scene scene;
    Neighberhoods::Neighberhoods neighberhood;
    RunModes::RunModes runMode;


protected:
    bool _isResultView = false;
    bool _isSettingsSet = false;

};

#endif // SETTINGS_H
