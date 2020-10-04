//
// Created by jiale on 2020/9/30.
//
#include <iostream>
#include <gtk/gtk.h>
#include <i3ipc++/ipc.hpp>
#include "NotificationMainWindow.h"
using namespace std;
#ifndef I3_SWITCH_NOTIFICATION_NOTIFICATION_H
#define I3_SWITCH_NOTIFICATION_NOTIFICATION_H

class NotificationController {
   public:
    static bool running;
    explicit NotificationController(NotificationMainWindow *mainWindow);
    static int *i3WorkspaceNotification(NotificationMainWindow *mainWindow);
    static int *windowController(NotificationMainWindow *mainWindow);
    const int MICROSECOND_UNIT = 1000 * 1000;
};

#endif  //I3_SWITCH_NOTIFICATION_NOTIFICATION_H
