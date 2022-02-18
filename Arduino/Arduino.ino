#include "Arduino_menu.h"

void setup() {
    setupMenu();

}

void loop() {
    taskManager.runLoop();

}


void CALLBACK_FUNCTION onSaveSettings(int id) {
    // TODO - your menu change code
}



void CALLBACK_FUNCTION onStartToasting(int id) {
    // TODO - your menu change code
}


void CALLBACK_FUNCTION onNameChanged(int id) {
    // TODO - your menu change code
}
