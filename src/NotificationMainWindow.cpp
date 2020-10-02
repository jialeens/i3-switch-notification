//
// Created by jiale on 2020/9/30.
//
#include <gtk/gtk.h>
#include "NotificationMainWindow.h"

NotificationMainWindow::NotificationMainWindow(int argc, char *argv[]) {
    this->show = false;
    this->showTime = 0;
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_POPUP);
    gtk_widget_realize(window);
    gtk_widget_set_opacity(window, 0.7);
    gtk_window_set_keep_above(GTK_WINDOW(window), true);
    gtk_window_set_title(GTK_WINDOW(window), "window");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(window), false);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(window), true);
    label = gtk_label_new("");
    gtk_label_set_markup(GTK_LABEL(label), "<span foreground='white' font_desc='16'></span>");
    gtk_label_set_width_chars(GTK_LABEL(label), 20);
    GtkWidget *box = gtk_event_box_new();
    gtk_container_add(GTK_CONTAINER(box), label);
    g_signal_connect(box, "button_press_event", G_CALLBACK(buttonPressed), (gpointer)this);
    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_window_set_keep_above(GTK_WINDOW(window), true);
    gtk_window_set_modal(GTK_WINDOW(window), true);
}
int NotificationMainWindow::start() {
    gtk_widget_show_all(window);
    gtk_widget_hide(window);
    gtk_main();
    return 0;
}

gboolean NotificationMainWindow::buttonPressed(GtkWidget *eventbox,
                                               GdkEventButton *event,
                                               NotificationMainWindow *mainWindow) {
    if (event->type == GDK_BUTTON_PRESS) {
        if (mainWindow->show) {
            gtk_widget_hide(mainWindow->window);
        }
        mainWindow->showTime = g_get_real_time();
        mainWindow->show = false;
    }
    return FALSE;
}

int NotificationMainWindow::showNotification(NotificationMainWindow *mainWindow) {
    char *output;
    if (!mainWindow->show) {
        gtk_widget_show(mainWindow->window);
    }
    mainWindow->show = true;
    output = g_markup_printf_escaped("<span foreground='white' font_desc='16'>%s</span>", mainWindow->workspaceName.c_str());
    gtk_label_set_markup(GTK_LABEL(mainWindow->label), output);
    free(output);
    return 0;
}

int NotificationMainWindow::hideNotification(NotificationMainWindow *mainWindow) {
    if (mainWindow->show) {
        gtk_widget_hide(mainWindow->window);
    }
    mainWindow->show = false;
    return 0;
}
int NotificationMainWindow::setWorkspaceName(string name) {
    workspaceName = std::move(name);
    return 0;
}
int NotificationMainWindow::setShowTime(gint64 time) {
    return showTime = time;
}
gint64 NotificationMainWindow::getShowTime() {
    return showTime;
}
