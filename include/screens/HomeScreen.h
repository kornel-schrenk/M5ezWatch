#ifndef HomeScreen_h
#define HomeScreen_h

#include "Arduino.h"
#include "M5ez.h"
#include "ezTime.h"
#include "Unit_RTC.h"

#include "images/jpgs.h"
#include "images/jpgsdark.h"

#include "screens/M5ezWatchScreen.h"
#include "screens/StopwatchScreen.h"
#include "screens/AlarmScreen.h"
#include "screens/TimerScreen.h"

class HomeScreen : public M5ezWatchScreen {

    public:
        void initHomeScreen(Unit_RTC* rtc, StopwatchScreen* stopwatchScreen, AlarmScreen* alarmScreen, TimerScreen* timerScreen);
        void handleButtonPress(String buttonName, StopwatchScreen* stopwatchScreen, AlarmScreen* alarmScreen, TimerScreen* timerScreen); 
        
        void displayHomeClock(AlarmScreen* alarmScreen, TimerScreen* timerScreen);        
        bool isMinimalModeActive();               

    private:
        bool _isMinimalModeActive = false;
        Unit_RTC* _rtc;

        void updateTime();
        void updateDate();
        void updateAmPm();

        String getTimezoneLocation();
        void storeTimeInRtc();        
};

#endif