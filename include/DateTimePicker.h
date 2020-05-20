#include "Arduino.h"
#include "M5ez.h"
#include "ezTime.h"

#define PICKER_SCREEN_MAIN_MENU     9100
#define PICKER_SCREEN_PICKER_MENU   9200
#define PICKER_SCREEN_YEAR_PICKER   9300
#define PICKER_SCREEN_MONTH_PICKER  9400
#define PICKER_SCREEN_DAY_PICKER    9500
#define PICKER_SCREEN_HOUR_PICKER   9600
#define PICKER_SCREEN_MINUTE_PICKER 9700

class DateTimePicker {
public:    
    static time_t runOnce(String pickerName, time_t alarmTime);
private:
    static String displayPickerMenu(String pickerName);
    
    // The currently picked date and time values
    static String _pickedYear;
    static String _pickedMonth;
    static String _pickedDay;
    static String _pickedHour;
    static String _pickedMinute;

    /////////////
    // Pickers //
    /////////////

    // General picker settings
    static String _pickerButtons;
    static GFXfont _pickerFont;
    static int8_t _pickerCheckType;
    static String _pickerCheckButtonName;

    static void displayYearPicker();
    static bool advancedDisplayYearPicker(ezMenu* callingMenu);
    static void displayMonthPicker();
    static bool advancedDisplayMonthPicker(ezMenu* callingMenu);
    static void displayDayPicker();
    static bool advancedDisplayDayPicker(ezMenu* callingMenu);
    static void displayHourPicker();
    static bool advancedDisplayHourPicker(ezMenu* callingMenu);
    static void displayMinutePicker();
    static bool advancedDisplayMinutePicker(ezMenu* callingMenu);  
};