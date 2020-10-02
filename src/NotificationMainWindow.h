//
// Created by jiale on 2020/9/30.
//
#include <gtk/gtk.h>
#include <iostream>
using namespace std;
#ifndef I3_SWITCH_NOTIFICATION_MAINWINDOW_H
#define I3_SWITCH_NOTIFICATION_MAINWINDOW_H

class NotificationMainWindow {
   public:
    explicit NotificationMainWindow(int argc, char *argv[]);
    int start();
    static gboolean buttonPressed(GtkWidget *eventbox,
                                  GdkEventButton *event,
                                  NotificationMainWindow *mainWindow);
    static int showNotification(NotificationMainWindow *mainWindow);
    static int hideNotification(NotificationMainWindow *mainWindow);
    int setShowTime(gint64 time);
    int setWorkspaceName(string name);
    gint64 getShowTime();
    bool running = true;

   private:
    GtkWidget *label;
    GtkWidget *window;
    gint64 showTime;
    bool show;
    string workspaceName;
};

#endif  //I3_SWITCH_NOTIFICATION_MAINWINDOW_H
