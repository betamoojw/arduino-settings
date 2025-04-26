# Settings库

这是一个用于ESP8266/ESP32项目的通用设置管理库，提供了简单的配置存取接口。

## 特性

- 使用JSON格式存储配置在LittleFS文件系统中
- 支持多种数据类型（整数、浮点数、布尔值、字符串）
- 自动保存和加载设置
- 提供简单的键值对存取接口
- 支持从JSON字符串批量更新设置
- 内存高效，只在必要时写入文件系统

## 安装

1. 将此库复制到您的PlatformIO项目的`lib/Settings`目录下
2. 在代码中包含头文件：`#include "Settings.h"`

## 使用方法

### 基本使用

```cpp
#include "Settings.h"

// 创建设置对象
Settings settings;

void setup() {
  // 初始化设置
  settings.begin();
  
  // 读取设置，如果不存在则使用默认值
  int timeout = settings.getInt("timeout", 30);
  bool soundEnabled = settings.getBool("sound", true);
  String serverAddress = settings.getString("server", "192.168.1.1");
  
  // 修改设置
  settings.setInt("timeout", 60);
  settings.setBool("sound", false);
  settings.setString("server", "192.168.1.100");
  
  // 保存设置（如果有更改）
  settings.save();
}
```

### 高级使用

```cpp
// 使用不同的配置文件路径和更大的JSON文档大小
Settings settings("/custom_config.json", 2048);

// 检查设置是否存在
if (settings.exists("wifi_ssid")) {
  // ...
}

// 批量更新设置
String jsonConfig = "{\"timeout\":120,\"sound\":true,\"server\":\"10.0.0.1\"}";
settings.updateFromJson(jsonConfig);

// 获取所有设置为JSON字符串
String allSettings = settings.getSettingsJson();

// 删除设置项
settings.remove("old_setting");

// 清除所有设置
settings.clear();
```

## API参考

### 构造函数

- `Settings(const String& configFilePath = "/config.json", size_t docSize = 1024)`
  - 创建设置对象，可选择指定配置文件路径和JSON文档大小

### 核心方法

- `bool begin()` - 初始化设置系统并加载配置
- `bool save()` - 保存所有未保存的更改
- `bool reload()` - 重新从文件加载设置
- `bool exists(const String& key)` - 检查设置项是否存在

### 获取设置

- `int getInt(const String& key, int defaultValue = 0)`
- `bool getBool(const String& key, bool defaultValue = false)`
- `float getFloat(const String& key, float defaultValue = 0.0)`
- `String getString(const String& key, const String& defaultValue = "")`

### 设置值

- `void setInt(const String& key, int value)`
- `void setBool(const String& key, bool value)`
- `void setFloat(const String& key, float value)`
- `void setString(const String& key, const String& value)`

### 其他方法

- `void remove(const String& key)` - 删除设置项
- `void clear()` - 清除所有设置
- `String getSettingsJson()` - 获取JSON格式的所有设置
- `bool updateFromJson(const String& json)` - 从JSON更新设置
- `DynamicJsonDocument& getJsonDocument()` - 获取原始JSON文档引用

## 示例

参见`examples`目录中的示例代码。

## 许可证

MIT 