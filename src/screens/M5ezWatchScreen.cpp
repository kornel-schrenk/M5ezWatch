#include "screens/M5ezWatchScreen.h"
#include "Preferences.h"

bool M5ezWatchScreen::isClockWidgetDisplayed()
{
  Preferences prefs;
  prefs.begin("M5ez", true); // read-only
  bool isDisplayed = prefs.getBool("clock_on", true);
  prefs.end();
  return isDisplayed;
}

void M5ezWatchScreen::refreshClockWidget()
{
  //HACK: Refresh the clock widget at the top in every minute
  if (this->isClockWidgetDisplayed())
  {
    ez.header.draw("clock");
  }
}