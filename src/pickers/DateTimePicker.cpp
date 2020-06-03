#include "pickers/DateTimePicker.h"

///////////////////////////
// Variable declarations //
///////////////////////////

  // The currently picked date and time values
  String DateTimePicker::_pickedYear;
  String DateTimePicker::_pickedMonth;
  String DateTimePicker::_pickedDay;
  String DateTimePicker::_pickedHour;
  String DateTimePicker::_pickedMinute;
  String DateTimePicker::_pickedSecond;

  // General picker settings
  String DateTimePicker::_pickerButtons;
  GFXfont DateTimePicker::_pickerFont;
  int8_t DateTimePicker::_pickerCheckType;
  String DateTimePicker::_pickerCheckButtonName;

/////////////
// Pickers //
/////////////

// Year Picker //

void DateTimePicker::_displayYearPicker() 
{  
  ezMenu yearPickerMenu("Year");

  yearPickerMenu.buttons(_pickerButtons);
  yearPickerMenu.txtFont(&_pickerFont);
  yearPickerMenu.setCheckType(_pickerCheckType);
  yearPickerMenu.setCheckButtonName(_pickerCheckButtonName);

  yearPickerMenu.addItem("2020");
  yearPickerMenu.addItem("2021");
  yearPickerMenu.addItem("2022");
  yearPickerMenu.addItem("2023");

  yearPickerMenu.check(_pickedYear);

  yearPickerMenu.runOnce();
  
  if (yearPickerMenu.pickButton() == "Ok") {  
    String checkedYear = yearPickerMenu.getCheckedItemName();
    if (checkedYear != "") {
      _pickedYear = checkedYear;

      //Check 29th February in non-leap years --> change it to 28th February
      if (_pickedYear.toInt() % 4 != 0 && _pickedMonth == "02" && _pickedDay == "29") {
        _pickedDay = "28";
      }
    }    
  }
}

bool DateTimePicker::_advancedDisplayYearPicker(ezMenu* callingMenu) {
  if (callingMenu->pickButton() != "Ok") {
    _displayYearPicker();
   }
   return true;
}

// Month Picker //

void DateTimePicker::_displayMonthPicker()
{
  ezMenu monthPickerMenu("Month");

  monthPickerMenu.buttons(_pickerButtons);
  monthPickerMenu.txtFont(&_pickerFont);
  monthPickerMenu.setCheckType(_pickerCheckType);
  monthPickerMenu.setCheckButtonName(_pickerCheckButtonName);
  monthPickerMenu.upOnFirst("last|up");
  monthPickerMenu.downOnLast("first|down");

  monthPickerMenu.addItem("01");
  monthPickerMenu.addItem("02");
  monthPickerMenu.addItem("03");
  monthPickerMenu.addItem("04");
  monthPickerMenu.addItem("05");
  monthPickerMenu.addItem("06");
  monthPickerMenu.addItem("07");
  monthPickerMenu.addItem("08");
  monthPickerMenu.addItem("09");
  monthPickerMenu.addItem("10");
  monthPickerMenu.addItem("11");
  monthPickerMenu.addItem("12");  

  monthPickerMenu.check(_pickedMonth);

  monthPickerMenu.runOnce();
  
  if (monthPickerMenu.pickButton() == "Ok") {    
    String checkedMonth = monthPickerMenu.getCheckedItemName();
    if (checkedMonth != "") {
      _pickedMonth = checkedMonth;

      //Correct the wrongly set last day of the month
      if (_pickedDay == "31" && (_pickedMonth == "04" || _pickedMonth == "06" || _pickedMonth == "09" || _pickedMonth == "11")) {
        _pickedDay = "30";
      }
      if (_pickedMonth == "02" && (_pickedDay == "30" || _pickedDay == "31")) {        
        if (_pickedYear.toInt() % 4 == 0) {
          _pickedDay = "29"; //In leap year
        } else {
          _pickedDay = "28";
        }        
      }      
      if (_pickedMonth == "02" && _pickedDay == "29" && _pickedYear.toInt() % 4 != 0) { 
        _pickedDay = "28"; //In NOT leap year 
      }
    } 
  }
}

bool DateTimePicker::_advancedDisplayMonthPicker(ezMenu* callingMenu) {
  if (callingMenu->pickButton() != "Ok") {
    _displayMonthPicker();
   }
   return true;
}

// Day Picker //

void DateTimePicker::_displayDayPicker()
{
  ezMenu dayPickerMenu("Day");

  dayPickerMenu.buttons(_pickerButtons);
  dayPickerMenu.txtFont(&_pickerFont);
  dayPickerMenu.setCheckType(_pickerCheckType);
  dayPickerMenu.setCheckButtonName(_pickerCheckButtonName);
  dayPickerMenu.upOnFirst("last|up");
  dayPickerMenu.downOnLast("first|down");

  dayPickerMenu.addItem("01");
  dayPickerMenu.addItem("02");
  dayPickerMenu.addItem("03");
  dayPickerMenu.addItem("04");
  dayPickerMenu.addItem("05");
  dayPickerMenu.addItem("06");
  dayPickerMenu.addItem("07");
  dayPickerMenu.addItem("08");
  dayPickerMenu.addItem("09");
  dayPickerMenu.addItem("10");
  dayPickerMenu.addItem("11");
  dayPickerMenu.addItem("12");  
  dayPickerMenu.addItem("13");
  dayPickerMenu.addItem("14");
  dayPickerMenu.addItem("15");
  dayPickerMenu.addItem("16");
  dayPickerMenu.addItem("17");
  dayPickerMenu.addItem("18");
  dayPickerMenu.addItem("19");
  dayPickerMenu.addItem("20");
  dayPickerMenu.addItem("21");
  dayPickerMenu.addItem("22");
  dayPickerMenu.addItem("23");
  dayPickerMenu.addItem("24");
  dayPickerMenu.addItem("25");
  dayPickerMenu.addItem("26");
  dayPickerMenu.addItem("27");
  dayPickerMenu.addItem("28");    
  if (_pickedMonth != "02" || _pickedYear.toInt() % 4 == 0) {
    dayPickerMenu.addItem("29");
  }  
  if (_pickedMonth != "02") {
    dayPickerMenu.addItem("30");
  }
  if (_pickedMonth == "01" || _pickedMonth == "03" || _pickedMonth == "05" || _pickedMonth == "07" || _pickedMonth == "08" || _pickedMonth ==  "10" || _pickedMonth == "12") {
    dayPickerMenu.addItem("31");
  }

  if (!dayPickerMenu.check(_pickedDay)) dayPickerMenu.check(0);

  dayPickerMenu.runOnce();
  
if (dayPickerMenu.pickButton() == "Ok") {
    String checkedDay = dayPickerMenu.getCheckedItemName();
    if (checkedDay != "") {
      _pickedDay = checkedDay;
    } 
  }
}

bool DateTimePicker::_advancedDisplayDayPicker(ezMenu* callingMenu) {
  if (callingMenu->pickButton() != "Ok") {
    _displayDayPicker();
   }
   return true;
}

// Hour Picker //

void DateTimePicker::_displayHourPicker()
{
  ezMenu hourPickerMenu("Hour");

  hourPickerMenu.buttons(_pickerButtons);
  hourPickerMenu.txtFont(&_pickerFont);
  hourPickerMenu.setCheckType(_pickerCheckType);
  hourPickerMenu.setCheckButtonName(_pickerCheckButtonName);
  hourPickerMenu.upOnFirst("last|up");
  hourPickerMenu.downOnLast("first|down");

  hourPickerMenu.addItem("00");
  hourPickerMenu.addItem("01");
  hourPickerMenu.addItem("02");
  hourPickerMenu.addItem("03");
  hourPickerMenu.addItem("04");
  hourPickerMenu.addItem("05");
  hourPickerMenu.addItem("06");
  hourPickerMenu.addItem("07");
  hourPickerMenu.addItem("08");
  hourPickerMenu.addItem("09");
  hourPickerMenu.addItem("10");
  hourPickerMenu.addItem("11");
  hourPickerMenu.addItem("12");
  hourPickerMenu.addItem("13");
  hourPickerMenu.addItem("14");
  hourPickerMenu.addItem("15");
  hourPickerMenu.addItem("16");
  hourPickerMenu.addItem("17");
  hourPickerMenu.addItem("18");
  hourPickerMenu.addItem("19");
  hourPickerMenu.addItem("20");
  hourPickerMenu.addItem("21");
  hourPickerMenu.addItem("22");
  hourPickerMenu.addItem("23");  

  hourPickerMenu.check(_pickedHour);

  hourPickerMenu.runOnce();
  
  if (hourPickerMenu.pickButton() == "Ok") {    
    String checkedHour = hourPickerMenu.getCheckedItemName();
    if (checkedHour != "") {
      _pickedHour = checkedHour;
    } 
  }
}

bool DateTimePicker::_advancedDisplayHourPicker(ezMenu* callingMenu) {
  if (callingMenu->pickButton() != "Ok") {
    _displayHourPicker();
   }
   return true;
}

// Minute Picker //

void DateTimePicker::_displayMinutePicker()
{
  ezMenu minutePickerMenu("Minute");

  minutePickerMenu.buttons(_pickerButtons);
  minutePickerMenu.txtFont(&_pickerFont);
  minutePickerMenu.setCheckType(_pickerCheckType);
  minutePickerMenu.setCheckButtonName(_pickerCheckButtonName);
  minutePickerMenu.upOnFirst("last|up");
  minutePickerMenu.downOnLast("first|down");

  minutePickerMenu.addItem("00");
  minutePickerMenu.addItem("01");
  minutePickerMenu.addItem("02");
  minutePickerMenu.addItem("03");
  minutePickerMenu.addItem("04");
  minutePickerMenu.addItem("05");
  minutePickerMenu.addItem("06");
  minutePickerMenu.addItem("07");
  minutePickerMenu.addItem("08");
  minutePickerMenu.addItem("09");
  minutePickerMenu.addItem("10");
  minutePickerMenu.addItem("11");
  minutePickerMenu.addItem("12");
  minutePickerMenu.addItem("13");
  minutePickerMenu.addItem("14");
  minutePickerMenu.addItem("15");
  minutePickerMenu.addItem("16");
  minutePickerMenu.addItem("17");
  minutePickerMenu.addItem("18");
  minutePickerMenu.addItem("19");
  minutePickerMenu.addItem("20");
  minutePickerMenu.addItem("21");
  minutePickerMenu.addItem("22");
  minutePickerMenu.addItem("23");
  minutePickerMenu.addItem("24");
  minutePickerMenu.addItem("25");
  minutePickerMenu.addItem("26");
  minutePickerMenu.addItem("27");
  minutePickerMenu.addItem("28");
  minutePickerMenu.addItem("29");
  minutePickerMenu.addItem("30");
  minutePickerMenu.addItem("31");
  minutePickerMenu.addItem("32");
  minutePickerMenu.addItem("33");
  minutePickerMenu.addItem("34");
  minutePickerMenu.addItem("35");
  minutePickerMenu.addItem("36");
  minutePickerMenu.addItem("37");
  minutePickerMenu.addItem("38");
  minutePickerMenu.addItem("39");
  minutePickerMenu.addItem("40");
  minutePickerMenu.addItem("41");
  minutePickerMenu.addItem("42");
  minutePickerMenu.addItem("43");
  minutePickerMenu.addItem("44");
  minutePickerMenu.addItem("45");
  minutePickerMenu.addItem("46");
  minutePickerMenu.addItem("47");
  minutePickerMenu.addItem("48");
  minutePickerMenu.addItem("49");
  minutePickerMenu.addItem("50");
  minutePickerMenu.addItem("51");
  minutePickerMenu.addItem("52");
  minutePickerMenu.addItem("53");
  minutePickerMenu.addItem("54");
  minutePickerMenu.addItem("55");
  minutePickerMenu.addItem("56");
  minutePickerMenu.addItem("57");
  minutePickerMenu.addItem("58");
  minutePickerMenu.addItem("59");

  minutePickerMenu.check(_pickedMinute);

  minutePickerMenu.runOnce();
  
  if (minutePickerMenu.pickButton() == "Ok") {           
    String checkedMinute = minutePickerMenu.getCheckedItemName();
    if (checkedMinute != "") {
      _pickedMinute = checkedMinute;
    } 
  }
}

bool DateTimePicker::_advancedDisplayMinutePicker(ezMenu* callingMenu) {
  if (callingMenu->pickButton() != "Ok") {
    _displayMinutePicker();
   }
   return true;
}

// Minute Picker //

void DateTimePicker::_displaySecondPicker()
{
  ezMenu secondPickerMenu("Second");

  secondPickerMenu.buttons(_pickerButtons);
  secondPickerMenu.txtFont(&_pickerFont);
  secondPickerMenu.setCheckType(_pickerCheckType);
  secondPickerMenu.setCheckButtonName(_pickerCheckButtonName);
  secondPickerMenu.upOnFirst("last|up");
  secondPickerMenu.downOnLast("first|down");

  secondPickerMenu.addItem("00");
  secondPickerMenu.addItem("01");
  secondPickerMenu.addItem("02");
  secondPickerMenu.addItem("03");
  secondPickerMenu.addItem("04");
  secondPickerMenu.addItem("05");
  secondPickerMenu.addItem("06");
  secondPickerMenu.addItem("07");
  secondPickerMenu.addItem("08");
  secondPickerMenu.addItem("09");
  secondPickerMenu.addItem("10");
  secondPickerMenu.addItem("11");
  secondPickerMenu.addItem("12");
  secondPickerMenu.addItem("13");
  secondPickerMenu.addItem("14");
  secondPickerMenu.addItem("15");
  secondPickerMenu.addItem("16");
  secondPickerMenu.addItem("17");
  secondPickerMenu.addItem("18");
  secondPickerMenu.addItem("19");
  secondPickerMenu.addItem("20");
  secondPickerMenu.addItem("21");
  secondPickerMenu.addItem("22");
  secondPickerMenu.addItem("23");
  secondPickerMenu.addItem("24");
  secondPickerMenu.addItem("25");
  secondPickerMenu.addItem("26");
  secondPickerMenu.addItem("27");
  secondPickerMenu.addItem("28");
  secondPickerMenu.addItem("29");
  secondPickerMenu.addItem("30");
  secondPickerMenu.addItem("31");
  secondPickerMenu.addItem("32");
  secondPickerMenu.addItem("33");
  secondPickerMenu.addItem("34");
  secondPickerMenu.addItem("35");
  secondPickerMenu.addItem("36");
  secondPickerMenu.addItem("37");
  secondPickerMenu.addItem("38");
  secondPickerMenu.addItem("39");
  secondPickerMenu.addItem("40");
  secondPickerMenu.addItem("41");
  secondPickerMenu.addItem("42");
  secondPickerMenu.addItem("43");
  secondPickerMenu.addItem("44");
  secondPickerMenu.addItem("45");
  secondPickerMenu.addItem("46");
  secondPickerMenu.addItem("47");
  secondPickerMenu.addItem("48");
  secondPickerMenu.addItem("49");
  secondPickerMenu.addItem("50");
  secondPickerMenu.addItem("51");
  secondPickerMenu.addItem("52");
  secondPickerMenu.addItem("53");
  secondPickerMenu.addItem("54");
  secondPickerMenu.addItem("55");
  secondPickerMenu.addItem("56");
  secondPickerMenu.addItem("57");
  secondPickerMenu.addItem("58");
  secondPickerMenu.addItem("59");

  secondPickerMenu.check(_pickedSecond);

  secondPickerMenu.runOnce();
  
  if (secondPickerMenu.pickButton() == "Ok") {           
    String checkedSecond = secondPickerMenu.getCheckedItemName();
    if (checkedSecond != "") {
      _pickedSecond = checkedSecond;
    } 
  }
}

bool DateTimePicker::_advancedDisplaySecondPicker(ezMenu* callingMenu) {
  if (callingMenu->pickButton() != "Ok") {
    _displaySecondPicker();
   }
   return true;
}

/////////////////
// Picker Menu //
/////////////////

String DateTimePicker::_displayPickerMenu(String pickerName, bool displayCancel, bool onlyPickTime, bool displaySeconds)
{ 
  ezMenu pickerMenu(pickerName);

  if (displayCancel) {
    pickerMenu.buttons("up#Ok|Set#Select##down#Back|Cancel"); 
  } else {
    pickerMenu.buttons("up#Ok|Set#Select##down#Back|Menu"); 
  }  
  pickerMenu.txtFont(&FreeMonoBold12pt7b); 

  String yearItemText = "Year:   " + _pickedYear; 
  String monthItemText = "Month:  " + _pickedMonth;
  String dayItemText = "Day:    " + _pickedDay;
  String hourItemText = "Hour:   " + _pickedHour;
  String minuteItemText = "Minute: " + _pickedMinute;
  String secondItemText = "Second: " + _pickedSecond;

  // Advanced functions are required, because the "Ok" button is reserved for saving and "Select" button is used to reach sub-level picker menus
  // In M5ez all non navigation buttons are action buttons 
  if (!onlyPickTime) {
    pickerMenu.addItem(yearItemText, NULL, _advancedDisplayYearPicker, NULL);
    pickerMenu.addItem(monthItemText, NULL, _advancedDisplayMonthPicker, NULL);
    pickerMenu.addItem(dayItemText, NULL, _advancedDisplayDayPicker, NULL);
  }
  pickerMenu.addItem(hourItemText, NULL, _advancedDisplayHourPicker, NULL);
  pickerMenu.addItem(minuteItemText, NULL, _advancedDisplayMinutePicker, NULL);
  if (displaySeconds) {
    pickerMenu.addItem(secondItemText, NULL, _advancedDisplaySecondPicker, NULL);  
  }
  
  pickerMenu.runOnce();
 
  if (pickerMenu.pickButton() == "Back") {
    return "Back";    
  } else if (pickerMenu.pickButton() == "Ok") {
    return "Ok";
  } 
  return "";
}

time_t DateTimePicker::runOnce(String pickerName, time_t initialTime, bool displayCancel, bool onlyPickTime, bool displaySeconds)
{
  _pickedYear = String(year(initialTime));
  _pickedMonth = String(zeropad(month(initialTime), 2));
  _pickedDay = String(zeropad(day(initialTime), 2));
  _pickedHour = String(zeropad(hour(initialTime), 2));
  _pickedMinute = String(zeropad(minute(initialTime), 2));
  _pickedSecond = String(zeropad(second(initialTime), 2));

  _pickerButtons = "up#Ok#Select##down#Back";
  _pickerFont = FreeMonoBold12pt7b;
  _pickerCheckType = CHECK_TYPE_RADIO;
  _pickerCheckButtonName = "Select";

  while (true) {
    String selectedAction = _displayPickerMenu(pickerName, displayCancel, onlyPickTime, displaySeconds);
    if (selectedAction == "Back") {
      return 0;
    } else if (selectedAction == "Ok") {
      break;
    }
  }

  uint8_t seconds = 0;
  if (displaySeconds) {
    seconds = _pickedSecond.toInt();
  } 

  return makeTime(_pickedHour.toInt(), _pickedMinute.toInt(), seconds, _pickedDay.toInt(), _pickedMonth.toInt(), _pickedYear.toInt());
}