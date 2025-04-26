#include "Settings.h"
#include "Logger.h"

// 构造函数
Settings::Settings(const String& configFilePath, size_t docSize) 
    : configFile(configFilePath), jsonDoc(docSize), changed(false) {
}

// 初始化设置
bool Settings::begin() {
    // 如果文件系统未初始化，立即初始化
    if (!LittleFS.begin()) {
        LOG_E("SETTINGS", "文件系统初始化失败");
        return false;
    }
    
    // 尝试从文件加载设置
    if (!loadFromFile()) {
        LOG_W("SETTINGS", "无法加载设置，使用空配置");
        // 初始化为空配置
        jsonDoc.clear();
        changed = true;
    }
    
    return true;
}

// 保存设置到文件
bool Settings::saveToFile() {
    File configFileHandle = LittleFS.open(configFile, "w");
    if (!configFileHandle) {
        LOG_E("SETTINGS", "无法打开配置文件进行写入: %s", configFile.c_str());
        return false;
    }
    
    serializeJson(jsonDoc, configFileHandle);
    configFileHandle.close();
    
    LOG_I("SETTINGS", "设置已保存到文件: %s", configFile.c_str());
    changed = false;
    return true;
}

// 从文件加载设置
bool Settings::loadFromFile() {
    if (!LittleFS.exists(configFile)) {
        LOG_W("SETTINGS", "配置文件不存在: %s", configFile.c_str());
        return false;
    }
    
    File configFileHandle = LittleFS.open(configFile, "r");
    if (!configFileHandle) {
        LOG_E("SETTINGS", "无法打开配置文件进行读取: %s", configFile.c_str());
        return false;
    }
    
    DeserializationError error = deserializeJson(jsonDoc, configFileHandle);
    configFileHandle.close();
    
    if (error) {
        LOG_E("SETTINGS", "无法解析JSON: %s", error.c_str());
        return false;
    }
    
    LOG_I("SETTINGS", "已从文件加载设置: %s", configFile.c_str());
    changed = false;
    return true;
}

// 保存更改
bool Settings::save() {
    if (!changed) {
        LOG_D("SETTINGS", "无需保存，设置未更改");
        return true;
    }
    
    return saveToFile();
}

// 重新加载设置
bool Settings::reload() {
    if (changed) {
        LOG_W("SETTINGS", "有未保存的更改，重新加载将丢失这些更改");
    }
    
    return loadFromFile();
}

// 检查配置项是否存在
bool Settings::exists(const String& key) const {
    return jsonDoc.containsKey(key);
}

// 获取整数设置
int Settings::getInt(const String& key, int defaultValue) const {
    if (!jsonDoc.containsKey(key)) {
        return defaultValue;
    }
    
    return jsonDoc[key];
}

// 获取布尔设置
bool Settings::getBool(const String& key, bool defaultValue) const {
    if (!jsonDoc.containsKey(key)) {
        return defaultValue;
    }
    
    return jsonDoc[key];
}

// 获取浮点数设置
float Settings::getFloat(const String& key, float defaultValue) const {
    if (!jsonDoc.containsKey(key)) {
        return defaultValue;
    }
    
    return jsonDoc[key];
}

// 获取字符串设置
String Settings::getString(const String& key, const String& defaultValue) const {
    if (!jsonDoc.containsKey(key)) {
        return defaultValue;
    }
    
    return jsonDoc[key].as<String>();
}

// 设置整数值
void Settings::setInt(const String& key, int value) {
    if (!jsonDoc.containsKey(key) || jsonDoc[key] != value) {
        jsonDoc[key] = value;
        changed = true;
    }
}

// 设置布尔值
void Settings::setBool(const String& key, bool value) {
    if (!jsonDoc.containsKey(key) || jsonDoc[key] != value) {
        jsonDoc[key] = value;
        changed = true;
    }
}

// 设置浮点数值
void Settings::setFloat(const String& key, float value) {
    if (!jsonDoc.containsKey(key) || fabs(jsonDoc[key].as<float>() - value) > 0.000001) {
        jsonDoc[key] = value;
        changed = true;
    }
}

// 设置字符串值
void Settings::setString(const String& key, const String& value) {
    if (!jsonDoc.containsKey(key) || strcmp(jsonDoc[key].as<const char*>(), value.c_str()) != 0) {
        jsonDoc[key] = value;
        changed = true;
    }
}

// 删除配置项
void Settings::remove(const String& key) {
    if (jsonDoc.containsKey(key)) {
        jsonDoc.remove(key);
        changed = true;
    }
}

// 清除所有配置
void Settings::clear() {
    if (!jsonDoc.isNull()) {
        jsonDoc.clear();
        changed = true;
    }
}

// 获取JSON格式的设置
String Settings::getSettingsJson() const {
    String jsonString;
    serializeJson(jsonDoc, jsonString);
    return jsonString;
}

// 从JSON更新设置
bool Settings::updateFromJson(const String& json) {
    DynamicJsonDocument tempDoc(jsonDoc.capacity());
    DeserializationError error = deserializeJson(tempDoc, json);
    
    if (error) {
        LOG_E("SETTINGS", "更新设置失败：无法解析JSON: %s", error.c_str());
        return false;
    }
    
    // 遍历tempDoc中的所有键并更新jsonDoc
    JsonObject obj = tempDoc.as<JsonObject>();
    for (JsonPair pair : obj) {
        jsonDoc[pair.key()] = pair.value();
    }
    
    changed = true;
    
    // 保存更新后的设置到文件
    if (saveToFile()) {
        LOG_I("SETTINGS", "设置已从JSON更新并保存");
        return true;
    } else {
        LOG_E("SETTINGS", "无法保存更新后的设置");
        return false;
    }
}

// 获取原始JSON文档的引用
DynamicJsonDocument& Settings::getJsonDocument() {
    return jsonDoc;
} 