#include <iostream>

#include <gtk/gtk.h>
#include "NotificationMainWindow.h"
#include "NotificationController.h"
using namespace std;

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    Settings::getSettings();
    new NotificationController(new NotificationMainWindow());
    gtk_main();
    cout << "end" << endl;
    return 0;
}
