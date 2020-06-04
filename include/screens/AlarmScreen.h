#ifndef AlarmScreen_h
#define AlarmScreen_h

#include "Arduino.h"
#include "M5ez.h"
#include "ezTime.h"

#include "screens/M5ezWatchScreen.h"

class AlarmScreen : public M5ezWatchScreen {

    public:
        void initAlarmScreen();
        
        void checkAndFireAlarm();
        bool isRunning();

        void setInitialAlarmTime(time_t alarmTime);
    private:
        bool _isAlarmRunning = false;
        time_t _alarmTime;

};

#endif