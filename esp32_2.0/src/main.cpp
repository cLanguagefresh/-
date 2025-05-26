#include "HandleMsg.h"
#include "CommunicationWithSTM32Qt.h"

DeviceManager Manager;  // 声明外部全局实例


void setup() {

Serial.begin(9600);
Manager.initialize(Serial); // 网络初始化


}
void loop() {
  Manager.update(Serial);
  CommunicationWithSTM32Qt();
 // delay(500);

   

}