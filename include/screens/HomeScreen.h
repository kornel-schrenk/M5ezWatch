#ifndef HomeScreen_h
#define HomeScreen_h

#include "Arduino.h"
#include "M5ez.h"
#include "ezTime.h"

#include "images/jpgs.h"
#include "images/jpgsdark.h"

#include "screens/StopwatchScreen.h"
#include "screens/AlarmScreen.h"
#include "screens/TimerScreen.h"

class HomeScreen {

    public:
        void updateTime();
        void updateDate();
        void updateAmPm();
        
        void initHomeScreen(StopwatchScreen* stopwatchScreen, AlarmScreen* alarmScreen, TimerScreen* timerScreen);
        void displayHomeClock(AlarmScreen* alarmScreen, TimerScreen* timerScreen);
    private:
        void _refreshClockWidget();
        bool _isClockWidgetDisplayed();
};


#endif