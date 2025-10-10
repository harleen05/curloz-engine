#include "global/globalDisplay.h"
#include "global/globalConfig.h"

int global::Display::getWidth()
{
        auto config = global::Config::getConfig();
        return config["display"]["width"].value_or(1280);
}
int global::Display::getheight()
{
        auto config = global::Config::getConfig();
        return config["display"]["width"].value_or(720);
}
bool global::Display::VSync() 
{
        auto config = global::Config::getConfig();
        return config["display"]["vsync"].value_or(true);
}
int global::Display::getRefreshRate()
{
        auto config = global::Config::getConfig();
        return config["display"]["refreshrate"].value_or(60);
}

void global::Display::saveConfig()
{

}
