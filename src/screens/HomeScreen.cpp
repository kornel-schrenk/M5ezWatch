#include "screens/HomeScreen.h"
#include "screens/Screens.h"
#include "Preferences.h"
#include "RTClib.h"

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
    this->updateTime();
    this->updateDate();
    this->updateAmPm();
  }
}

void HomeScreen::displayHomeClock(AlarmScreen* alarmScreen, TimerScreen* timerScreen)
{
  if (timeStatus() == timeSet) {
    if (minuteChanged()) {  
      alarmScreen->checkAndFireAlarm();        
      this->updateTime();
      this->updateDate();
      this->updateAmPm();
      this->refreshClockWidget();             
    }

    if (secondChanged()) {    
      this->updateTime();
      timerScreen->checkAndFireTimer(SCREEN_HOME);      
    } 
  }
}

bool HomeScreen::isMinimalModeActive()
{
  return _isMinimalModeActive;
}

void HomeScreen::handleButtonPress(String buttonName, StopwatchScreen* stopwatchScreen, AlarmScreen* alarmScreen, TimerScreen* timerScreen)
{
  if (buttonName == "Update")
  {
    ez.buttons.show("$Update # Menu # Minimal");
    updateNTP();
    if (timeStatus() == timeSet)
    {
      //Update timezone based on Preferences
      String storedTimezone = getTimezoneLocation();
      Serial.println("Stored timezone: " + storedTimezone);
      ez.clock.tz.setLocation(storedTimezone);
      Serial.println("New timezone was set to " + storedTimezone);

      this->storeTimeInRtc();

      ez.msgBox("Updated", dateTime(ez.clock.tz.now(), "Y-m-d H:i:s") + "||was set.", "Ok");
    }
    else
    {
      ez.msgBox("Error", "Time update failed.", "Ok");
    }
    initHomeScreen(stopwatchScreen, alarmScreen, timerScreen);
  }
  else if (buttonName == "Minimal")
  {
    if (!_isMinimalModeActive)
    {
      _isMinimalModeActive = true;
      ez.screen.clear();
      this->updateTime();
      this->updateDate();
    }
    else
    {
      _isMinimalModeActive = false;
    }
  }
}

String HomeScreen::getTimezoneLocation()
{
	Preferences prefs;
	prefs.begin("M5ez", true);	// read-only
	String savedTimezone = prefs.getString("timezone", "GeoIP");
  prefs.end();
  return savedTimezone;
}

void HomeScreen::storeTimeInRtc()
{
  //Update the RTC based time
  DateTime rtcDateTime = DateTime(ez.clock.tz.now());
 
  RTC_DS1307 rtc;
  rtc.adjust(rtcDateTime);

  Serial.println(F("RTC time was set as: "));
  Serial.print(rtcDateTime.year(), DEC);
  Serial.print('-');
  Serial.print(rtcDateTime.month(), DEC);
  Serial.print('-');
  Serial.print(rtcDateTime.day(), DEC);
  Serial.print(' ');
  Serial.print(rtcDateTime.hour(), DEC);
  Serial.print(':');
  Serial.print(rtcDateTime.minute(), DEC);
  Serial.print(':');
  Serial.print(rtcDateTime.second(), DEC);
  Serial.println();
}