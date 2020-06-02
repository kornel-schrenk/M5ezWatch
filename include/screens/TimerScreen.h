#ifndef TimerScreen_h
#define TimerScreen_h

#include "Arduino.h"
#include "M5ez.h"
#include "ezTime.h"

class TimerScreen {

    public:
        void displayZeroTimerTime();
        void checkAndFireTimer(int currentScreen);
        void displayElapsedTimerTime();
        void initTimerScreen();
        void displayTimer();
        void startTimer();
        void stopTimer();
        bool isRunning();
        
        int getTimerIntervalHours();
        void setTimerIntervalHours(int hours);
        int getTimerIntervalMinutes();
        void setTimerIntervalMinutes(int minutes);
        int getTimerIntervalSeconds();
        void setTimerIntervalSeconds(int seconds);

    private:
        bool _isTimerRunning = false;
        unsigned long _timerStartTimestamp;
        unsigned long _timerElapsedTime = 0;
        int _timerIntervalHours = 0;
        int _timerIntervalMinutes = 1;
        int _timerIntervalSeconds = 0;
};

#endif