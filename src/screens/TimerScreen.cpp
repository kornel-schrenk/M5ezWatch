#include "screens/TimerScreen.h"
#include "screens/Screens.h"
#include "pickers/DateTimePicker.h"

void TimerScreen::displayZeroTimerTime()
{
    ez.canvas.color(ez.theme->foreground);
    ez.canvas.font(numonly7seg48);
    ez.canvas.pos(56, 150);

    ez.canvas.print(zeropad(0, 2));
    ez.canvas.print(":");
    ez.canvas.print(zeropad(0, 2));
    ez.canvas.print(":");
    ez.canvas.print(zeropad(0, 2));

    M5.Lcd.fillRect(10, 100, 300, 25, ez.theme->background);
    M5.Lcd.progressBar(10, 100, 300, 25, 0);
}

void TimerScreen::checkAndFireTimer(int currentScreen)
{
    if (_isTimerRunning)
    {
        unsigned long elapsedTimeInSeconds = now() - _timerStartTimestamp;

        int elapsedHours = elapsedTimeInSeconds / 3600;
        int elapsedMinutes = elapsedTimeInSeconds % 3600 / 60;
        int elapsedSeconds = elapsedTimeInSeconds % 3600 % 60;

        if (elapsedHours == _timerIntervalHours && elapsedMinutes == _timerIntervalMinutes && elapsedSeconds == _timerIntervalSeconds)
        {
            // Play a buzz
            M5.Speaker.tone(900, 300);

            // Restart the timer
            _timerStartTimestamp = now();

            if (currentScreen == SCREEN_TIMER)
            {
                // Display zero time
                this->displayZeroTimerTime();
            }
        }
    }
}

void TimerScreen::displayElapsedTimerTime()
{
    int timerIntervalInSeconds = _timerIntervalHours * 3600 + _timerIntervalMinutes * 60 + _timerIntervalSeconds;

    unsigned long elapsedTimeInSeconds = now() - _timerStartTimestamp;

    // Fix times, if timer overran on non-timer and non-home screens
    if (elapsedTimeInSeconds > timerIntervalInSeconds) {
        int remainingSeconds = elapsedTimeInSeconds % timerIntervalInSeconds;
        elapsedTimeInSeconds = remainingSeconds;
        _timerStartTimestamp = now() - remainingSeconds;
    }

    int hours = elapsedTimeInSeconds / 3600;
    int minutes = elapsedTimeInSeconds % 3600 / 60;
    int seconds = elapsedTimeInSeconds % 3600 % 60;

    ez.canvas.color(ez.theme->foreground);
    ez.canvas.font(numonly7seg48);
    ez.canvas.pos(56, 150);

    ez.canvas.print(zeropad(hours, 2));
    ez.canvas.print(":");
    ez.canvas.print(zeropad(minutes, 2));
    ez.canvas.print(":");
    ez.canvas.print(zeropad(seconds, 2));

    // Calculate elapsed time %    
    uint8_t elapsedPercentage = (elapsedTimeInSeconds * 100) / timerIntervalInSeconds;
    M5.Lcd.progressBar(10, 100, 300, 25, elapsedPercentage);

    this->checkAndFireTimer(SCREEN_TIMER);
}

void TimerScreen::initTimerScreen()
{
    ez.screen.clear();
    ez.header.show("Timer");

    ez.canvas.color(ez.theme->foreground);

    ez.canvas.font(sans26);
    ez.canvas.pos(20, 50);
    ez.canvas.print("Interval: ");
    ez.canvas.pos(200, 50);
    ez.canvas.print(zeropad(_timerIntervalHours, 2));
    ez.canvas.print(":");
    ez.canvas.print(zeropad(_timerIntervalMinutes, 2));
    ez.canvas.print(":");
    ez.canvas.print(zeropad(_timerIntervalSeconds, 2));

    if (_isTimerRunning)
    {
        ez.buttons.show("Stop # Interval # Menu");
        this->displayElapsedTimerTime();
    }
    else
    {
        ez.buttons.show("Start # Interval # Menu");
        this->displayZeroTimerTime();
    }
}

void TimerScreen::displayTimer()
{
    if (minuteChanged()) {
        this->refreshClockWidget();
        this->displayElapsedTimerTime();
    }

    if (secondChanged() && _isTimerRunning)
    {
        this->displayElapsedTimerTime();
    }
}

void TimerScreen::startTimer()
{
    _timerStartTimestamp = now();
    _isTimerRunning = true;
    this->displayZeroTimerTime();
}

void TimerScreen::stopTimer()
{
    _isTimerRunning = false;
}

bool TimerScreen::isRunning()
{
    return _isTimerRunning;
}

int TimerScreen::getTimerIntervalHours()
{
    return _timerIntervalHours;
}

void TimerScreen::setTimerIntervalHours(int hours)
{
    _timerIntervalHours = hours;
}

int TimerScreen::getTimerIntervalMinutes()
{
    return _timerIntervalMinutes;
}

void TimerScreen::setTimerIntervalMinutes(int minutes)
{
    _timerIntervalMinutes = minutes;
}

int TimerScreen::getTimerIntervalSeconds()
{
    return _timerIntervalSeconds;
}

void TimerScreen::setTimerIntervalSeconds(int seconds)
{
    _timerIntervalSeconds = seconds;
}

void TimerScreen::handleButtonPress(String buttonName)
{
    if (buttonName == "Start")
    {
        ez.buttons.show("$Stop # Interval # Menu");
        this->startTimer();
        delay(300);
        ez.buttons.show("Stop # Interval # Menu");
    }
    else if (buttonName == "Stop")
    {
        ez.buttons.show("$Start # Interval # Menu");
        this->stopTimer();
        delay(300);
        ez.buttons.show("Start # Interval # Menu");
    }
    else if (buttonName == "Interval")
    {
        time_t initialTime = now();
        initialTime = makeTime(this->getTimerIntervalHours(), this->getTimerIntervalMinutes(), this->getTimerIntervalSeconds(), day(initialTime), month(initialTime), year(initialTime));
        Serial.println("Initial interval time: " + dateTime(initialTime, "Y-m-d H:i:s"));

        DateTimePicker intervalPicker;
        time_t pickedTime = intervalPicker.runOnce("Interval", initialTime, true, true);

        if (pickedTime != 0)
        {
            this->setTimerIntervalHours(hour(pickedTime));
            this->setTimerIntervalMinutes(minute(pickedTime));
            this->setTimerIntervalSeconds(second(pickedTime));

            Serial.println("Picked interval time: " + dateTime(pickedTime, "Y-m-d H:i:s"));
        }
        this->initTimerScreen();
    }
}