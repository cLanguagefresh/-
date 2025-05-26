// RemoteCommand.c
#include "RemoteCommand.h"
#include "hum_state.h"
#include "Serial.h"
#include <string.h>

#define CMD_BUFFER_SIZE 256  // ���󻺳�����С
static char cmd_buffer[CMD_BUFFER_SIZE];
static uint8_t cmd_index = 0;

// �� RemoteCommand.c �ж����־λ
volatile uint8_t cmd_received_flag = 0;


// ��ȡJSON�ַ�����ָ������ֵ
static int extract_value(const char* buffer, const char* key) {
    char search_str[32];
    if (strlen(key) > 20) return 0; // ��ֹ��������
    snprintf(search_str, sizeof(search_str), "\"%s\":", key);
    char* value_start = strstr(buffer, search_str);
    if (!value_start) return 0;
    
    value_start += strlen(search_str);
    // �������ܵĿո��ð�ź����Ч�ַ�
    while (*value_start && (*value_start == ' ' || *value_start == ':' || *value_start == ',')) value_start++;
    
    // ����Ƿ�Ϊ����
    if (*value_start < '0' || *value_start > '9') return 0;
    return (int)strtol(value_start, NULL, 10);
}

// �������յ�������
static void parse_command(const char* buffer) {
    // �����ʽʾ��: {"cmd":"set_humi","value":60}
    if (strstr(buffer, "set_humi")) {
        int value = extract_value(buffer, "value");
        HumState_SetTargetHumi((uint8_t)value); // ����Ŀ��ʪ��
    } else if (strstr(buffer, "set_state")) {
        HumState_HandleKeyEvent(KEY_MODE_PRESSED); // �л�״̬
    }
}

void RemoteCommand_Process(void) {
    while (Serial_RxAvailable()) { // ʹ��while��ȡ���п�������
        char c = Serial_ReadByte();
        if (c == '\n' || cmd_index >= CMD_BUFFER_SIZE - 1) {
            cmd_buffer[cmd_index] = '\0'; // ȷ����ֹ��
            if (cmd_index > 0) {
                Serial_Printf("[DEBUG] Received: %s\r\n", cmd_buffer);
                parse_command(cmd_buffer);
            }
            cmd_index = 0;
        } else {
            if (c != '\r') { // ���Իس���
                cmd_buffer[cmd_index++] = c;
            }
        }
    }
}

