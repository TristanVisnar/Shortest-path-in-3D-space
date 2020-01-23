#include "settings.h"

Settings::Settings()
{
    neighberhood = Neighberhoods::NEIGHBERHOOD6;
    runMode = RunModes::NORMAL;
}

Settings::Settings(const Settings &settings)
{
    this->runMode = settings.runMode;
    this->neighberhood = settings.neighberhood;
    this->scene = *new Scene(settings.scene);
    this->_isSettingsSet = true;
}

Settings::~Settings()
{

}

QString Settings::getNeighberhoodName()
{
    switch (neighberhood) {
    case Neighberhoods::NEIGHBERHOOD6:
        return "6-sosedstvo";
    case Neighberhoods::NEIGHBERHOOD26:
        return "26-sosedstvo";
    default:
        return "";
    }
}

QString Settings::getRunModeName()
{
    switch (runMode) {
    case RunModes::NORMAL:
        return "Normalno";
    case RunModes::HIDDEN:
        return "S skrivnimi ovirami";
    }
}

