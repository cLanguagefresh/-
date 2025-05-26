#include "HandleMsg.h"// 示例：上级目录的 include 文件夹
#include "CommunicationWithSTM32Qt.h"

extern DeviceManager Manager;  // 声明外部全局实例

void CommunicationWithSTM32Qt(void)
{

if (Serial.available() > 0) {
    // 读取串口数据
    String raw_data = Serial.readStringUntil('\n');  // 读取直到换行符
    raw_data.trim();  // 去除首尾空白

    // 解析JSON
    StaticJsonDocument<JSON_BUFFER_SIZE> doc;
    DeserializationError error = deserializeJson(doc, raw_data);

    // 检查解析错误
   //  if (error) {
   //   Serial.print("JSON解析失败: ");
   //   Serial.println(error.c_str());
   //   return;
  //  }

     // 检查必需字段
     if (!doc.containsKey("T") || !doc.containsKey("H") || 
     !doc.containsKey("N") || !doc.containsKey("W") ||
     !doc.containsKey("State") || !doc.containsKey("TargetH")) {
    // Serial.println("错误: 数据字段缺失");
     return;
 }

    // 提取数据
    float temperature = doc["T"];  // 温度
    float humidity = doc["H"];     // 湿度
    float noise = doc["N"];        // 噪音
    float water = doc["W"];        // 水位
    int state = doc["State"];           // 加湿器状态（0:手动, 1:睡眠, 2:关机）
    int target_humi = doc["TargetH"];   // 目标湿度

    // 打印调试信息
   // Serial.println("------ 传感器数据 ------");
   // Serial.print("温度: "); Serial.println(temperature);
   // Serial.print("湿度: "); Serial.println(humidity);
   // Serial.print("噪音: "); Serial.println(noise);
   // Serial.print("水位: "); Serial.println(water);
   // Serial.print("状态: "); Serial.println(state);
   // Serial.print("目标湿度: "); Serial.println(target_humi);
   // Serial.println("------------------------");

    // 发布到MQTT主题
        DynamicJsonDocument tempDoc(64);
        tempDoc["value"] = temperature;
        Manager.publishJSON("temp_view", tempDoc);  // 温度主题

        DynamicJsonDocument noiseDoc(64);
        noiseDoc["value"] = noise;
        Manager.publishJSON("sound_view", noiseDoc); // 噪音主题

        // 水位数据发布到"water_view"主题
        DynamicJsonDocument waterDoc(64);
         waterDoc["value"] = water;
        Manager.publishJSON("water_view", waterDoc);  // 水位主题
        
        DynamicJsonDocument humDoc(64);
        humDoc["value"] = humidity;
        Manager.publishJSON("hum_view", humDoc);   //湿度查看

        DynamicJsonDocument stateDoc(64);
        stateDoc["value"] = state;
        Manager.publishJSON("state_view", stateDoc);  // 发布状态

        DynamicJsonDocument targetDoc(64);
        targetDoc["value"] = target_humi;
        Manager.publishJSON("target_humi_view", targetDoc);  // 发布目标湿度

        

  }
// else Serial.println("------ ESP32未连接传感器 ------");

}


  


