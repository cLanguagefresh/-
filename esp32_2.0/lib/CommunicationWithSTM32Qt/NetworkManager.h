#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

// 包含必要的库文件
#include <WiFi.h>           // WiFi功能
#include <PubSubClient.h>   // MQTT客户端库
#include <ArduinoJson.h>    // JSON处理库
#include <vector>
// 网络管理类声明
class NetworkManager {
protected:
    // 网络配置参数
    const char* _ssid;        // WiFi名称
    const char* _password;    // WiFi密码
    const char* _mqttServer;  // MQTT服务器地址
    
    // 网络客户端实例
    WiFiClient _wifiClient;    // TCP客户端
    PubSubClient _mqttClient; // MQTT客户端
    
    // 订阅主题列表
   const char* _topics[8] = {  // 修改数组大小为8
    "state_view_to_stm32",              // 0: 状态控制
    "target_humi_to_stm32",         //湿度阈值控制
    "temp_view",                //1: 温度查看
    "sound_view",               //2: 噪音查看
    "target_humi_view", //3: 湿度阈值控制
    "state_view",               //4.加湿器状态
    "water_view",                //5: 水位查看（新增）
    "hum_view"                  //湿度查看
};

public:
    // 构造函数：初始化网络参数
    NetworkManager(const char* ssid, const char* pass, const char* mqtt)
        : _ssid(ssid), _password(pass), _mqttServer(mqtt),
          _mqttClient(_wifiClient) {} // 初始化MQTT客户端

    // 初始化方法：启动网络连接
    void initialize(Stream& serial) {
        serial.println("\n[系统] 初始化中...");
        
        // 连接WiFi
        connectToWiFi(serial);
        
        // 配置MQTT服务器和回调
        _mqttClient.setServer(_mqttServer, 1883);
        _mqttClient.setCallback([this](char* t, byte* p, unsigned int l) {
            this->handleMQTTMessage(t, p, l); // Lambda捕获this指针
        });
        
        serial.println("[系统] 初始化完成");
    }

    // 基础发布方法（默认QoS0）
    bool publish(const String& topic, const String& msg, 
                bool retain = false) {  // 移除qos参数
        // 检查连接状态
        if (!_mqttClient.connected()) reconnectMQTT(Serial);
        
        // 调用MQTT发布（使用二进制payload）
        return _mqttClient.publish(
            topic.c_str(),          // 主题
            (const uint8_t*)msg.c_str(), // 消息内容
            msg.length(),          // 消息长度
            retain                 // 保留标志
        );
    }

    // 模板方法：发布JSON数据
    template<typename T>
    bool publishJSON(const String& topic, T& doc, bool retain = false) {  // 移除qos参数
        String json;  // 序列化缓冲区
        serializeJson(doc, json); // 序列化JSON文档
        return publish(topic, json, retain);
    }

    // 主循环更新方法（必须定期调用）
    void update(Stream& serial) {
        checkConnection(serial);   // 检查网络连接
        _mqttClient.loop();         // MQTT心跳处理
        
        // 每5秒发布系统状态
        static unsigned long lastReport = 0;
        if(millis() - lastReport > 5000) {
            lastReport = millis();
            publishSystemStatus();  // 状态报告
        }
    }

    // 以下为需要子类实现的纯虚函数（业务逻辑接口）
    virtual void target_humi_to_stm32(const JsonDocument& doc) = 0;                       // 湿度控制
    virtual void temp_view(const JsonDocument& doc) = 0;                          // 温度查看
    virtual void sound_view(const JsonDocument& doc) = 0;                         // 噪音查看
    virtual void target_humi_view(const JsonDocument& doc) = 0;        //灭菌灯控制
    virtual void state_view(const JsonDocument& doc) = 0;                      //加湿器状态    
    virtual void water_view(const JsonDocument& doc) = 0;                      //水位状态   
    virtual void hum_view(const JsonDocument& doc) = 0;                      //湿度状态   
    virtual void state_view_to_stm32(const JsonDocument& doc) = 0;                      //湿度状态   

private:
    // 连接WiFi（带超时检测）
    void connectToWiFi(Stream& serial) {
        serial.println("[WiFi] 连接中...");
        WiFi.mode(WIFI_STA);  // 设置STA模式
        WiFi.begin(_ssid, _password);
        
        // 10秒连接超时检测
        unsigned long start = millis();
        while(WiFi.status() != WL_CONNECTED && millis()-start < 10000) {
            delay(500);
            serial.print(".");
        }
        
        // 连接失败处理
        if(!WiFi.isConnected()) {
            serial.println("\n[WiFi] 连接失败!");
            releaseResources(); // 释放资源
            ESP.restart(); // 重启设备
        }
        serial.printf("\n[WiFi] 已连接 IP: %s\n", WiFi.localIP().toString().c_str());
    }

    // MQTT重连机制
    void reconnectMQTT(Stream& serial) {
        serial.println("[MQTT] 重新连接...");
        while(!_mqttClient.connect("ESP32Client")) {  // 客户端ID固定
            serial.printf("失败! 错误码: %d\n", _mqttClient.state());
            
            if(_mqttClient.state() == -2){
              serial.println("[WIFI] 连接断开");
              checkConnection(serial);  // 正确调用方式
            }
            delay(5000);  // 5秒重试间隔
        }
        serial.println("[MQTT] 连接成功");
        // 重新订阅所有主题
        for(int i=0; i<8; ++i) {
            _mqttClient.subscribe(_topics[i]);
           Serial.printf("[MQTT] 成功订阅主题: %s\n", _topics[i]);  // 原为%S
        }
    }

    // 发布系统状态信息
    void publishSystemStatus() {
        DynamicJsonDocument doc(128);  // 预分配内存
        doc["heap"] = ESP.getFreeHeap(); // 剩余内存
        doc["rssi"] = WiFi.RSSI();     // 信号强度
        publishJSON("system/status", doc); // 使用默认QoS0
    }

    // MQTT消息回调处理
    void handleMQTTMessage(char* topic, byte* payload, unsigned int length) {
        // 创建临时字符缓冲区
        char message[length + 1]; // +1 用于字符串终止符
        memcpy(message, payload, length);
        message[length] = '\0'; // 添加终止符
        
        String msg = String(message); // 正确构造字符串
        DynamicJsonDocument doc(256); // 创建JSON文档
        deserializeJson(doc, msg);    // 解析JSON
        
        // 遍历主题列表查找匹配项
        for(int i=0; i<8; i++) {
            if(strcmp(topic, _topics[i]) == 0) {
                processCommand(i, doc);  // 处理对应命令
                break;
            }
        }
    }

    


    // 命令分发处理
   
void processCommand(int index, DynamicJsonDocument& doc) {
    switch(index) {
        case 0: state_view_to_stm32(doc); break;           // 状态控制
        case 1: target_humi_to_stm32(doc); break;         // 湿度阈值控制
        case 2: temp_view(doc); break;    // 温度查看
        case 3: sound_view(doc); break;       // 噪音查看
        case 4: target_humi_view(doc); break;   // 湿度阈值控制
        case 5: state_view(doc); break;                 // 加湿器状态
        case 6: water_view(doc); break;  // 水位查看
        case 7: hum_view(doc); break;  // 湿度查看
    }
}

    // 网络连接状态检查
    void checkConnection(Stream& serial) {
        static unsigned long lastCheck = 0;
        if(millis() - lastCheck > 2000) {  // 每2秒检查
            lastCheck = millis();
            if(!WiFi.isConnected()) {
                serial.println("[网络] 连接丢失!");
                releaseResources(); // 释放资源
                ESP.restart();  // 网络异常重启
            }
        }
    }

    // 释放资源
    void releaseResources() {
        // 释放WiFi客户端
        _wifiClient.stop();
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);

        // 释放其他资源
        // 如果有其他动态分配的内存或资源，可以在这里释放
    }

   
};



#endif // NETWORKMANAGER_H
