#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtMqtt/qmqttclient.h>
#include <QTimer>
#include <QTime>

extern const QString ACK_TOPIC_STATE;
extern const QString ACK_TOPIC_HUMI;
extern const QString ACK_TOPIC_HUMIDIFIER;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    QDateTime Data;
    QTimer qtimer;
    QTime  qtime;
private slots:
    void on_connectButton_clicked();
    void connectSuccess();//成功连接槽函数
    void Datatimeout_slot();//显示时间
    void receiveMessage(const QByteArray &buf,const QMqttTopicName &topic);//接收消息槽函数

    void on_ledButton_clicked();


    void on_humiChangeButton_clicked();

    void on_clButton_clicked();

    void on_doorButton_clicked();

    void on_fanButton_clicked();

    void on_jsqButton_clicked();

    void on_modePushButton_clicked();

    void refreshUI();



    void sendCommand();
    void updateModeLabel(const QString &text); // 新增槽函数
    void updateHumiEdit(float value);

private:
    Ui::Widget *ui;
    QMqttClient *client;
     QTimer *m_ackTimer; // 添加此行

    // 数据缓存
       QString m_lastTemp = "N/A";     // 温度
       QString m_lastHumi = "N/A";     // 湿度
       QString m_lastSound = "N/A";    // 噪音
        QString m_lastWater= "N/A";    // 水位
       QDateTime m_lastUpdateTime;     // 最后更新时间戳

         int m_currentMsgId = 0;
         const int MAX_MSG_ID = 65535; // 16位无符号数最大值

          QString getExpectedAckTopic() const;
           QString m_expectedAckType; // 确保已定义

           // 新增成员：待确认消息队列
               struct PendingMessage {
                   QString topic;
                   QByteArray payload;
                   int msgId;
                   int retries = 0;
                   QDateTime lastSentTime;
               };
               QList<PendingMessage> m_pendingMessages; // 待确认消息队列
               const int MAX_RETRIES = 3;               // 最大重试次数



};

#endif // WIDGET_H
