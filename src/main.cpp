#include "Arduino.h"
#include "M5Stack.h"
#include "ezTime.h"
#include "M5ez.h"
#include "Preferences.h"
#include "RTClib.h"

#include "jpgs.h"
#include "jpgsdark.h"
// #include "bmps.h"
// #include "xbmps.h"

#define SCREEN_HOME       210
#define SCREEN_STOP_WATCH 220
#define SCREEN_ALARM      230
#define SCREEN_TIMER      240
#define SCREEN_SETTINGS   260
#define SCREEN_MENU       270

const String VERSION_NUMBER = "0.1.4";

int _currentScreen = SCREEN_HOME;

bool _clockWidgetDisplayed = false;

RTC_DS1307 rtc;

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

  //BMP files from FLASH - Black background is transparent
  // mainMenu.addBmpImageItem(stopwatch_bmp, "Stopwatch", stopwatch_bmp_width, stopwatch_bmp_height);
  // mainMenu.addBmpImageItem(alarm_bmp, "Alarm", alarm_bmp_width, alarm_bmp_height);
  // mainMenu.addBmpImageItem(timer_bmp, "Timer", timer_bmp_width, timer_bmp_height);  
  // mainMenu.addBmpImageItem(settings_bmp, "Settings", settings_bmp_width, settings_bmp_height);  
  // mainMenu.addBmpImageItem(about_bmp, "About", about_bmp_width, about_bmp_height);  
  // mainMenu.addBmpImageItem(back_bmp, "Back", back_bmp_width, back_bmp_height);
  
  //XBMP files in blue foreground and black background
  // mainMenu.addXBmpImageItem(stopwatch_xbmp, "Stopwatch", stopwatch_xbmp_width, stopwatch_xbmp_height, TFT_BLUE, TFT_BLACK);
  // mainMenu.addXBmpImageItem(alarm_xbmp, "Alarm", alarm_xbmp_width, alarm_xbmp_height, TFT_BLUE, TFT_BLACK);
  // mainMenu.addXBmpImageItem(timer_xbmp, "Timer", timer_xbmp_width, timer_xbmp_height, TFT_BLUE, TFT_BLACK);  
  // mainMenu.addXBmpImageItem(settings_xbmp, "Settings", settings_xbmp_width, settings_xbmp_height, TFT_BLUE, TFT_BLACK);  
  // mainMenu.addXBmpImageItem(about_xbmp, "About", about_xbmp_width, about_xbmp_height, TFT_BLUE, TFT_BLACK);  
  // mainMenu.addXBmpImageItem(back_xbmp, "Back", back_xbmp_width, back_xbmp_height, TFT_BLUE, TFT_BLACK);

  //JPG files from SD card - Default theme
  // mainMenu.addItem(SD, "/M5ezWatch/StopwatchDefault.jpg", "Stopwatch");
  // mainMenu.addItem(SD, "/M5ezWatch/AlarmDefault.jpg", "Alarm");
  // mainMenu.addItem(SD, "/M5ezWatch/TimerDefault.jpg", "Timer");
  // mainMenu.addItem(SD, "/M5ezWatch/SettingsDefault.jpg", "Settings", ez.settings.menu);
  // mainMenu.addItem(SD, "/M5ezWatch/AboutDefault.jpg", "About");
  // mainMenu.addItem(SD, "/M5ezWatch/BackDefault.jpg", "Back");

  //JPG files from SD card - Dark theme
  // mainMenu.addItem(SD, "/M5ezWatch/StopwatchDark.jpg", "Stopwatch");
  // mainMenu.addItem(SD, "/M5ezWatch/AlarmDark.jpg", "Alarm");
  // mainMenu.addItem(SD, "/M5ezWatch/TimerDark.jpg", "Timer");
  // mainMenu.addItem(SD, "/M5ezWatch/SettingsDark.jpg", "Settings", ez.settings.menu);
  // mainMenu.addItem(SD, "/M5ezWatch/AboutDark.jpg", "About");
  // mainMenu.addItem(SD, "/M5ezWatch/BackDark.jpg", "Back");

  //Raw bmp files from SD card - only 24 bit (True Color) bmp are supported without transparency (BMP file format does not support transparency)
  //https://online-converting.com/image/convert2bmp/
  // mainMenu.addBmpImageItem(SD, "/M5ezWatch/Stopwatch.bmp", "Stopwatch");
  // mainMenu.addBmpImageItem(SD, "/M5ezWatch/Alarm.bmp", "Alarm");
  // mainMenu.addBmpImageItem(SD, "/M5ezWatch/Timer.bmp", "Timer");
  // mainMenu.addBmpImageItem(SD, "/M5ezWatch/Settings.bmp", "Settings", ez.settings.menu);
  // mainMenu.addBmpImageItem(SD, "/M5ezWatch/About.bmp", "About");
  // mainMenu.addBmpImageItem(SD, "/M5ezWatch/Back.bmp", "Back");
  
  //PNG files from SD card - its rendering is quite SLOW!!!
  // mainMenu.addPngImageItem(SD, "/M5ezWatch/Stopwatch.png", "Stopwatch");
  // mainMenu.addPngImageItem(SD, "/M5ezWatch/Alarm.png", "Alarm");
  // mainMenu.addPngImageItem(SD, "/M5ezWatch/Timer.png", "Timer");
  // mainMenu.addPngImageItem(SD, "/M5ezWatch/Settings.png", "Settings", ez.settings.menu);
  // mainMenu.addPngImageItem(SD, "/M5ezWatch/About.png", "About");
  // mainMenu.addPngImageItem(SD, "/M5ezWatch/Back.png", "Back");
  
  return mainMenu;
}

///////////////////////
// Lifecycle methods //
///////////////////////

void setup() {
  #include <themes/default.h>
  #include <themes/dark.h>

  //Serial.begin(115200);  
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
  if (buttonPressed != "") {    
    //Handle button press on the current screen
    switch (_currentScreen) {
      case SCREEN_HOME:		      
        if (buttonPressed  == "Menu") {          
          _currentScreen = SCREEN_MENU;
          initMainMenu().run();        
          initHomeScreen();
        } else if (buttonPressed  == "Update") {
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
      case SCREEN_STOP_WATCH:			
        break;
      case SCREEN_ALARM:			
        break;
      case SCREEN_TIMER:			
        break;
      case SCREEN_SETTINGS:			
        break;
      case SCREEN_MENU:			
        break;  
    }
  } else {
    //NO Button was pressed - Normal operation
    switch (_currentScreen) {
      case SCREEN_HOME:
        displayHomeClock();
        break;
      case SCREEN_STOP_WATCH:			
        break;
      case SCREEN_ALARM:			
        break;
      case SCREEN_TIMER:			
        break;
      case SCREEN_SETTINGS:			
        break;
      case SCREEN_MENU:			
        break;
    }
  }
}
