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
    this->displayActualTime();
  } else {
    ez.buttons.show("Start # Reset # Menu");
    this->displayZeroTime();
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
  if (minuteChanged()) {
    this->refreshClockWidget();
    this->displayActualTime();
  }

  if (secondChanged() && _isStopWatchRunning) {
    this->displayActualTime();
  }
}

bool StopwatchScreen::isRunning()
{
    return _isStopWatchRunning;
}

void StopwatchScreen::handleButtonPress(String buttonName)
{
  if (buttonName == "Start")
  {
    ez.buttons.show("$Stop # Reset # Menu");
    this->startStopwatch();
    delay(300);
    ez.buttons.show("Stop # Reset # Menu");
  }
  else if (buttonName == "Stop")
  {
    ez.buttons.show("$Start # Reset # Menu");
    this->stopStopwatch();
    delay(300);
    ez.buttons.show("Start # Reset # Menu");
  }
  else if (buttonName == "Reset")
  {
    ez.buttons.show("Start # $Reset # Menu");
    this->resetStopwatch();
    delay(300);
    ez.buttons.show("Start # Reset # Menu");
  }
}