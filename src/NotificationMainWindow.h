//
// Created by jiale on 2020/9/30.
//

#ifndef I3_SWITCH_NOTIFICATION_NOTIFACTIONMAINWINDOW_H
#define I3_SWITCH_NOTIFICATION_NOTIFACTIONMAINWINDOW_H
#include <gtk/gtk.h>
#include <iostream>
#include "Settings.h"
class NotificationMainWindow {
   public:
    NotificationMainWindow();
    void buildUI();
    void setShowTime(gint64 time);
    void setWorkspaceName(std::string name);
    gint64 getShowTime();
    bool isShow();
    void setShow(bool setShow);
    bool isRunning();
    // static
    static gboolean buttonPressed(GtkWidget *eventbox, GdkEventButton *event, NotificationMainWindow *mainWindow);
    static int showNotification(NotificationMainWindow *mainWindow);
    static int hideNotification(NotificationMainWindow *mainWindow);

   private:
    GtkWidget *window{};
    GtkWidget *nameLabel{};
    GtkWidget *symbolLabel{};
    gint64 showTime{};
    bool show{};
    std::string workspaceName{};
    bool running = TRUE;
    const gchar *stylePath = "../share/i3-switch-notification/css/style.css";
    void loadStyle();
    static void transparent(GtkWidget *window);
    static void clicked(GtkWindow *window, GdkEventButton *event, gpointer data);
    static void screenChanged(GtkWidget *widget, GdkScreen *old, gpointer data);
    static bool draw(GtkWidget *widget, cairo_t *cr, gpointer data);
};

#endif  //I3_SWITCH_NOTIFICATION_MAINWINDOW_H
