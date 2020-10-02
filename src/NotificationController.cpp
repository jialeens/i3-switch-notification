//
// Created by jiale on 2020/9/30.
//

#include "NotificationController.h"

int *NotificationController::i3WorkspaceNotification(NotificationMainWindow *mainWindow) {
    i3ipc::connection conn;
    conn.subscribe(i3ipc::ET_WORKSPACE);
    conn.signal_workspace_event.connect([=](const i3ipc::workspace_event_t &ev) {
        if (ev.current && ev.type == i3ipc::WorkspaceEventType::FOCUS) {
            mainWindow->setShowTime(g_get_real_time() + 1000 * 1000);
            mainWindow->setWorkspaceName(ev.current->name);
        }
    });
    while (mainWindow->running) {
        conn.handle_event();
    }
    return nullptr;
}

int *NotificationController::windowController(NotificationMainWindow *mainWindow) {
    while (mainWindow->running) {
        guint64 current = g_get_real_time();
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
    g_thread_new("WinControl", (GThreadFunc)windowController, mainWindow);
    g_thread_new("i3IPC", (GThreadFunc)i3WorkspaceNotification, mainWindow);
}
