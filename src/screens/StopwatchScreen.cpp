#include "screens/StopwatchScreen.h"

void StopwatchScreen::displayZeroTime() 
{
  ez.canvas.pos(50, 80);

  ez.canvas.print(zeropad(0, 2));  
  ez.canvas.print(":");
  ez.canvas.print(zeropad(0, 2));
  ez.canvas.print(":");
  ez.canvas.print(zeropad(0, 2)); 
}

void StopwatchScreen::displayActualTime()
{
  unsigned long elapsedTimeInSeconds = now() - _stopwatchStartTimestamp;

  int hours = elapsedTimeInSeconds / 3600;
  elapsedTimeInSeconds = elapsedTimeInSeconds % 3600;
  int minutes = elapsedTimeInSeconds / 60;
  int seconds = elapsedTimeInSeconds % 60;

  ez.canvas.pos(50, 80);

  ez.canvas.print(zeropad(hours, 2));  
  ez.canvas.print(":");
  ez.canvas.print(zeropad(minutes, 2));
  ez.canvas.print(":");
  ez.canvas.print(zeropad(seconds, 2));   
}

void StopwatchScreen::initStopwatchScreen() 
{ 
  ez.screen.clear();
  ez.header.show("Stopwatch");
      
  ez.canvas.color(ez.theme->foreground);
  ez.canvas.font(numonly7seg48);
  
  if (_isStopWatchRunning) {
    ez.buttons.show("Stop # Reset # Menu");
    displayActualTime();
  } else {
    ez.buttons.show("Start # Reset # Menu");
    displayZeroTime();
  }
}

void StopwatchScreen::startStopwatch()
{
  if (_stopwatchElapsedTime == 0) {
    _stopwatchStartTimestamp = now();
  } else {
    _stopwatchStartTimestamp = now() - _stopwatchElapsedTime;
  }
  _isStopWatchRunning = true;  
}

void StopwatchScreen::stopStopwatch()
{
  _stopwatchElapsedTime = now() - _stopwatchStartTimestamp;
  _isStopWatchRunning = false;
}

void StopwatchScreen::resetStopwatch()
{
  _stopwatchElapsedTime = 0;
  _isStopWatchRunning = false;

  ez.canvas.color(ez.theme->foreground);
  ez.canvas.font(numonly7seg48);
  ez.canvas.pos(50, 80);

  ez.canvas.print(zeropad(0, 2));  
  ez.canvas.print(":");
  ez.canvas.print(zeropad(0, 2));
  ez.canvas.print(":");
  ez.canvas.print(zeropad(0, 2)); 
}

void StopwatchScreen::displayStopwatch()
{
  if (secondChanged() && _isStopWatchRunning) {
    displayActualTime();
  }
}

bool StopwatchScreen::isRunning()
{
    return _isStopWatchRunning;
}