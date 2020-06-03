#include "screens/AlarmScreen.h"
#include "pickers/DateTimePicker.h"

void AlarmScreen::initAlarmScreen() 
{ 
  if (_alarmTime == 0 && timeStatus() == timeSet) {
    this->setInitialAlarmTime(ez.clock.tz.now());
  }

  DateTimePicker alarmPicker;
  time_t pickedDateTime = alarmPicker.runOnce("Alarm", _alarmTime, _isAlarmRunning, false, false);

  if (pickedDateTime == 0) { // Back pressed
    if (_isAlarmRunning) {
      ez.msgBox("Not Alarmed", dateTime(_alarmTime, "Y-m-d H:i") + "||was cancelled.", "Ok");
    }
    _isAlarmRunning = false;    
  }  else if (pickedDateTime <= ez.clock.tz.now()) {
    ez.msgBox("Error", dateTime(pickedDateTime, "Y-m-d H:i") + "||is in the past!", "Ok");    
  } else {
    _isAlarmRunning = true;
    _alarmTime = pickedDateTime;
    ez.msgBox("Alarmed", dateTime(_alarmTime, "Y-m-d H:i") + "||was set.", "Ok");
  }
}

void AlarmScreen::checkAndFireAlarm()
{
  if (_isAlarmRunning && ez.clock.tz.now() >= _alarmTime) {
    
    Serial.print(dateTime(_alarmTime, "Y-m-d H:i:s"));
    Serial.println(F(" ALARM!!!"));
    
    _isAlarmRunning = false;

    // Play the alarm sound
    M5.Speaker.tone(900, 150);
    delay(150);
    M5.Speaker.tone(1000, 150);
    delay(150);
    M5.Speaker.tone(1100, 150);
    delay(150);
    M5.Speaker.tone(1000, 150);
    delay(150);
    M5.Speaker.tone(900, 150);

    // Clear the alarm icon on the screen 
    M5.Lcd.fillRect(285, 104, 32, 32, ez.theme->background);       
  }
}

bool AlarmScreen::isRunning()
{
    return _isAlarmRunning;
}

void AlarmScreen::setInitialAlarmTime(time_t alarmTime)
{
  _alarmTime = alarmTime;
}