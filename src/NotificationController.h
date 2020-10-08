//
// Created by jiale on 2020/9/30.
//

#ifndef I3_SWITCH_NOTIFICATION_NOTIFICATIONCONTROLLER_H
#define I3_SWITCH_NOTIFICATION_NOTIFICATIONCONTROLLER_H

#include <iostream>
#include <gtk/gtk.h>
#include <i3ipc++/ipc.hpp>
#include "Consts.h"
#include "NotificationMainWindow.h"
#include "Settings.h"

class NotificationController {
   public:
    explicit NotificationController(NotificationMainWindow *mainWindow);
    static void *i3WorkspaceNotification(NotificationMainWindow *mainWindow);
    static void *windowController(NotificationMainWindow *mainWindow);

   private:
};

#endif  //I3_SWITCH_NOTIFICATION_NOTIFICATION_H
