/**
 * Settings库高级使用示例
 * 演示如何在系统管理中使用Settings库
 */

#include <Arduino.h>
#include "Settings.h"
#include "Logger.h"
#include <LittleFS.h>

// 配置对象
Settings settings("/system_config.json", 2048);

// 设备配置结构
struct DeviceConfig {
  // 系统参数
  String deviceName;
  String firmwareVersion;
  bool debugMode;
  
  // 网络配置
  String wifiSSID;
  String wifiPassword;
  bool staticIP;
  String ipAddress;
  String subnet;
  String gateway;
  String dns;
  
  // MQTT配置
  bool mqttEnabled;
  String mqttServer;
  int mqttPort;
  String mqttUsername;
  String mqttPassword;
  String mqttTopic;
  int mqttQos;
  bool mqttRetain;
  
  // 传感器校准
  float calibrationFactor;
  float offset;
  int sampleRate;
  
  // 电源管理
  int sleepTime;
  bool deepSleepEnabled;
  int wakeupPin;
  
  // 加载配置
  void loadFromSettings() {
    // 系统参数
    deviceName = settings.getString("device_name", "ESP设备");
    firmwareVersion = settings.getString("firmware_version", "1.0.0");
    debugMode = settings.getBool("debug_mode", false);
    
    // 网络配置
    wifiSSID = settings.getString("wifi_ssid", "");
    wifiPassword = settings.getString("wifi_password", "");
    staticIP = settings.getBool("static_ip", false);
    ipAddress = settings.getString("ip_address", "192.168.1.100");
    subnet = settings.getString("subnet", "255.255.255.0");
    gateway = settings.getString("gateway", "192.168.1.1");
    dns = settings.getString("dns", "8.8.8.8");
    
    // MQTT配置
    mqttEnabled = settings.getBool("mqtt_enabled", false);
    mqttServer = settings.getString("mqtt_server", "");
    mqttPort = settings.getInt("mqtt_port", 1883);
    mqttUsername = settings.getString("mqtt_username", "");
    mqttPassword = settings.getString("mqtt_password", "");
    mqttTopic = settings.getString("mqtt_topic", "esp/data");
    mqttQos = settings.getInt("mqtt_qos", 0);
    mqttRetain = settings.getBool("mqtt_retain", false);
    
    // 传感器校准
    calibrationFactor = settings.getFloat("calibration_factor", 1.0);
    offset = settings.getFloat("offset", 0.0);
    sampleRate = settings.getInt("sample_rate", 10);
    
    // 电源管理
    sleepTime = settings.getInt("sleep_time", 0);
    deepSleepEnabled = settings.getBool("deep_sleep_enabled", false);
    wakeupPin = settings.getInt("wakeup_pin", 0);
  }
  
  // 保存配置
  void saveToSettings() {
    // 系统参数
    settings.setString("device_name", deviceName);
    settings.setString("firmware_version", firmwareVersion);
    settings.setBool("debug_mode", debugMode);
    
    // 网络配置
    settings.setString("wifi_ssid", wifiSSID);
    settings.setString("wifi_password", wifiPassword);
    settings.setBool("static_ip", staticIP);
    settings.setString("ip_address", ipAddress);
    settings.setString("subnet", subnet);
    settings.setString("gateway", gateway);
    settings.setString("dns", dns);
    
    // MQTT配置
    settings.setBool("mqtt_enabled", mqttEnabled);
    settings.setString("mqtt_server", mqttServer);
    settings.setInt("mqtt_port", mqttPort);
    settings.setString("mqtt_username", mqttUsername);
    settings.setString("mqtt_password", mqttPassword);
    settings.setString("mqtt_topic", mqttTopic);
    settings.setInt("mqtt_qos", mqttQos);
    settings.setBool("mqtt_retain", mqttRetain);
    
    // 传感器校准
    settings.setFloat("calibration_factor", calibrationFactor);
    settings.setFloat("offset", offset);
    settings.setInt("sample_rate", sampleRate);
    
    // 电源管理
    settings.setInt("sleep_time", sleepTime);
    settings.setBool("deep_sleep_enabled", deepSleepEnabled);
    settings.setInt("wakeup_pin", wakeupPin);
    
    // 保存设置
    settings.save();
  }
  
  // 重置为默认配置
  void resetToDefaults() {
    // 系统参数
    deviceName = "ESP设备";
    firmwareVersion = "1.0.0";
    debugMode = false;
    
    // 网络配置
    wifiSSID = "";
    wifiPassword = "";
    staticIP = false;
    ipAddress = "192.168.1.100";
    subnet = "255.255.255.0";
    gateway = "192.168.1.1";
    dns = "8.8.8.8";
    
    // MQTT配置
    mqttEnabled = false;
    mqttServer = "";
    mqttPort = 1883;
    mqttUsername = "";
    mqttPassword = "";
    mqttTopic = "esp/data";
    mqttQos = 0;
    mqttRetain = false;
    
    // 传感器校准
    calibrationFactor = 1.0;
    offset = 0.0;
    sampleRate = 10;
    
    // 电源管理
    sleepTime = 0;
    deepSleepEnabled = false;
    wakeupPin = 0;
    
    // 保存默认设置
    saveToSettings();
  }
  
  // 将配置导出为JSON
  String toJSON() {
    return settings.getSettingsJson();
  }
  
  // 从JSON导入配置
  bool fromJSON(const String& json) {
    return settings.updateFromJson(json);
  }
};

// 创建全局配置对象
DeviceConfig config;

// 打印设备配置
void printDeviceConfig() {
  Serial.println("\n========== 设备配置 ==========");
  
  Serial.println("\n-- 系统参数 --");
  Serial.printf("设备名称: %s\n", config.deviceName.c_str());
  Serial.printf("固件版本: %s\n", config.firmwareVersion.c_str());
  Serial.printf("调试模式: %s\n", config.debugMode ? "开启" : "关闭");
  
  Serial.println("\n-- 网络配置 --");
  Serial.printf("WiFi SSID: %s\n", config.wifiSSID.c_str());
  Serial.printf("WiFi 密码: %s\n", config.wifiPassword.length() > 0 ? "********" : "<未设置>");
  Serial.printf("静态IP: %s\n", config.staticIP ? "开启" : "关闭");
  if (config.staticIP) {
    Serial.printf("IP地址: %s\n", config.ipAddress.c_str());
    Serial.printf("子网掩码: %s\n", config.subnet.c_str());
    Serial.printf("网关: %s\n", config.gateway.c_str());
    Serial.printf("DNS: %s\n", config.dns.c_str());
  }
  
  Serial.println("\n-- MQTT配置 --");
  Serial.printf("MQTT: %s\n", config.mqttEnabled ? "开启" : "关闭");
  if (config.mqttEnabled) {
    Serial.printf("服务器: %s\n", config.mqttServer.c_str());
    Serial.printf("端口: %d\n", config.mqttPort);
    Serial.printf("用户名: %s\n", config.mqttUsername.c_str());
    Serial.printf("密码: %s\n", config.mqttPassword.length() > 0 ? "********" : "<未设置>");
    Serial.printf("主题: %s\n", config.mqttTopic.c_str());
    Serial.printf("QoS: %d\n", config.mqttQos);
    Serial.printf("保留消息: %s\n", config.mqttRetain ? "是" : "否");
  }
  
  Serial.println("\n-- 传感器校准 --");
  Serial.printf("校准系数: %.4f\n", config.calibrationFactor);
  Serial.printf("偏移量: %.2f\n", config.offset);
  Serial.printf("采样率: %d\n", config.sampleRate);
  
  Serial.println("\n-- 电源管理 --");
  Serial.printf("睡眠时间: %d分钟\n", config.sleepTime);
  Serial.printf("深度睡眠: %s\n", config.deepSleepEnabled ? "开启" : "关闭");
  Serial.printf("唤醒引脚: %d\n", config.wakeupPin);
  
  Serial.println("\n================================\n");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("Settings库高级使用示例 - 系统配置管理");
  
  // 初始化日志系统
  LOG->setLogLevel(LOG_DEBUG);
  
  // 初始化文件系统
  if (!LittleFS.begin()) {
    Serial.println("文件系统初始化失败!");
    return;
  }
  
  // 初始化设置
  if (!settings.begin()) {
    Serial.println("设置初始化失败!");
    return;
  }
  
  // 检查是否首次运行，如果是则设置默认配置
  if (!settings.exists("first_run")) {
    Serial.println("首次运行，设置默认配置");
    settings.setBool("first_run", true);
    config.resetToDefaults();
  } else {
    // 加载配置
    config.loadFromSettings();
  }
  
  // 打印当前配置
  printDeviceConfig();
  
  // 演示更改一些设置
  Serial.println("更改一些配置项...");
  config.deviceName = "传感器节点1";
  config.debugMode = true;
  config.wifiSSID = "HomeWiFi";
  config.mqttEnabled = true;
  config.mqttServer = "mqtt.example.com";
  config.calibrationFactor = 2.35678;
  config.sampleRate = 20;
  
  // 保存新配置
  config.saveToSettings();
  
  // 重新加载并打印
  config.loadFromSettings();
  printDeviceConfig();
  
  // 演示JSON导出导入
  Serial.println("导出配置为JSON...");
  String jsonConfig = config.toJSON();
  Serial.println(jsonConfig);
  
  Serial.println("\n修改配置后重新导入...");
  // 修改后的JSON配置
  String modifiedJson = "{\"device_name\":\"传感器节点2\",\"mqtt_port\":1884,\"calibration_factor\":3.14159}";
  Serial.println(modifiedJson);
  
  if (config.fromJSON(modifiedJson)) {
    Serial.println("JSON导入成功!");
  } else {
    Serial.println("JSON导入失败!");
  }
  
  // 重新加载并打印
  config.loadFromSettings();
  printDeviceConfig();
  
  Serial.println("设置已完成初始化!");
}

void loop() {
  // 每隔5秒更新一个随机配置值来演示
  static unsigned long lastUpdateTime = 0;
  
  if (millis() - lastUpdateTime >= 5000) {
    lastUpdateTime = millis();
    
    // 随机选一个配置项修改
    int choice = random(0, 5);
    switch (choice) {
      case 0:
        config.sampleRate = random(1, 50);
        Serial.printf("更新采样率: %d\n", config.sampleRate);
        break;
      case 1:
        config.calibrationFactor += random(-10, 10) / 100.0;
        Serial.printf("更新校准系数: %.4f\n", config.calibrationFactor);
        break;
      case 2:
        config.sleepTime = random(0, 30);
        Serial.printf("更新睡眠时间: %d分钟\n", config.sleepTime);
        break;
      case 3:
        config.debugMode = !config.debugMode;
        Serial.printf("切换调试模式: %s\n", config.debugMode ? "开启" : "关闭");
        break;
      case 4:
        config.mqttPort = random(1880, 1890);
        Serial.printf("更新MQTT端口: %d\n", config.mqttPort);
        break;
    }
    
    // 保存配置
    config.saveToSettings();
  }
  
  delay(100);
} 