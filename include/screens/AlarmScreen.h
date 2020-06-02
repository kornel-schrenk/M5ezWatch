#ifndef AlarmScreen_h
#define AlarmScreen_h

#include "Arduino.h"
#include "M5ez.h"
#include "ezTime.h"

class AlarmScreen {

    public:
        void initAlarmScreen();
        void checkAndFireAlarm();
        bool isRunning();

    private:
        bool _isAlarmRunning = false;
        time_t _alarmTime = now();

};

#endif