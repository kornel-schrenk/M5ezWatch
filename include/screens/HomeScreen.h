#ifndef HomeScreen_h
#define HomeScreen_h

#include "Arduino.h"
#include "M5ez.h"
#include "ezTime.h"

#include "images/jpgs.h"
#include "images/jpgsdark.h"

#include "screens/M5ezWatchScreen.h"
#include "screens/StopwatchScreen.h"
#include "screens/AlarmScreen.h"
#include "screens/TimerScreen.h"

class HomeScreen : public M5ezWatchScreen {

    public:
        void initHomeScreen(StopwatchScreen* stopwatchScreen, AlarmScreen* alarmScreen, TimerScreen* timerScreen);
        
        void displayHomeClock(AlarmScreen* alarmScreen, TimerScreen* timerScreen);
        void updateTime();
        void updateDate();
        void updateAmPm();
        
        bool isMinimalModeActive();

        void handleButtonPress(String buttonName, StopwatchScreen* stopwatchScreen, AlarmScreen* alarmScreen, TimerScreen* timerScreen);
        
        String getTimezoneLocation();
        void storeTimeInRtc();

    private:
        bool _isMinimalModeActive = false;
};

#endif