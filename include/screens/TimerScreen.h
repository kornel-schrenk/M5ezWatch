#ifndef TimerScreen_h
#define TimerScreen_h

#include "Arduino.h"
#include "M5ez.h"
#include "ezTime.h"

#include "screens/M5ezWatchScreen.h"

class TimerScreen : public M5ezWatchScreen {

    public:
        void initTimerScreen();
        void displayTimer();

        void checkAndFireTimer(int currentScreen);
                        
        bool isRunning();        

        void handleButtonPress(String buttonName);

    private:
        bool _isTimerRunning = false;
        unsigned long _timerStartTimestamp;
        unsigned long _timerElapsedTime = 0;
        int _timerIntervalHours = 0;
        int _timerIntervalMinutes = 1;
        int _timerIntervalSeconds = 0;

        void displayZeroTimerTime();
        void displayElapsedTimerTime();        

        void startTimer();
        void stopTimer();

        int getTimerIntervalHours();
        void setTimerIntervalHours(int hours);
        int getTimerIntervalMinutes();
        void setTimerIntervalMinutes(int minutes);
        int getTimerIntervalSeconds();
        void setTimerIntervalSeconds(int seconds);        
};

#endif