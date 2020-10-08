//
// Created by jiale on 2020/10/8.
//

#ifndef I3_SWITCH_NOTIFICATION_SETTINGS_H
#define I3_SWITCH_NOTIFICATION_SETTINGS_H
#include <gtk/gtk.h>
#include <map>

#include "Consts.h"
class Settings {
   private:
    Settings();
    gchar*** workspaces{};
    gsize workspacesNum{};
    gchar* defaultSymbol{};
    gchar* symbolFontFamily{};
    gint symbolFontSize{};
    gchar* nameFontFamily{};
    gint nameFontSize{};
    static Settings* settings;
    void loadSettings();
    void checkSettingFile(gchar* string);

   public:
    static Settings* getSettings();
    gchar* getWorkspaceSymbol(const char* workspaceName);
    gchar* getSymbolFontFamily();
    gint getSymbolFontSize();
    gchar* getNameFontFamily();
    gint getNameFontSize();
    const gchar* configPath = "../share/i3-switch-notification/conf/i3-switch-notification.conf";
};

#endif  //I3_SWITCH_NOTIFICATION_SETTINGS_H
