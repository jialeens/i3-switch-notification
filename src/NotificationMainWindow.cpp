//
// Created by jiale on 2020/9/30.
//
#include "NotificationMainWindow.h"
using namespace std;

NotificationMainWindow::NotificationMainWindow(int argc, char *argv[]) {
    show = false;
    showTime = 0;
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_POPUP);
    gtk_widget_realize(window);
    loadStyle();
    gtk_widget_set_name(window, "mainWindow");
    gtk_widget_set_opacity(window, 0.5);
    gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
    gtk_window_set_title(GTK_WINDOW(window), "window");
    gtk_widget_set_app_paintable(window, TRUE);
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(window), TRUE);
    workspaceNameLabel = gtk_label_new(nullptr);
    workspaceNameSymbol = gtk_label_new(nullptr);
    gtk_label_set_markup(GTK_LABEL(workspaceNameLabel), "<span foreground='white' font_desc='16'></span>");
    gtk_label_set_width_chars(GTK_LABEL(workspaceNameLabel), 20);
    GtkWidget *symbolBox = gtk_event_box_new();
    GtkWidget *nameBox = gtk_event_box_new();
    gtk_container_add(GTK_CONTAINER(symbolBox), workspaceNameSymbol);
    gtk_container_add(GTK_CONTAINER(nameBox), workspaceNameLabel);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_container_add(GTK_CONTAINER(box), symbolBox);
    gtk_container_add(GTK_CONTAINER(box), nameBox);
    gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
    gtk_window_set_modal(GTK_WINDOW(window), TRUE);

    g_signal_connect(symbolBox, "button_press_event", G_CALLBACK(buttonPressed), (gpointer)this);
    g_signal_connect(nameBox, "button_press_event", G_CALLBACK(buttonPressed), (gpointer)this);
}

void NotificationMainWindow::start() {
    gtk_widget_show_all(window);
    gtk_widget_hide(window);
    gtk_main();
}

void NotificationMainWindow::loadStyle() {
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;

    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    GError *error = nullptr;
    gtk_css_provider_load_from_file(provider, g_file_new_for_path(stylePath), &error);
    g_object_unref(provider);
}

gboolean NotificationMainWindow::buttonPressed(GtkWidget *eventbox,
                                               GdkEventButton *event,
                                               NotificationMainWindow *mainWindow) {
    if (eventbox && event->type == GDK_BUTTON_PRESS) {
        if (mainWindow->isShow()) {
            gtk_widget_hide(mainWindow->window);
        }
        mainWindow->setShowTime(g_get_real_time());
        mainWindow->setShow(FALSE);
    }
    return FALSE;
}

int NotificationMainWindow::showNotification(NotificationMainWindow *mainWindow) {
    char *nameLabelText;
    char *symbolLabelText;
    if (!mainWindow->isShow()) {
        gtk_widget_show(mainWindow->window);
    }
    Settings *settings = Settings::getSettings();
    gchar *symbol = settings->getWorkspaceSymbol(g_strdup_printf("%s", mainWindow->workspaceName.c_str()));
    gchar *symbolFontFamily = settings->getSymbolFontFamily();
    gint symbolFontSize = settings->getSymbolFontSize();

    symbolLabelText = g_markup_printf_escaped(
        "<span foreground='white' font_desc='%d' font_family='%s'>%s</span>",
        symbolFontSize, symbolFontFamily, symbol);
    gtk_label_set_markup(GTK_LABEL(mainWindow->workspaceNameSymbol), symbolLabelText);

    gchar *nameFonFamily = settings->getNameFontFamily();
    gint nameFontSize = settings->getNameFontSize();
    nameLabelText = g_markup_printf_escaped(
        "<span foreground='white' font_desc='%d' font_family='%s'>%s</span>",
        nameFontSize, nameFonFamily, mainWindow->workspaceName.c_str());
    gtk_label_set_markup(GTK_LABEL(mainWindow->workspaceNameLabel), nameLabelText);
    g_free(symbolLabelText);
    mainWindow->setShow(true);
    return 0;
}

int NotificationMainWindow::hideNotification(NotificationMainWindow *mainWindow) {
    if (mainWindow->isShow()) {
        gtk_widget_hide(mainWindow->window);
    }
    mainWindow->setShow(false);
    return 0;
}
void NotificationMainWindow::setWorkspaceName(string name) {
    workspaceName = std::move(name);
}
void NotificationMainWindow::setShowTime(gint64 time) {
    showTime = time;
}
gint64 NotificationMainWindow::getShowTime() {
    return this->showTime;
}
bool NotificationMainWindow::isRunning() {
    return running;
}
bool NotificationMainWindow::isShow() {
    return show;
}
void NotificationMainWindow::setShow(bool setShow) {
    this->show = setShow;
}
