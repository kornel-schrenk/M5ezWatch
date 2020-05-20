#include "Arduino.h"
#include "M5Stack.h"
#include "ezTime.h"
#include "M5ez.h"
#include "Preferences.h"
#include "RTClib.h"

#include "jpgs.h"
#include "jpgsdark.h"

#include "DateTimePicker.h"

#define SCREEN_HOME       210
#define SCREEN_STOPWATCH  220
#define SCREEN_ALARM      230
#define SCREEN_TIMER      240
#define SCREEN_SETTINGS   260

const String VERSION_NUMBER = "0.1.4";

int _currentScreen = SCREEN_HOME;

bool _clockWidgetDisplayed = false;

RTC_DS1307 rtc;

bool _isStopWatchRunning = false;
unsigned long _stopwatchStartTimestamp;
unsigned long _stopwatchElapsedTime = 0;

bool _isAlarmRunning = false;

bool _isTimerRunning = false;
unsigned long _timerStartTimestamp;
unsigned long _timerElapsedTime = 0;
int _timerIntervalMinutes = 1;
int _timerIntervalSeconds = 0;

/////////////////////
// Utility methods //
/////////////////////

bool isClockWidgetDisplayed() 
{
  Preferences prefs;
  prefs.begin("M5ez", true);	// read-only
  bool isDisplayed = prefs.getBool("clock_on", true);    
  prefs.end();
  return isDisplayed;
}

String getTimezoneLocation()
{
	Preferences prefs;
	prefs.begin("M5ez", true);	// read-only
	String savedTimezone = prefs.getString("timezone", "GeoIP");
  prefs.end();
  return savedTimezone;
}

void powerOff() { 
  M5.Power.powerOFF(); 
}

/////////////////
// HOME SCREEN //
/////////////////

void updateTime()
{    
    ez.canvas.color(ez.theme->foreground);
    ez.canvas.font(numonly7seg48);
    ez.canvas.pos(50, 80);
    ez.canvas.print(ez.clock.tz.dateTime("H:i:s"));  
}

void updateDate()
{
    String currentDate = ez.clock.tz.dateTime("Y-m-d");

    ez.canvas.color(ez.theme->foreground);
    ez.canvas.font(sans26);
    ez.canvas.pos(100, 150);
    ez.canvas.print(currentDate);
}

void refreshClockWidget() 
{
  //HACK: Refresh the clock widget at the top in every minute    
  if (_clockWidgetDisplayed) {
    ez.header.draw("clock");    
  }   
}

void initHomeScreen() 
{  
  _currentScreen = SCREEN_HOME;
  _clockWidgetDisplayed = isClockWidgetDisplayed();

  ez.screen.clear();
  ez.header.show("M5ezWatch");
  ez.buttons.show("Update # Menu # Power Off");  

  if (ez.theme->name == "Default") {
    if (_isStopWatchRunning) {
      M5.Lcd.drawJpg((uint8_t *)stopwatch_jpg_small, (sizeof(stopwatch_jpg_small) / sizeof(stopwatch_jpg_small[0])), 285, 64, 32, 32);   
    }
    if (_isAlarmRunning) {
      M5.Lcd.drawJpg((uint8_t *)alarm_jpg_small, (sizeof(alarm_jpg_small) / sizeof(alarm_jpg_small[0])), 285, 104, 32, 32);
    }
    if (_isTimerRunning) {
      M5.Lcd.drawJpg((uint8_t *)timer_jpg_small, (sizeof(timer_jpg_small) / sizeof(timer_jpg_small[0])), 285, 144, 32, 32);
    } 
  } else if (ez.theme->name == "Dark") {
    if (_isStopWatchRunning) {
      M5.Lcd.drawJpg((uint8_t *)stopwatch_jpg_small_dark, (sizeof(stopwatch_jpg_small_dark) / sizeof(stopwatch_jpg_small_dark[0])), 285, 64, 32, 32);   
    }
    if (_isAlarmRunning) {
      M5.Lcd.drawJpg((uint8_t *)alarm_jpg_small, (sizeof(alarm_jpg_small_dark) / sizeof(alarm_jpg_small_dark[0])), 285, 104, 32, 32);
    }
    if (_isTimerRunning) {
      M5.Lcd.drawJpg((uint8_t *)timer_jpg_small, (sizeof(timer_jpg_small_dark) / sizeof(timer_jpg_small_dark[0])), 285, 144, 32, 32);
    } 
  }    

  if (timeSet) {     
    updateTime();
    updateDate();
  }
}

void displayHomeClock()
{
  if (timeSet) {
    if (minuteChanged()) {          
      updateTime();
      updateDate();
      refreshClockWidget();
    }

    if (secondChanged()) {    
      updateTime();
    } 
  }
}

//////////////////////
// STOPWATCH SCREEN //
//////////////////////

void displayZeroTime() 
{
  ez.canvas.pos(50, 80);

  ez.canvas.print(zeropad(0, 2));  
  ez.canvas.print(":");
  ez.canvas.print(zeropad(0, 2));
  ez.canvas.print(":");
  ez.canvas.print(zeropad(0, 2)); 
}

void displayActualTime()
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

void initStopwatchScreen() 
{ 
  _currentScreen = SCREEN_STOPWATCH;
  _clockWidgetDisplayed = isClockWidgetDisplayed();

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

void startStopwatch()
{
  if (_stopwatchElapsedTime == 0) {
    _stopwatchStartTimestamp = now();
  } else {
    _stopwatchStartTimestamp = now() - _stopwatchElapsedTime;
  }
  _isStopWatchRunning = true;  
}

void stopStopwatch()
{
  _stopwatchElapsedTime = now() - _stopwatchStartTimestamp;
  _isStopWatchRunning = false;
}

void resetStopwatch()
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

void displayStopwatch()
{
  if (secondChanged() && _isStopWatchRunning) {
    displayActualTime();
  }
}

//////////////////
// ALARM SCREEN //
//////////////////

void initAlarmScreen() 
{ 
  _currentScreen = SCREEN_ALARM;
  _clockWidgetDisplayed = isClockWidgetDisplayed();
  
  DateTimePicker alarmPicker;
  String pickedDateTime = alarmPicker.runOnce("Alarm");

  //TODO Process the results
  Serial.print("Returned: ");
  Serial.println(pickedDateTime);  

  // Go back to the Home Screen
  initHomeScreen();
}

//////////////////
// TIMER SCREEN //
//////////////////

void displayZeroTimerTime() 
{
  ez.canvas.color(ez.theme->foreground);
  ez.canvas.font(numonly7seg48);
  ez.canvas.pos(90, 150);    

  ez.canvas.print(zeropad(0, 2));
  ez.canvas.print(":");
  ez.canvas.print(zeropad(0, 2));

  M5.Lcd.fillRect(10, 100, 300, 25, ez.theme->background);
  M5.Lcd.progressBar(10, 100, 300, 25, 0);
}

void checkAndFireTimer(int elapsedMinutes, int elapsedSeconds)
{
  if (_isTimerRunning && elapsedMinutes == _timerIntervalMinutes && elapsedSeconds == _timerIntervalSeconds) {
    // Play a buzz
    M5.Speaker.tone(900, 300);

    // Restart the timer
    _timerStartTimestamp = now();

    // Display zero time
    displayZeroTimerTime();
  }
}

void displayElapsedTimerTime()
{
  unsigned long elapsedTimeInSeconds = now() - _timerStartTimestamp;

  int minutes = elapsedTimeInSeconds / 60;
  int seconds = elapsedTimeInSeconds % 60;

  ez.canvas.color(ez.theme->foreground);
  ez.canvas.font(numonly7seg48);
  ez.canvas.pos(90, 150);    

  ez.canvas.print(zeropad(minutes, 2));
  ez.canvas.print(":");
  ez.canvas.print(zeropad(seconds, 2));

  // Calculate elapsed time %
  int timerIntervalInSeconds = _timerIntervalMinutes * 60 + _timerIntervalSeconds;
  uint8_t elapsedPercentage = (elapsedTimeInSeconds * 100) / timerIntervalInSeconds;
  M5.Lcd.progressBar(10, 100, 300, 25, elapsedPercentage);

  checkAndFireTimer(minutes, seconds);
}

void initTimerScreen()
{
  _currentScreen = SCREEN_TIMER;
  _clockWidgetDisplayed = isClockWidgetDisplayed();  

  ez.screen.clear();
  ez.header.show("Timer");

  ez.canvas.color(ez.theme->foreground);
  
  ez.canvas.font(sans26);
  ez.canvas.pos(20, 50);
  ez.canvas.print("Interval: ");
  ez.canvas.pos(200, 50);
  ez.canvas.print(zeropad(_timerIntervalMinutes, 2));  
  ez.canvas.print(":");
  ez.canvas.print(zeropad(_timerIntervalSeconds, 2));   

  if (_isTimerRunning) {
    ez.buttons.show("Stop # Interval # Menu");
    displayElapsedTimerTime();
  } else {
    ez.buttons.show("Start # Interval # Menu");
    displayZeroTimerTime();
  }  
}

void displayTimer()
{
  if (secondChanged() && _isTimerRunning) {
    displayElapsedTimerTime();
  }
}

void startTimer()
{
  _timerStartTimestamp = now();
  _isTimerRunning = true;
  displayZeroTimerTime();  
}

void stopTimer()
{
  _isTimerRunning = false;
}

///////////////
// MAIN MENU //
///////////////

ezMenu initMainMenu()
{
  ezMenu mainMenu;

  //If the image is 96x96 px
  mainMenu.imgFromTop(72);
  mainMenu.imgFromLeft(112);
  
  if (ez.theme->name == "Default") {  
    mainMenu.imgBackground(ez.theme->background);
    mainMenu.imgCaptionColor(ez.theme->header_bgcolor);

    //JPG files from FLASH - Default theme
    mainMenu.addItem(stopwatch_jpg, "Stopwatch");
    mainMenu.addItem(alarm_jpg, "Alarm");
    mainMenu.addItem(timer_jpg, "Timer");
    mainMenu.addItem(settings_jpg, "Settings", ez.settings.menu);
    mainMenu.addItem(about_jpg, "About");
    mainMenu.addItem(back_jpg, "Back");
  } else if (ez.theme->name == "Dark") {
    mainMenu.imgBackground(ez.theme->background);
    mainMenu.imgCaptionColor(ez.theme->header_fgcolor);

    //JPG files from FLASH - Dark theme
    mainMenu.addItem(stopwatch_jpg_dark, "Stopwatch");
    mainMenu.addItem(alarm_jpg_dark, "Alarm");
    mainMenu.addItem(timer_jpg_dark, "Timer");
    mainMenu.addItem(settings_jpg_dark, "Settings", ez.settings.menu);
    mainMenu.addItem(about_jpg_dark, "About");
    mainMenu.addItem(back_jpg_dark, "Back");
  }
  
  return mainMenu;
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
  //The update needs several seconds to execute, which makes the seconds counter stop until the update
  setInterval(0);

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

  initHomeScreen();  
}

void loop() {    
  String buttonPressed = ez.buttons.poll();
  if (buttonPressed  == "Menu") {              
    back_to_menu:
    int16_t selectedIndex = initMainMenu().runOnce();                   
    switch (selectedIndex) {      
      case 1: initStopwatchScreen(); break;  //Stopwatch screen
      case 2: initAlarmScreen(); break;      //Alarm screen
      case 3: initTimerScreen(); break;      
      case 4: ez.settings.menuObj.runOnce(); Serial.println("Goto"); goto back_to_menu; break;     //Settings screen
      default: initHomeScreen(); break; 
    }     
  } else if (buttonPressed != "") {    
    //Handle button press on the current screen
    switch (_currentScreen) {
      case SCREEN_HOME:		      
         if (buttonPressed  == "Update") {
          ez.buttons.show("$Update # Menu # Power Off"); 
          updateNTP();        
          if (timeSet) {
            //Update timezone based on Preferences
            String storedTimezone = getTimezoneLocation();
            Serial.println("Stored timezone: " + storedTimezone);
            ez.clock.tz.setLocation(storedTimezone);
            Serial.println("New timezone was set to " + storedTimezone);

            updateTime();
            updateDate();
            refreshClockWidget();

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
          ez.buttons.show("Update # Menu # Power Off");  
        } else if (buttonPressed  == "Power Off") {
          powerOff();
        }
        break;
      case SCREEN_STOPWATCH:
        if (buttonPressed == "Start") {
          ez.buttons.show("$Stop # Reset # Menu");                    
          startStopwatch();
          delay(300);
          ez.buttons.show("Stop # Reset # Menu");
        } else if (buttonPressed == "Stop") {
          ez.buttons.show("$Start # Reset # Menu");
          stopStopwatch();
          delay(300);
          ez.buttons.show("Start # Reset # Menu");
        } else if (buttonPressed == "Reset") {
          ez.buttons.show("Start # $Reset # Menu");
          resetStopwatch();
          delay(300);
          ez.buttons.show("Start # Reset # Menu");
        }
        break;
      case SCREEN_ALARM:			
        break;
      case SCREEN_TIMER:		
        if (buttonPressed == "Start") {
          ez.buttons.show("$Stop # Interval # Menu");                    
          startTimer();
          delay(300);
          ez.buttons.show("Stop # Interval # Menu");
        } else if (buttonPressed == "Stop") {
          ez.buttons.show("$Start # Interval # Menu");
          stopTimer();
          delay(300);
          ez.buttons.show("Start # Interval # Menu");
        }	
        break;
      case SCREEN_SETTINGS:			
        break;
    }
  } else {
    //NO Button was pressed - Normal operation
    switch (_currentScreen) {
      case SCREEN_HOME:
        displayHomeClock();
        break;
      case SCREEN_STOPWATCH:
        displayStopwatch();
        break;
      case SCREEN_ALARM:			
        break;
      case SCREEN_TIMER:
        displayTimer();			
        break;
      case SCREEN_SETTINGS:			
        break;
    }
  }
}
