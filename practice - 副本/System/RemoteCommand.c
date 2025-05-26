// RemoteCommand.c
#include "RemoteCommand.h"
#include "hum_state.h"
#include "Serial.h"
#include <string.h>

#define CMD_BUFFER_SIZE 256  // 增大缓冲区大小
static char cmd_buffer[CMD_BUFFER_SIZE];
static uint8_t cmd_index = 0;

// 在 RemoteCommand.c 中定义标志位
volatile uint8_t cmd_received_flag = 0;


// 提取JSON字符串中指定键的值
static int extract_value(const char* buffer, const char* key) {
    char search_str[32];
    if (strlen(key) > 20) return 0; // 防止键名过长
    snprintf(search_str, sizeof(search_str), "\"%s\":", key);
    char* value_start = strstr(buffer, search_str);
    if (!value_start) return 0;
    
    value_start += strlen(search_str);
    // 跳过可能的空格或冒号后的无效字符
    while (*value_start && (*value_start == ' ' || *value_start == ':' || *value_start == ',')) value_start++;
    
    // 检查是否为数字
    if (*value_start < '0' || *value_start > '9') return 0;
    return (int)strtol(value_start, NULL, 10);
}

// 解析接收到的命令
static void parse_command(const char* buffer) {
    // 命令格式示例: {"cmd":"set_humi","value":60}
    if (strstr(buffer, "set_humi")) {
        int value = extract_value(buffer, "value");
        HumState_SetTargetHumi((uint8_t)value); // 设置目标湿度
    } else if (strstr(buffer, "set_state")) {
        HumState_HandleKeyEvent(KEY_MODE_PRESSED); // 切换状态
    }
}

void RemoteCommand_Process(void) {
    while (Serial_RxAvailable()) { // 使用while读取所有可用数据
        char c = Serial_ReadByte();
        if (c == '\n' || cmd_index >= CMD_BUFFER_SIZE - 1) {
            cmd_buffer[cmd_index] = '\0'; // 确保终止符
            if (cmd_index > 0) {
                Serial_Printf("[DEBUG] Received: %s\r\n", cmd_buffer);
                parse_command(cmd_buffer);
            }
            cmd_index = 0;
        } else {
            if (c != '\r') { // 忽略回车符
                cmd_buffer[cmd_index++] = c;
            }
        }
    }
}

