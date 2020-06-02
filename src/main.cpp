#include "Arduino.h"
#include "M5Stack.h"
#include "ezTime.h"
#include "M5ez.h"
#include "Preferences.h"
#include "RTClib.h"

#include "images/jpgs.h"
#include "images/jpgsdark.h"

#include "pickers/DateTimePicker.h"
#include "pickers/SettingsPicker.h"

#include "screens/Screens.h"
#include "screens/StopwatchScreen.h"
#include "screens/TimerScreen.h"
#include "screens/AlarmScreen.h"
#include "screens/HomeScreen.h"
#include "screens/MainMenu.h"

RTC_DS1307 rtc;

const String VERSION_NUMBER = "0.7.0";

int _currentScreen = SCREEN_HOME;

int16_t _lastPickedMainMenuIndex = 1;

bool _isMinimalModeActive = false;

bool _backToMenu = false;

/////////////
// SCREENS //
/////////////

StopwatchScreen stopwatchScreen = StopwatchScreen();
AlarmScreen alarmScreen = AlarmScreen();
TimerScreen timerScreen = TimerScreen();
HomeScreen homeScreen = HomeScreen();
MainMenu mainMenuScreen = MainMenu();

/////////////////////
// Utility methods //
/////////////////////

String getTimezoneLocation()
{
	Preferences prefs;
	prefs.begin("M5ez", true);	// read-only
	String savedTimezone = prefs.getString("timezone", "GeoIP");
  prefs.end();
  return savedTimezone;
}

void setTimeFromRtc()
{
  if (timeStatus() == timeNotSet || timeStatus() == timeNeedsSync) 
  {
    Serial.println("Network time is unavailable");    

      //Real Time Clock (RTC) initialization
	    if (rtc.begin()) {
		    unsigned long timerTimestamp = rtc.now().unixtime();
		    Serial.println(F("RTC time available: "));		

        DateTime now = rtc.now();
        Serial.print(now.year(), DEC);
        Serial.print('-');
        Serial.print(now.month(), DEC);
        Serial.print('-');
        Serial.print(now.day(), DEC);
        Serial.print(' ');
        Serial.print(now.hour(), DEC);
        Serial.print(':');
        Serial.print(now.minute(), DEC);
        Serial.print(':');
        Serial.print(now.second(), DEC);
        Serial.println();

        setTime(timerTimestamp);        
        Serial.println("RTC based time was set.");
	    } else {
		    Serial.println(F("RTC is NOT available."));
        // No clock will be displayed - update has to be done manually with the Update button        
	    }
  }  
}

void storeTimeInRtc()
{
  //Update the RTC based time
  DateTime rtcDateTime = DateTime(ez.clock.tz.now());
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

///////////////////////
// Lifecycle methods //
///////////////////////

void setup() {
  #include <themes/default.h>
  #include <themes/dark.h>
 
  ez.begin();  

  Serial.println("\n");
  Serial.println(F(" __  __ ___        __      __    _      _    "));
  Serial.println(F("|  \\/  | __| ___ __\\ \\    / /_ _| |_ __| |_  "));
  Serial.println(F("| |\\/| |__ \\/ -_)_ /\\ \\/\\/ / _` |  _/ _| ' \\ "));
  Serial.println(F("|_|  |_|___/\\___/__| \\_/\\_/\\__,_|\\__\\__|_||_|"));

  Serial.print(F("v"));
  Serial.print(VERSION_NUMBER);
  Serial.println("\n");

  //Disable automatic updates on the NTP server.
  //The update needs several seconds to execute, which makes the seconds counter freeze until the update
  setInterval(0);

  setTimeFromRtc();

  homeScreen.initHomeScreen(&stopwatchScreen, &alarmScreen, &timerScreen);  
}

void loop() {    
  String buttonPressed = "";
  if (!_backToMenu) {
    buttonPressed = ez.buttons.poll();
    if (M5.BtnC.wasPressed() && _isMinimalModeActive) {      
      homeScreen.initHomeScreen(&stopwatchScreen, &alarmScreen, &timerScreen);    
    }
  }
  if (_backToMenu || buttonPressed  == "Menu") {
    ezMenu mainMenu = mainMenuScreen.initMainMenu();
    // Set the menu selection based on the last visited menu item
    mainMenu.pickItem(_lastPickedMainMenuIndex - 1);    
    // Run the stuff behind the menu item and return with its index + 1
    _lastPickedMainMenuIndex = mainMenu.runOnce();

    switch (_lastPickedMainMenuIndex)
    {
    case 0:
      homeScreen.initHomeScreen(&stopwatchScreen, &alarmScreen, &timerScreen);
      _backToMenu = false;
      _currentScreen = SCREEN_HOME;
      break;
    case 1:
      stopwatchScreen.initStopwatchScreen();
      _backToMenu = false;
      _currentScreen = SCREEN_STOPWATCH;
      break;
    case 2:
      alarmScreen.initAlarmScreen();
      _backToMenu = true;
      _currentScreen = SCREEN_ALARM;
      break;
    case 3:
      timerScreen.initTimerScreen();
      _backToMenu = false;
      _currentScreen = SCREEN_TIMER;
      break;
    case 4:
      SettingsPicker settingsPicker;
      settingsPicker.runOnce("Settings");
      _backToMenu = true;
      _currentScreen = SCREEN_SETTINGS;
      break;
    case 5:
      ez.msgBox("About", "Smart watch for M5Stack ESP32 Core | Version: " + VERSION_NUMBER + "| Author: kornel@schrenk.hu", "Menu");
      _backToMenu = true;
      break;
    }
  } else if (buttonPressed != "") {    
    //Handle button press on the current screen
    switch (_currentScreen) {
      case SCREEN_HOME:
        if (buttonPressed == "Update")
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

            storeTimeInRtc();

            ez.msgBox("Updated", dateTime(ez.clock.tz.now(), "Y-m-d H:i:s") + "||was set.", "Ok");            
          } else {
            ez.msgBox("Error", "Time update failed.", "Ok");
          }
          homeScreen.initHomeScreen(&stopwatchScreen, &alarmScreen, &timerScreen);
        }
        else if (buttonPressed == "Minimal")
        {
          if (!_isMinimalModeActive) {
            _isMinimalModeActive = true;
            ez.screen.clear();
            homeScreen.updateTime();
            homeScreen.updateDate();
          } else {
            _isMinimalModeActive = false;
          }         
        }
        break;
      case SCREEN_STOPWATCH:
        if (buttonPressed == "Start") {
          ez.buttons.show("$Stop # Reset # Menu");                    
          stopwatchScreen.startStopwatch();
          delay(300);
          ez.buttons.show("Stop # Reset # Menu");
        } else if (buttonPressed == "Stop") {
          ez.buttons.show("$Start # Reset # Menu");
          stopwatchScreen.stopStopwatch();
          delay(300);
          ez.buttons.show("Start # Reset # Menu");
        } else if (buttonPressed == "Reset") {
          ez.buttons.show("Start # $Reset # Menu");
          stopwatchScreen.resetStopwatch();
          delay(300);
          ez.buttons.show("Start # Reset # Menu");
        }
        break;
      case SCREEN_TIMER:		
        if (buttonPressed == "Start") {
          ez.buttons.show("$Stop # Interval # Menu");                    
          timerScreen.startTimer();
          delay(300);
          ez.buttons.show("Stop # Interval # Menu");
        } else if (buttonPressed == "Stop") {
          ez.buttons.show("$Start # Interval # Menu");
          timerScreen.stopTimer();
          delay(300);
          ez.buttons.show("Start # Interval # Menu");
        }	else if (buttonPressed == "Interval") {
          time_t initialTime = now();
          initialTime = makeTime(timerScreen.getTimerIntervalHours(), timerScreen.getTimerIntervalMinutes(), timerScreen.getTimerIntervalSeconds(), day(initialTime), month(initialTime), year(initialTime));
          Serial.println("Initial interval time: " + dateTime(initialTime, "Y-m-d H:i:s"));

          DateTimePicker intervalPicker;
          time_t pickedTime = intervalPicker.runOnce("Interval", initialTime, true, true);

          if (pickedTime != 0) {        
            timerScreen.setTimerIntervalHours(hour(pickedTime));
            timerScreen.setTimerIntervalMinutes(minute(pickedTime));
            timerScreen.setTimerIntervalSeconds(second(pickedTime));

            Serial.println("Picked interval time: " + dateTime(pickedTime, "Y-m-d H:i:s"));
          }
          timerScreen.initTimerScreen();
        }
        break;
    }
  } else {
    //NO Button was pressed - Normal operation
    switch (_currentScreen) {
      case SCREEN_HOME:
        homeScreen.displayHomeClock(&alarmScreen, &timerScreen);
        break;
      case SCREEN_STOPWATCH:
        stopwatchScreen.displayStopwatch();
        break;
      case SCREEN_TIMER:
        timerScreen.displayTimer();			
        break;
    }
  }
}
