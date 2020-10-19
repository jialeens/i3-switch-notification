//
// Created by jiale on 2020/10/8.
//

#ifndef I3_SWITCH_NOTIFICATION_SETTINGS_H
#define I3_SWITCH_NOTIFICATION_SETTINGS_H
#include <gtk/gtk.h>
#include <map>
#include "Constant.h"
using namespace std;
class Settings {
   private:
    Settings();
    const gchar* configPath = "../share/i3-switch-notification/conf/i3-switch-notification.conf";
    gint windowWidth{};
    gint windowHeight{};
    gchar*** workspaces{};
    gsize workspacesNum{};
    gchar* defaultSymbol{};
    gchar* symbolFontFamily{};
    gint symbolFontSize{};
    gchar* nameFontFamily{};
    gint nameFontSize{};
    gboolean windowTransparent{};
    gchar* nameFontColor{};
    gchar* symbolFontColor{};
    gboolean loadStyleFromCssFile{};
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
    gint getWindowWidth();
    gint getWindowHeight();
    gboolean getWindowTransparent();
    gchar* getNameFontColor();
    gchar* getSymbolFontColor();
    gboolean isLoadStyleFromCssFile();
};

#endif  //I3_SWITCH_NOTIFICATION_SETTINGS_H
