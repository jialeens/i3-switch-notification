//
// Created by jiale on 2020/9/30.
//
#include "NotificationMainWindow.h"
using namespace std;

NotificationMainWindow::NotificationMainWindow() {
    show = false;
    showTime = 0;
    buildUI();
}

void NotificationMainWindow::buildUI() {
    GtkWidget *box;
    GtkWidget *symbolBox;
    GtkWidget *nameBox;

    //load style
    loadStyle();

    // init window
    window = gtk_window_new(GTK_WINDOW_POPUP);

    gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
    gtk_window_set_title(GTK_WINDOW(window), "window");
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER_ALWAYS);

    Settings *pSettings = Settings::getSettings();
    gint windowWidth = pSettings->getWindowWidth();
    gint windowHeight = pSettings->getWindowHeight();
    if (windowWidth != 0 && windowHeight != 0) {
        gtk_widget_set_size_request(window, windowWidth, windowHeight);
    }

    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_destroy_with_parent(GTK_WINDOW(window), TRUE);
    gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
    gtk_window_set_modal(GTK_WINDOW(window), TRUE);

    gtk_widget_set_app_paintable(window, TRUE);
    gtk_widget_set_name(window, "mainWindow");

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    nameLabel = gtk_label_new("nameLabel");
    gtk_widget_set_name(nameLabel, "nameLabel");
    symbolLabel = gtk_label_new("symbolLabel");
    gtk_widget_set_name(symbolLabel, "symbolLabel");
    symbolBox = gtk_event_box_new();
    nameBox = gtk_event_box_new();

    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_container_add(GTK_CONTAINER(box), symbolBox);
    gtk_container_add(GTK_CONTAINER(box), nameBox);
    gtk_container_add(GTK_CONTAINER(symbolBox), symbolLabel);
    gtk_container_add(GTK_CONTAINER(nameBox), nameLabel);

    g_signal_connect(box, "button_press_event", G_CALLBACK(buttonPressed), (gpointer)this);
    g_signal_connect(window, "delete-event", gtk_main_quit, NULL);
    if (pSettings->getWindowTransparent()) {
        NotificationMainWindow::transparent(window);
    }

    gtk_widget_show_all(window);
    gtk_widget_hide(window);
}

void NotificationMainWindow::transparent(GtkWidget *window) {
    gtk_widget_set_app_paintable(window, true);
    gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(window, "draw", G_CALLBACK(draw), NULL);
    g_signal_connect(window, "screen-changed", G_CALLBACK(screenChanged), NULL);
    g_signal_connect(window, "button-press-event", G_CALLBACK(clicked), NULL);
    screenChanged(window, nullptr, nullptr);
}

static bool supportsAlpha = false;
void NotificationMainWindow::screenChanged(GtkWidget *widget, GdkScreen *old, gpointer data) {
    GdkScreen *screen = gtk_widget_get_screen(widget);
    GdkVisual *visual = gdk_screen_get_rgba_visual(screen);
    if (!visual) {
        fprintf(stderr, "Your screen doesn't support alpha!\n");
        fprintf(stderr, "Maybe you lack a compositor or the compositor doesn't support transparencies\n");
        visual = gdk_screen_get_system_visual(screen);
    } else {
        supportsAlpha = true;
    }
    gtk_widget_set_visual(widget, visual);
}
bool NotificationMainWindow::draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    cairo_t *newCr = gdk_cairo_create(gtk_widget_get_window(widget));
    if (supportsAlpha) {
        cairo_set_source_rgba(newCr, 0, 0, 0, 0);
    } else {
        cairo_set_source_rgb(newCr, 0, 0, 0);
    }
    cairo_set_operator(newCr, CAIRO_OPERATOR_SOURCE);
    cairo_paint(newCr);
    //    cairo_destroy(newCr);
    return false;
}

void NotificationMainWindow::clicked(GtkWindow *window, GdkEventButton *event, gpointer data) {
    gtk_window_set_decorated(window, !gtk_window_get_decorated(window));
}
void NotificationMainWindow::loadStyle() {
    GtkCssProvider *provider;
    GdkDisplay *display;
    GdkScreen *screen;
    /// ***
    GFile *css_fp;
    css_fp = g_file_new_for_path(stylePath);
    /// ***
    provider = gtk_css_provider_new();
    display = gdk_display_get_default();
    screen = gdk_display_get_default_screen(display);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    GError *error = nullptr;

    gchar *cssData = buildDefaultCss();

    if (Settings::getSettings()->isLoadStyleFromCssFile()) {
        gtk_css_provider_load_from_file(provider, css_fp, &error);
    } else {
        gtk_css_provider_load_from_data(provider, cssData, -1, &error);
    }
    if (error) {
        g_error_free(error);
    }
    if (css_fp) {
        g_object_unref(css_fp);
    }
}
gchar *NotificationMainWindow::buildDefaultCss() const {
    Settings *pSettings = Settings::getSettings();
    gchar *cssData = g_strdup_printf("#nameLabel {\n"
                                     "    font-size: %dpx;\n"
                                     "    color: %s;\n"
                                     "    text-shadow: 2px 2px 2px rgba(0, 0, 0, 0.51);\n"
                                     "    font-family: %s\n"
                                     "}\n"
                                     "#symbolLabel {\n"
                                     "    font-size: %dpx;\n"
                                     "    color: %s;\n"
                                     "    text-shadow: 2px 2px 2px rgba(0, 0, 0, 0.51);\n"
                                     "    font-family: %s;\n"
                                     "}\n",
                                     pSettings->getNameFontSize(),
                                     pSettings->getNameFontColor(),
                                     pSettings->getNameFontFamily(),
                                     pSettings->getSymbolFontSize(),
                                     pSettings->getSymbolFontColor(),
                                     pSettings->getSymbolFontFamily());
    return cssData;
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
    gchar *name;
    gchar *symbol;
    if (!mainWindow->isShow()) {
        gtk_widget_show(mainWindow->window);
    }
    name = g_strdup_printf("%s", mainWindow->workspaceName.c_str());
    symbol = Settings::getSettings()->getWorkspaceSymbol(name);
    gtk_label_set_text(GTK_LABEL(mainWindow->symbolLabel), symbol);
    gtk_label_set_text(GTK_LABEL(mainWindow->nameLabel), name);
    mainWindow->setShow(true);
    g_free(name);
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
