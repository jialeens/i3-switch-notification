//
// Created by jiale on 2020/10/8.
//

#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include "Settings.h"
using namespace std;
Settings* Settings::settings = nullptr;
void Settings::loadSettings() {
    gchar* rc_file = g_build_filename(g_get_user_config_dir(), PREFERENCES_FILE, NULL);
    checkSettingFile(rc_file);
    GKeyFile* rc_key = g_key_file_new();
    GError* error = nullptr;
    if (g_key_file_load_from_file(rc_key, rc_file, G_KEY_FILE_NONE, &error)) {
        gchar** keys = g_key_file_get_keys(rc_key, "workspaces", &workspacesNum, nullptr);
        workspaces = new gchar**[workspacesNum];
        gchar* value;
        for (int i = 0; i < workspacesNum; i++) {
            value = g_key_file_get_value(rc_key,
                                         "workspaces",
                                         keys[i],
                                         &error);
            workspaces[i] = new gchar* [2] { keys[i], value };
        }
        defaultSymbol = g_key_file_get_string(rc_key, "workspaces", "default", &error);
        loadStyleFromCssFile = g_key_file_get_boolean(rc_key, "style", "loadStyleFromCssFile", &error);
        if (!isLoadStyleFromCssFile()) {
            windowWidth = g_key_file_get_integer(rc_key, "style", "window.width", &error);
            if (windowWidth == 0) {
                windowWidth = 50;
            }
            windowHeight = g_key_file_get_integer(rc_key, "style", "window.height", &error);
            if (windowHeight == 0) {
                windowHeight = 50;
            }
            windowTransparent = g_key_file_get_boolean(rc_key, "style", "window.transparent", &error);

            symbolFontColor = g_key_file_get_string(rc_key, "style", "symbol.font.color", &error);
            symbolFontFamily = g_key_file_get_string(rc_key, "style", "symbol.font.family", &error);
            if (symbolFontFamily == nullptr) {
                symbolFontFamily = nullptr;
            }
            symbolFontSize = g_key_file_get_integer(rc_key, "style", "symbol.font.size", &error);
            if (symbolFontSize == 0) {
                symbolFontSize = 48;
            }
            nameFontColor = g_key_file_get_string(rc_key, "style", "name.font.color", &error);
            nameFontFamily = g_key_file_get_string(rc_key, "style", "name.font.family", &error);
            if (nameFontFamily == nullptr) {
                nameFontFamily = nullptr;
            }
            nameFontSize = g_key_file_get_integer(rc_key, "style", "name.font.size", &error);
            if (nameFontSize == 0) {
                nameFontSize = 16;
            }
        }
        g_free(keys);
        g_key_file_free(rc_key);
        g_free(rc_file);
    }
}
gchar* Settings::getWorkspaceSymbol(const gchar* workspaceName) {
    for (int i = 0; i < workspacesNum; ++i) {
        if (g_strcmp0(workspaces[i][0], workspaceName) == 0) {
            return workspaces[i][1];
        }
    }
    return defaultSymbol;
}
gint Settings::getSymbolFontSize() {
    return symbolFontSize;
}
gchar* Settings::getSymbolFontFamily() {
    return symbolFontFamily;
}
gint Settings::getNameFontSize() {
    return nameFontSize;
}
gchar* Settings::getNameFontFamily() {
    return nameFontFamily;
}
gint Settings::getWindowWidth() {
    return windowWidth;
}
gint Settings::getWindowHeight() {
    return windowHeight;
}
Settings* Settings::getSettings() {
    if (nullptr == settings) {
        settings = new Settings();
        settings->loadSettings();
    }
    return settings;
}
void Settings::checkSettingFile(gchar* settingFilePath) {
    if (access(settingFilePath, F_OK) != 0) {
        gchar* configDir = g_build_filename(g_get_user_config_dir(), PROJECT_NAME, NULL);
        if (access(configDir, F_OK) != 0) {
            mkdir(configDir, 0755);
        }
        std::ifstream src(configPath, std::ios::binary);
        std::ofstream dst(settingFilePath, std::ios::binary);
        dst << src.rdbuf();
        src.close();
        dst.close();
        g_free(configDir);
    }
}
gboolean Settings::getWindowTransparent() {
    return windowTransparent;
}
gchar* Settings::getNameFontColor() {
    return nameFontColor;
}
gchar* Settings::getSymbolFontColor() {
    return symbolFontColor;
}
gboolean Settings::isLoadStyleFromCssFile() {
    return loadStyleFromCssFile;
}
Settings::Settings() = default;
