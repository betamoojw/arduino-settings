#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

/**
 * @brief 通用配置管理类，用于保存和读取系统配置
 * 
 * 使用JSON格式存储配置在LittleFS文件系统中
 * 支持多种类型的配置项，如整数、布尔值、字符串等
 */
class Settings {
private:
    // 配置文件路径
    String configFile;
    
    // 配置项保存的JSON文档
    DynamicJsonDocument jsonDoc;
    
    // 是否发生过更改
    bool changed;
    
    // 保存设置到文件
    bool saveToFile();
    
    // 从文件加载设置
    bool loadFromFile();
    
public:
    /**
     * @brief 构造函数
     * @param configFilePath 配置文件路径，默认为/config.json
     * @param docSize JSON文档大小，默认为1024字节
     */
    Settings(const String& configFilePath = "/config.json", size_t docSize = 1024);
    
    /**
     * @brief 初始化设置系统
     * @return 是否成功初始化
     */
    bool begin();
    
    /**
     * @brief 保存任何待处理的更改
     * @return 是否成功保存
     */
    bool save();
    
    /**
     * @brief 重新加载设置
     * @return 是否成功重新加载
     */
    bool reload();
    
    /**
     * @brief 检查配置项是否存在
     * @param key 配置项键名
     * @return 是否存在
     */
    bool exists(const String& key) const;

    // 获取设置项 - 各种类型
    int getInt(const String& key, int defaultValue = 0) const;
    bool getBool(const String& key, bool defaultValue = false) const;
    float getFloat(const String& key, float defaultValue = 0.0) const;
    String getString(const String& key, const String& defaultValue = "") const;
    
    // 设置配置项 - 各种类型
    void setInt(const String& key, int value);
    void setBool(const String& key, bool value);
    void setFloat(const String& key, float value);
    void setString(const String& key, const String& value);
    
    /**
     * @brief 删除配置项
     * @param key 配置项键名
     */
    void remove(const String& key);
    
    /**
     * @brief 清除所有配置
     */
    void clear();
    
    /**
     * @brief 获取JSON格式的设置
     * @return JSON字符串
     */
    String getSettingsJson() const;
    
    /**
     * @brief 从JSON更新设置
     * @param json JSON字符串
     * @return 是否成功更新
     */
    bool updateFromJson(const String& json);
    
    /**
     * @brief 获取原始JSON文档的引用（高级用法）
     * @return JSON文档引用
     */
    DynamicJsonDocument& getJsonDocument();
};

#endif // SETTINGS_H 