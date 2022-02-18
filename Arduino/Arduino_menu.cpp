/*
    The code in this file uses open source libraries provided by thecoderscorner

    DO NOT EDIT THIS FILE, IT WILL BE GENERATED EVERY TIME YOU USE THE UI DESIGNER
    INSTEAD EITHER PUT CODE IN YOUR SKETCH OR CREATE ANOTHER SOURCE FILE.

    All the variables you may need access to are marked extern in this file for easy
    use elsewhere.
 */

#include <tcMenu.h>
#include "Arduino_menu.h"
#include "ThemeMonoBordered.h"

// Global variable declarations
const PROGMEM  ConnectorLocalInfo applicationInfo = { "aaa", "fddaa423-cb5c-4024-8f67-a9742f4457f3" };
ArduinoEEPROMAbstraction glArduinoEeprom(&EEPROM);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C gfx(U8G2_R0, U8X8_PIN_NONE, 5, 4);
U8g2Drawable gfxDrawable(&gfx);
GraphicsDeviceRenderer renderer(30, applicationInfo.name, &gfxDrawable);

// Global Menu Item declarations
const PROGMEM AnyMenuInfo minfoSettingsSaveSettings = { "SaveSettings", 9, 0xffff, 0, onSaveSettings };
ActionMenuItem menuSettingsSaveSettings(&minfoSettingsSaveSettings, NULL);
RENDERING_CALLBACK_NAME_INVOKE(fnSettingsSerialNumberRtCall, largeNumItemRenderFn, "Serial Number", 7, NO_CALLBACK)
EditableLargeNumberMenuItem menuSettingsSerialNumber(fnSettingsSerialNumberRtCall, 8, 8, 0, true, &menuSettingsSaveSettings);
RENDERING_CALLBACK_NAME_INVOKE(fnSettingsUserNameRtCall, textItemRenderFn, "User Name", 16, onNameChanged)
TextMenuItem menuSettingsUserName(fnSettingsUserNameRtCall, 7, 5, &menuSettingsSerialNumber);
const PROGMEM BooleanMenuInfo minfoSettingsSafetyLock = { "Safety lock", 6, 15, 1, NO_CALLBACK, NAMING_TRUE_FALSE };
BooleanMenuItem menuSettingsSafetyLock(&minfoSettingsSafetyLock, false, &menuSettingsUserName);
RENDERING_CALLBACK_NAME_INVOKE(fnSettingsRtCall, backSubItemRenderFn, "Settings", -1, NO_CALLBACK)
const PROGMEM SubMenuInfo minfoSettings = { "Settings", 5, 0xffff, 0, NO_CALLBACK };
BackMenuItem menuBackSettings(fnSettingsRtCall, &menuSettingsSafetyLock);
SubMenuItem menuSettings(&minfoSettings, &menuBackSettings, NULL);
const PROGMEM AnyMenuInfo minfoStartToasting = { "Start toasting", 4, 0xffff, 0, onStartToasting };
ActionMenuItem menuStartToasting(&minfoStartToasting, &menuSettings);
const PROGMEM BooleanMenuInfo minfoFrozen = { "Frozen", 3, 6, 1, NO_CALLBACK, NAMING_YES_NO };
BooleanMenuItem menuFrozen(&minfoFrozen, false, &menuStartToasting);
const char enumStrType_0[] PROGMEM = "Bread";
const char enumStrType_1[] PROGMEM = "Bagel";
const char enumStrType_2[] PROGMEM = "Pancake";
const char* const enumStrType[] PROGMEM  = { enumStrType_0, enumStrType_1, enumStrType_2 };
const PROGMEM EnumMenuInfo minfoType = { "Type", 2, 4, 2, NO_CALLBACK, enumStrType };
EnumMenuItem menuType(&minfoType, 0, &menuFrozen);
const PROGMEM AnalogMenuInfo minfoToasterPower = { "Toaster power", 1, 2, 10, NO_CALLBACK, 0, 1, "" };
AnalogMenuItem menuToasterPower(&minfoToasterPower, 0, &menuType);

void setupMenu() {
    // First we set up eeprom and authentication (if needed).
    menuMgr.setEepromRef(&glArduinoEeprom);
    // Code generated by plugins.
    gfx.begin();
    renderer.setUpdatesPerSecond(10);
    switches.initialiseInterrupt(internalDigitalIo(), false);
    menuMgr.initForUpDownOk(&renderer, &menuToasterPower, 6, 7, 8);
    menuMgr.setBackButton(5);
    renderer.setTitleMode(BaseGraphicalRenderer::TITLE_FIRST_ROW);
    renderer.setUseSliderForAnalog(false);
    installMonoBorderedTheme(renderer, MenuFontDef(u8g2_font_bitcasual_t_all, 8), MenuFontDef(u8g2_font_finderskeepers_tf, 1), true);
}

