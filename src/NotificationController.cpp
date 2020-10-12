//
// Created by jiale on 2020/9/30.
//

#include "NotificationController.h"
using namespace std;

void *NotificationController::i3WorkspaceNotification(NotificationMainWindow *mainWindow) {
    i3ipc::connection conn;
    conn.subscribe(i3ipc::ET_WORKSPACE);
    conn.signal_workspace_event.connect([=](const i3ipc::workspace_event_t &ev) {
        if (ev.current && ev.type == i3ipc::WorkspaceEventType::FOCUS) {
            mainWindow->setShowTime(g_get_real_time() + NOTIFICATION_SHOW_TIME * 1000 * 1000);
            mainWindow->setWorkspaceName(ev.current->name);
        }
    });
    while (mainWindow->isRunning()) {
        conn.handle_event();
    }
    return nullptr;
}

void *NotificationController::windowController(NotificationMainWindow *mainWindow) {
    guint64 current;
    while (mainWindow->isRunning()) {
        current = g_get_real_time();
        if (current < mainWindow->getShowTime()) {
            gdk_threads_add_idle((GSourceFunc)NotificationMainWindow::showNotification, mainWindow);
        } else {
            gdk_threads_add_idle((GSourceFunc)NotificationMainWindow::hideNotification, mainWindow);
        }
        usleep(0.1 * MICROSECOND_UNIT);
    }
    return nullptr;
}

NotificationController::NotificationController(NotificationMainWindow *mainWindow) {
    g_thread_new("windowController", (GThreadFunc)windowController, mainWindow);
    g_thread_new("i3WorkspaceNotification", (GThreadFunc)i3WorkspaceNotification, mainWindow);
}
