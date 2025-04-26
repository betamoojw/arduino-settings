/**
 * Settings库基本使用示例
 * 演示如何使用Settings库进行配置管理
 */

#include <Arduino.h>
#include "Settings.h"
#include "Logger.h"

// 创建设置对象
Settings settings;

// 定义默认值
const int DEFAULT_AUTO_POWEROFF = 5;     // 默认自动关机时间（分钟）
const bool DEFAULT_BUZZER_ENABLED = true; // 默认蜂鸣器开启
const String DEFAULT_WIFI_SSID = "MyESP"; // 默认WiFi SSID

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("Settings库使用示例");
  
  // 初始化日志系统
  LOG->setLogLevel(LOG_DEBUG);
  
  // 初始化设置
  if (!settings.begin()) {
    Serial.println("设置初始化失败!");
    return;
  }
  
  // 首次运行时设置一些默认值
  if (!settings.exists("first_run")) {
    Serial.println("首次运行，设置默认配置");
    settings.setBool("first_run", true);
    settings.setInt("auto_poweroff", DEFAULT_AUTO_POWEROFF);
    settings.setBool("buzzer_enabled", DEFAULT_BUZZER_ENABLED);
    settings.setString("wifi_ssid", DEFAULT_WIFI_SSID);
    settings.setString("wifi_password", "");
    settings.setFloat("cal_factor", 2.54);
    
    // 保存默认设置
    settings.save();
  }
  
  // 读取设置并显示
  Serial.println("\n当前设置:");
  Serial.print("自动关机: ");
  Serial.print(settings.getInt("auto_poweroff"));
  Serial.println("分钟");
  
  Serial.print("蜂鸣器: ");
  Serial.println(settings.getBool("buzzer_enabled") ? "开启" : "关闭");
  
  Serial.print("WiFi SSID: ");
  Serial.println(settings.getString("wifi_ssid"));
  
  Serial.print("校准因子: ");
  Serial.println(settings.getFloat("cal_factor"), 2);
  
  // 修改一些设置
  Serial.println("\n修改设置:");
  settings.setInt("auto_poweroff", 10);
  settings.setBool("buzzer_enabled", !settings.getBool("buzzer_enabled"));
  settings.setFloat("cal_factor", 3.14159);
  settings.save();
  
  Serial.println("设置已更新!");
  
  // 重新显示更新后的设置
  Serial.println("\n更新后的设置:");
  Serial.print("自动关机: ");
  Serial.print(settings.getInt("auto_poweroff"));
  Serial.println("分钟");
  
  Serial.print("蜂鸣器: ");
  Serial.println(settings.getBool("buzzer_enabled") ? "开启" : "关闭");
  
  Serial.print("校准因子: ");
  Serial.println(settings.getFloat("cal_factor"), 5);
  
  // 使用JSON更新多个设置
  String jsonConfig = "{\"auto_poweroff\":15,\"wifi_ssid\":\"NewESP\",\"new_setting\":\"测试\"}";
  Serial.println("\n使用JSON更新多个设置:");
  Serial.println(jsonConfig);
  
  if (settings.updateFromJson(jsonConfig)) {
    Serial.println("JSON更新成功!");
  } else {
    Serial.println("JSON更新失败!");
  }
  
  // 获取所有设置为JSON
  String allSettings = settings.getSettingsJson();
  Serial.println("\n所有设置(JSON格式):");
  Serial.println(allSettings);
}

void loop() {
  // 在loop中演示如何更改设置
  static unsigned long lastUpdateTime = 0;
  static int counter = 0;
  
  if (millis() - lastUpdateTime >= 5000) {
    lastUpdateTime = millis();
    counter++;
    
    // 每5秒更新一次计数器设置
    settings.setInt("counter", counter);
    
    // 每10秒保存一次，演示延迟保存
    if (counter % 2 == 0) {
      Serial.print("保存计数器: ");
      Serial.println(counter);
      settings.save();
    } else {
      Serial.print("更新计数器但不保存: ");
      Serial.println(counter);
    }
  }
  
  delay(100);
} 