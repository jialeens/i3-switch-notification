#include <iostream>
#include <i3ipc++/ipc.hpp>
#include <gtk/gtk.h>

using namespace std;

bool show = false;
long showTime;
GtkWidget *window;
GtkWidget *label;
std::string workspace_name;
bool running = true;

void *i3_workspace_notification() {
    i3ipc::connection conn;
    conn.subscribe(i3ipc::ET_WORKSPACE);
    conn.signal_workspace_event.connect([=](const i3ipc::workspace_event_t &ev) {
        if (ev.current && ev.type == i3ipc::WorkspaceEventType::FOCUS) {
            showTime = g_get_real_time() + 100 * 0000;
            workspace_name = ev.current->name;
        }
    });
    while (running) {
        conn.handle_event();
    }
}

void showNotification() {
    char *output;
    if (!show) {
        gtk_widget_show(window);
    }
    show = true;
    output = g_markup_printf_escaped("<span foreground='white' font_desc='16'>%s</span>", workspace_name.c_str());
    gtk_label_set_markup(GTK_LABEL(label), output);
    free(output);
}

void hideNotification() {
    if (show) {
        gtk_widget_hide(window);
    }
    show = false;
}

void *window_controller() {
    while (running) {
        guint64 current = g_get_real_time();
        if (current < showTime) {
            gdk_threads_add_idle((GSourceFunc)showNotification, nullptr);

        } else {
            gdk_threads_add_idle((GSourceFunc)hideNotification, nullptr);
        }
        usleep(100000);
    }
}
gboolean button_pressed(GtkWidget *eventbox,
                        GdkEventButton *event,
                        GtkLabel *label1) {
    if (event->type == GDK_BUTTON_PRESS) {  // 双击
        if (show) {
            gtk_widget_hide(window);
        }
        showTime = g_get_real_time();
        show = false;
    }
    return FALSE;
}
int main(int argc, char *argv[]) {
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
    gtk_label_set_markup(GTK_LABEL(label),
                         "<span foreground='white' font_desc='16'></span>");
    gtk_label_set_width_chars(GTK_LABEL(label), 20);
    GtkWidget *box = gtk_event_box_new();
    gtk_container_add(GTK_CONTAINER(box), label);
    g_signal_connect(box, "button_press_event",
                     G_CALLBACK(button_pressed), (gpointer)label);
    gtk_container_add(GTK_CONTAINER(window), box);
    gtk_window_set_keep_above(GTK_WINDOW(window), true);
    gtk_window_set_modal(GTK_WINDOW(window), true);
    gtk_widget_show_all(window);
    gtk_widget_hide(window);
    g_thread_new("i3IPC", (GThreadFunc)i3_workspace_notification, nullptr);
    g_thread_new("WinControl", (GThreadFunc)window_controller, nullptr);
    gtk_main();
    return 0;
}
