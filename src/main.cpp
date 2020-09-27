#include <iostream>
#include <i3ipc++/ipc.hpp>


int main() {
    i3ipc::connection  conn;
    conn.subscribe(i3ipc::ET_WORKSPACE | i3ipc::ET_WINDOW | i3ipc::ET_BINDING);
    conn.signal_workspace_event.connect([](const i3ipc::workspace_event_t&  ev) {
        std::cout << "workspace_event: " << (char)ev.type << std::endl;
        if (ev.current) {
            std::cout << "\tSwitched to #" << ev.current->num << " - \"" << ev.current->name << '"' << std::endl;
        }
    });
    while (true) {
        conn.handle_event();
    }
    return 0;
}
