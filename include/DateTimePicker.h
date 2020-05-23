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
    static time_t runOnce(String pickerName, time_t initialTime, bool onlyPickTime = false);
private:
    static String _displayPickerMenu(String pickerName, bool onlyPickTime = false);
    
    // The currently picked date and time values
    static String _pickedYear;
    static String _pickedMonth;
    static String _pickedDay;
    static String _pickedHour;
    static String _pickedMinute;
    static String _pickedSecond;

    /////////////
    // Pickers //
    /////////////

    // General picker settings
    static String _pickerButtons;
    static GFXfont _pickerFont;
    static int8_t _pickerCheckType;
    static String _pickerCheckButtonName;

    static void _displayYearPicker();
    static bool _advancedDisplayYearPicker(ezMenu* callingMenu);
    static void _displayMonthPicker();
    static bool _advancedDisplayMonthPicker(ezMenu* callingMenu);
    static void _displayDayPicker();
    static bool _advancedDisplayDayPicker(ezMenu* callingMenu);
    static void _displayHourPicker();
    static bool _advancedDisplayHourPicker(ezMenu* callingMenu);
    static void _displayMinutePicker();
    static bool _advancedDisplayMinutePicker(ezMenu* callingMenu);
    static void _displaySecondPicker();
    static bool _advancedDisplaySecondPicker(ezMenu* callingMenu);  
};