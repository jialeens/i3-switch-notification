#include <iostream>

#include <gtk/gtk.h>
#include "NotificationMainWindow.h"
#include "NotificationController.h"
using namespace std;

int main(int argc, char *argv[]) {
    Settings::getSettings();
    NotificationMainWindow *mainWindow;
    mainWindow = new NotificationMainWindow(argc, argv);
    new NotificationController(mainWindow);
    mainWindow->start();
    return 0;
}
