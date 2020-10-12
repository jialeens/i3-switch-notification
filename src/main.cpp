#include <iostream>

#include <gtk/gtk.h>
#include "NotificationMainWindow.h"
#include "NotificationController.h"
using namespace std;

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    Settings::getSettings();
    NotificationMainWindow *mainWindow;
    mainWindow = new NotificationMainWindow();
    new NotificationController(mainWindow);
    gtk_main();
    return 0;
}
