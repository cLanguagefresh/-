#ifndef HANDLEMSG_H
#define HANDLEMSG_H
#include <Arduino.h> 
#include <ArduinoJson.h>
#include "NetworkManager.h"
#include <Adafruit_NeoPixel.h> 
#define WH 32
#define PIN        23
#define NUMPIXELS  16
extern const char* ACK_TOPIC_STATE;
extern const char* ACK_TOPIC_HUMI;
extern const char* ACK_TOPIC_HUMIDIFIER;  

class DeviceManager : public NetworkManager {
public:
    DeviceManager() : NetworkManager("echoes", "88888888", "124.220.84.17") {}

    // 正确实现所有纯虚函数
    void target_humi_to_stm32(const JsonDocument& doc) override {
        if (doc.containsKey("set_humi")) { // 注意字段名统一
            int humi = doc["set_humi"];
            int msgId = doc["msgId"]; // 提取消息ID
            String cmd = "{\"cmd\":\"set_humi\",\"value\":" + String(humi) + "}\n";
           // Serial.print("[ESP32] 发送指令: ");
            Serial.print(cmd); // 打印原始指令
            Serial.println(cmd); // 通过串口发送

            // 发送确认（包含原始msgId）
        DynamicJsonDocument ackDoc(64);
        ackDoc["result"] = "success";
        ackDoc["cmd"] = "humi";
        ackDoc["msgId"] = msgId; // 关键：回传相同ID
        publishJSON(ACK_TOPIC_HUMI, ackDoc);
        }
    }
    void state_view_to_stm32(const JsonDocument& doc) override {        
        //状态  
        // 发送状态切换指令
        if (doc.containsKey("set_state")) {
            int state = doc["set_state"];
            int msgId = doc["msgId"];
            String cmd = "{\"cmd\":\"set_state\"}\n";
            Serial.println(cmd); // 通过串口发送给STM32
        

        // 提取接收到的msgId
    int receivedMsgId = doc["msgId"];
    DynamicJsonDocument ackDoc(128);
        ackDoc["result"] = "success";
        ackDoc["cmd"] = "state";
        ackDoc["msgId"] = msgId; // 关键：回传相同ID
        publishJSON(ACK_TOPIC_STATE, ackDoc);
        }

    }
    // HandleMsg.h
void hum_view(const JsonDocument& doc) override {  
    
    
    // 添加 override
    // 湿度查看逻辑
    }

    void sound_view(const JsonDocument& doc) override {
        // 噪音查看逻辑


    }

    void target_humi_view(const JsonDocument& doc) override {
        // 湿度阈值逻辑


    }

    void state_view(const JsonDocument& doc) override {  // 补全缺失方法
        // 加湿器状态处理逻辑

          
    DynamicJsonDocument ackDoc(64);
    ackDoc["result"] = "success";
    ackDoc["cmd"] = "humidifier";
    publishJSON("ack/humidifier", ackDoc);
    }
     void water_view(const JsonDocument& doc) override {        
        //水位状态  
        
                          
    }   
    
    void temp_view(const JsonDocument& doc) override {        
        //水位状态  
        
                          
    }   

    // 加湿器控制确认（在对应处理函数中添加）
void handleHumidifierControl() {
    // ...控制逻辑...
    
    DynamicJsonDocument ackDoc(64);
    ackDoc["result"] = "success";
    ackDoc["cmd"] = "humidifier";
    publishJSON("ack/humidifier", ackDoc);
}

// 示例：带错误码的确认消息
void sendErrorAck(const String& cmd, int errorCode) {
    DynamicJsonDocument ackDoc(128);
    ackDoc["result"] = "error";
    ackDoc["cmd"] = cmd;
    ackDoc["code"] = errorCode;
    ackDoc["msg"] = "Invalid humidity value";
    publishJSON("ack/" + cmd, ackDoc);
}
};
#endif
