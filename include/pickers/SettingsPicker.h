#include "Arduino.h"
#include "M5ez.h"
#include "ezTime.h"

#include "screens/M5ezWatchScreen.h"

class SettingsPicker : public M5ezWatchScreen {
public:    
    static void runOnce(String pickerName);
private:
    static String _displayPickerMenu(String pickerName);

    /////////////
    // Pickers //
    /////////////

    // General picker settings
    static String _pickerButtons;
    static GFXfont _pickerFont;
    static int8_t _pickerCheckType;
    static String _pickerCheckButtonName;

    static void _displayWifiPicker();
    static bool _advancedDisplayWifiPicker(ezMenu* callingMenu);    
    static void _displayClockPicker();
    static bool _advancedDisplayClockPicker(ezMenu* callingMenu);
    static void _displayBatteryPicker();
    static bool _advancedDisplayBatteryPicker(ezMenu* callingMenu);    
    static void _displayBacklightPicker();
    static bool _advancedDisplayBacklightPicker(ezMenu* callingMenu);
    static void _displayThemePicker();
    static bool _advancedDisplayThemePicker(ezMenu* callingMenu);
    static void _displayFactoryDefaultsPicker();
    static bool _advancedDisplayFactoryDefaultsPicker(ezMenu* callingMenu);
};