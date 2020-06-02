#include "screens/TimerScreen.h"
#include "screens/Screens.h"

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
                displayZeroTimerTime();
            }
        }
    }
}

void TimerScreen::displayElapsedTimerTime()
{
    unsigned long elapsedTimeInSeconds = now() - _timerStartTimestamp;

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
    int timerIntervalInSeconds = _timerIntervalHours * 3600 + _timerIntervalMinutes * 60 + _timerIntervalSeconds;
    uint8_t elapsedPercentage = (elapsedTimeInSeconds * 100) / timerIntervalInSeconds;
    M5.Lcd.progressBar(10, 100, 300, 25, elapsedPercentage);

    checkAndFireTimer(SCREEN_TIMER);
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
        displayElapsedTimerTime();
    }
    else
    {
        ez.buttons.show("Start # Interval # Menu");
        displayZeroTimerTime();
    }
}

void TimerScreen::displayTimer()
{
    if (secondChanged() && _isTimerRunning)
    {
        displayElapsedTimerTime();
    }
}

void TimerScreen::startTimer()
{
    _timerStartTimestamp = now();
    _isTimerRunning = true;
    displayZeroTimerTime();
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