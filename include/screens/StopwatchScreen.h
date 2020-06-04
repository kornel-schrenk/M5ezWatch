#ifndef StopwatchScreen_h
#define StopwatchScreen_h

#include "Arduino.h"
#include "M5ez.h"
#include "ezTime.h"

#include "screens/M5ezWatchScreen.h"

class StopwatchScreen : public M5ezWatchScreen {

    public:
        void initStopwatchScreen();
        
        void displayStopwatch();
        void displayZeroTime();
        void displayActualTime();
        
        void startStopwatch();
        void stopStopwatch();
        void resetStopwatch();
        
        bool isRunning();    

        void handleButtonPress(String buttonName);

    private:
        bool _isStopWatchRunning = false;
        unsigned long _stopwatchStartTimestamp;
        unsigned long _stopwatchElapsedTime = 0;
};

#endif