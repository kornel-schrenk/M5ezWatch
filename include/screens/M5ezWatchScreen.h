#ifndef M5ezWatchScreen_h
#define M5ezWatchScreen_h

#include "Arduino.h"
#include "M5ez.h"

#define SCREEN_HOME       210
#define SCREEN_STOPWATCH  220
#define SCREEN_ALARM      230
#define SCREEN_TIMER      240
#define SCREEN_SETTINGS   260

class M5ezWatchScreen {

    public: 
        void refreshClockWidget();
        bool isClockWidgetDisplayed();
    
    private:
};

#endif