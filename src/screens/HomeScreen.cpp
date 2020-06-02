#include "screens/HomeScreen.h"
#include "screens/Screens.h"
#include "Preferences.h"

void HomeScreen::updateTime()
{    
    ez.canvas.color(ez.theme->foreground);
    ez.canvas.font(numonly7seg48);
    ez.canvas.pos(50, 80);

    if (ez.clock.isClockFormat12()) {      
      ez.canvas.print(ez.clock.tz.dateTime("h:i:s"));
    } else {
      ez.canvas.print(ez.clock.tz.dateTime("H:i:s"));  
    }    
}

void HomeScreen::updateDate()
{
    String currentDate = ez.clock.tz.dateTime("Y-m-d");

    ez.canvas.color(ez.theme->foreground);
    ez.canvas.font(sans26);
    ez.canvas.pos(100, 150);
    ez.canvas.print(currentDate);
}

void HomeScreen::updateAmPm()
{
  if (ez.clock.isClockFormat12() && ez.clock.isAmPmIndicatorDisplayed()) {
    ez.canvas.pos(272, 108);
    ez.canvas.print(ez.clock.tz.dateTime("A"));
  }
}

void HomeScreen::initHomeScreen(StopwatchScreen* stopwatchScreen, AlarmScreen* alarmScreen, TimerScreen* timerScreen) 
{  
  ez.screen.clear();
  ez.header.show("M5ezWatch");
  ez.buttons.show("Update # Menu # Minimal");  

  if (ez.theme->name == "Default") {
    if (stopwatchScreen->isRunning()) {
      M5.Lcd.drawJpg((uint8_t *)stopwatch_jpg_small, (sizeof(stopwatch_jpg_small) / sizeof(stopwatch_jpg_small[0])), 285, 64, 32, 32);   
    }
    if (alarmScreen->isRunning()) {
      if (!ez.clock.isClockFormat12() || !ez.clock.isAmPmIndicatorDisplayed()) {
        M5.Lcd.drawJpg((uint8_t *)alarm_jpg_small, (sizeof(alarm_jpg_small) / sizeof(alarm_jpg_small[0])), 285, 104, 32, 32);
      } else {
        M5.Lcd.drawJpg((uint8_t *)alarm_jpg_small, (sizeof(alarm_jpg_small) / sizeof(alarm_jpg_small[0])), 285, 184, 32, 32);
      }  
    }
    if (timerScreen->isRunning()) {
      M5.Lcd.drawJpg((uint8_t *)timer_jpg_small, (sizeof(timer_jpg_small) / sizeof(timer_jpg_small[0])), 285, 144, 32, 32);
    } 
  } else if (ez.theme->name == "Dark") {
    if (stopwatchScreen->isRunning()) {
      M5.Lcd.drawJpg((uint8_t *)stopwatch_jpg_small_dark, (sizeof(stopwatch_jpg_small_dark) / sizeof(stopwatch_jpg_small_dark[0])), 285, 64, 32, 32);   
    }
    if (alarmScreen->isRunning()) {
      if (!ez.clock.isClockFormat12() || !ez.clock.isAmPmIndicatorDisplayed()) {
        M5.Lcd.drawJpg((uint8_t *)alarm_jpg_small_dark, (sizeof(alarm_jpg_small_dark) / sizeof(alarm_jpg_small_dark[0])), 285, 104, 32, 32);
      } else {
        M5.Lcd.drawJpg((uint8_t *)alarm_jpg_small_dark, (sizeof(alarm_jpg_small_dark) / sizeof(alarm_jpg_small_dark[0])), 285, 184, 32, 32);
      }
    }
    if (timerScreen->isRunning()) {
      M5.Lcd.drawJpg((uint8_t *)timer_jpg_small_dark, (sizeof(timer_jpg_small_dark) / sizeof(timer_jpg_small_dark[0])), 285, 144, 32, 32);
    } 
  }    

  if (timeStatus() == timeSet) {     
    updateTime();
    updateDate();
    updateAmPm();
  }
}

void HomeScreen::displayHomeClock(AlarmScreen* alarmScreen, TimerScreen* timerScreen)
{
  if (timeStatus() == timeSet) {
    if (minuteChanged()) {          
      updateTime();
      updateDate();
      updateAmPm();
      _refreshClockWidget();
      alarmScreen->checkAndFireAlarm();     
    }

    if (secondChanged()) {    
      updateTime();
      timerScreen->checkAndFireTimer(SCREEN_HOME);      
    } 
  }
}

bool HomeScreen::_isClockWidgetDisplayed() 
{
  Preferences prefs;
  prefs.begin("M5ez", true);	// read-only
  bool isDisplayed = prefs.getBool("clock_on", true);    
  prefs.end();
  return isDisplayed;
}

void HomeScreen::_refreshClockWidget() 
{
  //HACK: Refresh the clock widget at the top in every minute    
  if (_isClockWidgetDisplayed()) {
    ez.header.draw("clock");    
  }   
}