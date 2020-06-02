#ifndef StopwatchScreen_h
#define StopwatchScreen_h

#include "Arduino.h"
#include "M5ez.h"
#include "ezTime.h"

class StopwatchScreen {

    public:
        void displayZeroTime();
        void displayActualTime();
        void initStopwatchScreen();
        void startStopwatch();
        void stopStopwatch();
        void resetStopwatch();
        void displayStopwatch();
        bool isRunning();    

    private:
        bool _isStopWatchRunning = false;
        unsigned long _stopwatchStartTimestamp;
        unsigned long _stopwatchElapsedTime = 0;
};

#endif