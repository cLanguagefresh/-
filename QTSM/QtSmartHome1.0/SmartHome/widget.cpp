#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>

 static int currentState = 0; // 0:手动, 1:睡眠, 2:关机

// 定义不同操作对应的主题
const QString STATE_TOPIC_TO_STM32 = "state_view_to_stm32";
const QString TARGET_HUMI_TO_STM32 = "target_humi_to_stm32";
    // 新增确认主题
    const QString ACK_TOPIC_STATE = "ack/state";
    const QString ACK_TOPIC_HUMI = "ack/humi";
    const QString ACK_TOPIC_HUMIDIFIER = "ack/humidifier";



    QTimer *m_ackTimer; // 确认超时定时器
    bool m_waitingForAck = false; // 是否在等待确认
    QString m_lastCommand; // 最后发送的命令类型

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    client = new QMqttClient;
    client->setHostname("124.220.84.17");        //地址
    client->setPort(1883);                      //端口
    client->setUsername("test");                //用户名
    client->setPassword("123456");              //密码
    client->setKeepAlive(60); // 设置心跳间隔为60秒

    connect(client,&QMqttClient::connected,this,&Widget::connectSuccess);
    connect(client, &QMqttClient::errorChanged, this, [this](QMqttClient::ClientError error) {
        if (error != QMqttClient::NoError) {
            QMessageBox::warning(this, "连接错误", "MQTT连接发生异常，正在重连...");
            client->connectToHost(); // 立即尝试重连
        }
    });
    connect(client, &QMqttClient::disconnected, [this]() {
        QMessageBox::warning(this, "连接提示", "服务器断开，正在尝试重连...");
        QTimer::singleShot(5000, this, [this]() {
            client->connectToHost(); // 5秒后重连
        });
    });

    // 初始化定时器（2秒触发一次）
       QTimer *refreshTimer = new QTimer(this);
       connect(refreshTimer, &QTimer::timeout, this, &Widget::refreshUI);
       refreshTimer->start(2000); // 2000ms = 2秒
        m_lastUpdateTime = QDateTime::currentDateTime();

       // 添加以下初始化代码
           ui->humviewEdit->setText("60");    // 显示默认阈值
           ui->humiEdit->clear();             // 清空输入框

           // 初始化确认定时器
               m_ackTimer = new QTimer(this);
               m_ackTimer->setInterval(3000); // 3秒超时
               connect(m_ackTimer, &QTimer::timeout, this, [this](){
                   QDateTime now = QDateTime::currentDateTime();

                   // 遍历待确认队列
                   for (int i = m_pendingMessages.size() - 1; i >= 0; --i) {
                       PendingMessage &msg = m_pendingMessages[i];

                       // 检查是否超时（超过3秒未确认）
                       if (msg.lastSentTime.secsTo(now) >= 3) {
                           if (msg.retries < MAX_RETRIES) {
                               // 重发消息
                               client->publish(msg.topic, msg.payload, 1);
                               msg.retries++;
                               msg.lastSentTime = now;
                               qDebug() << "重发消息, MsgID:" << msg.msgId << "重试次数:" << msg.retries;
                           } else {
                               // 超过最大重试次数，移除并报错
                               m_pendingMessages.removeAt(i);
                               ui->StatusLabel->setText("操作失败! 设备无响应");
                               QTimer::singleShot(3000, [this]() { ui->StatusLabel->clear(); });
                           }
                       }
                   }
               });
                m_ackTimer->setSingleShot(true); // 单次触发模式

}

// 发送时处理溢出
void Widget::sendCommand() {
     m_currentMsgId = (m_currentMsgId >= MAX_MSG_ID) ? 0 : m_currentMsgId + 1;
    }

void Widget::updateModeLabel(const QString &text) {
    ui->CurrentModeLabel->setText(text); // 安全更新UI
}
void Widget::updateHumiEdit(float value) {
    ui->humviewEdit->setText(QString::number(value));
}



Widget::~Widget(){
    m_ackTimer->stop();
        delete m_ackTimer;
        delete client;
        delete ui;
}
//连接服务器
void Widget::on_connectButton_clicked(){
    client->connectToHost();
}

//显示时间
void Widget::Datatimeout_slot()
{
    Data=Data.addMSecs(30);
    QString year = Data.toString("yyyy-MM-dd dddd");
    QString time = Data.toString("hh:mm:ss");
    ui->TimeLable->setText(time);
    ui->YearLable->setText(year);

}

//连接成功调用这个槽函数
void Widget::connectSuccess(){
    QString subtopic ;
    QStringList topics = {

            "target_humi_view",
            "temp_view",
            "sound_view",
            "state_view",
            "HUM_STATUS",
            "water_view",
            "hum_view"
        };
        for (const QString &topic : topics) {
            client->subscribe(topic,1);
        }

        // 新增确认主题订阅
        client->subscribe(ACK_TOPIC_STATE, 1);
        client->subscribe(ACK_TOPIC_HUMI, 1);
        client->subscribe(ACK_TOPIC_HUMIDIFIER, 1);

    ui->connectButton->setText("已连接到服务器");
    QMessageBox::information(this,"连接提示","成功连接到服务器！");
  //  QMessageBox::warning(this, "连接提示", "服务器断开，正在尝试重连...");


    connect(&qtimer,SIGNAL(timeout()),this,SLOT(Datatimeout_slot()));

    Data= QDateTime::currentDateTime();
    qtimer.start(30);

    connect(client,&QMqttClient::messageReceived,this,&Widget::receiveMessage);
    connect(client,&QMqttClient::disconnected,[this]()
    {
        QMessageBox::warning(this,"连接提示","服务器断开");
        ui->connectButton->setText("连接服务器");
    });
}

//接收槽函数
void Widget::receiveMessage(const QByteArray &buf, const QMqttTopicName &topic) {
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(buf, &err);
    if (err.error != QJsonParseError::NoError) return;

    QJsonObject rootobj = doc.object();
    QString topicName = topic.name();

    if (topicName == "temp_view") {
        float temperature = rootobj["value"].toDouble();
        m_lastTemp = QString::number(temperature, 'f', 1) + "℃";
        m_lastUpdateTime = QDateTime::currentDateTime(); // 更新时间戳
    }
    else if (topicName == "hum_view") {
        float humidity = rootobj["value"].toDouble();
        m_lastHumi = QString::number(humidity) + "%";
        m_lastUpdateTime = QDateTime::currentDateTime();
    }
    else if (topicName == "sound_view") {
        float soundLevel = rootobj["value"].toDouble();
        m_lastSound = QString::number(soundLevel) + "dB";
        m_lastUpdateTime = QDateTime::currentDateTime();
        #ifdef QT_DEBUG
        qDebug() << "========================================";
        qDebug() << "主题：" << topic.name();
        qDebug() << "原始数据：" << buf;
        qDebug() << "解析后数据：" << rootobj;
        qDebug() << "噪音值：" << rootobj["value"].toDouble();
        #endif
    }
    else if (topicName == "water_view") {
        float waterLevel = rootobj["value"].toDouble();
        m_lastWater = QString::number(waterLevel) + "cm";
        m_lastUpdateTime = QDateTime::currentDateTime();
         #ifdef QT_DEBUG
        qDebug() << "========================================";
        qDebug() << "主题：" << topic.name();
        qDebug() << "原始数据：" << buf;
        qDebug() << "解析后数据：" << rootobj;
        qDebug() << "水位值：" << rootobj["value"].toDouble();
        #endif
    }
    else if (topicName == "state_view") {
        int state = rootobj["value"].toInt();
        QString stateText;
        switch (state) {
            case 0: stateText = "手动"; break;
            case 1: stateText = "睡眠"; break;
            case 2: stateText = "关机"; break;
            default: stateText = "未知";
        }
        // 通过invokeMethod调用
        QMetaObject::invokeMethod(
            this,
            "updateModeLabel",
            Qt::QueuedConnection,
            Q_ARG(QString, stateText)
        );
    }
    // 修改target_humi_view回调逻辑
    else if (topicName == "target_humi_view") {
        float targetHumi = rootobj["value"].toDouble();
        QMetaObject::invokeMethod(
            this,
            "updateHumiEdit",
            Qt::QueuedConnection,
            Q_ARG(float, targetHumi)
        );
    }

    // 处理确认消息
    // 处理确认消息（特定主题）
    if (topicName == ACK_TOPIC_STATE ||
        topicName == ACK_TOPIC_HUMI ||
        topicName == ACK_TOPIC_HUMIDIFIER)
    {
        QJsonObject ack = doc.object();
        QString result = ack["result"].toString();
        int receivedMsgId = ack["msgId"].toInt(); // 从确认消息中提取消息ID

        // 关键校验：匹配消息ID和当前期望的确认类型
        // 在确认消息处理分支中添加
        if (result == "success" && receivedMsgId == m_currentMsgId) {
            // 从队列中移除已确认的消息
            for (int i = 0; i < m_pendingMessages.size(); ++i) {
                if (m_pendingMessages[i].msgId == receivedMsgId) {
                    m_pendingMessages.removeAt(i);
                    break;
                }
            }
            // 更新UI
            ui->StatusLabel->setText("设备已确认操作");
            QTimer::singleShot(3000, [this]() { ui->StatusLabel->clear(); });
        }
        return;
    }

}

//模式切换

void Widget::on_modePushButton_clicked()
{

    sendCommand();

    currentState = (currentState + 1) % 3;

    // 生成带消息ID的命令
   m_currentMsgId = (m_currentMsgId == MAX_MSG_ID) ? 0 : m_currentMsgId + 1; // 防止溢出
    QJsonObject json;
    json.insert("set_state", currentState);
    json.insert("msgId", m_currentMsgId);
    client->publish(STATE_TOPIC_TO_STM32, QJsonDocument(json).toJson(), 1);

    // 设置确认期望参数
    m_expectedAckType = "state"; // 明确期望的确认类型
    m_waitingForAck = true;
    m_ackTimer->start();

    // 更新UI显示
    switch (currentState) {
        case 0: ui->ChangemodeLable->setText("手动"); break;
        case 1: ui->ChangemodeLable->setText("睡眠"); break;
        case 2: ui->ChangemodeLable->setText("关机"); break;
        default: ui->ChangemodeLable->setText("未知");
    }
}


//发送湿度阈值
void Widget::on_humiChangeButton_clicked() {
    bool ok;
    int humi = ui->humiEdit->text().toInt(&ok);
    if (!ok || humi < 0 || humi > 100) {
      //  QMessageBox::warning(this, "错误", "请输入0-100的整数");
        ui->StatusLabel->setText("设备已确认操作");
        QTimer::singleShot(3000, [this]() { ui->StatusLabel->clear(); });
        return;
    }

    // 生成带消息ID的命令

   m_currentMsgId = (m_currentMsgId == MAX_MSG_ID) ? 0 : m_currentMsgId + 1;

    QJsonObject json;
    json.insert("set_humi", humi);
    json.insert("msgId", m_currentMsgId);
     QByteArray payload = QJsonDocument(json).toJson();

     // 添加到待确认队列
        PendingMessage msg;
        msg.topic = TARGET_HUMI_TO_STM32;
        msg.payload = payload;
        msg.msgId = m_currentMsgId;
        msg.lastSentTime = QDateTime::currentDateTime();
        m_pendingMessages.append(msg);

        // 首次发送
        client->publish(msg.topic, msg.payload, 1); // QoS=1
        // 启动确认定时器（如果未运行）
            if (!m_ackTimer->isActive()) {
                m_ackTimer->start();
            }

            // 更新UI提示
            ui->StatusLabel->setText("等待设备确认...");
   // client->publish(TARGET_HUMI_TO_STM32, QJsonDocument(json).toJson(), 1);

    // 设置确认期望参数
    m_expectedAckType = "humi";
    m_waitingForAck = true;
    m_ackTimer->start();

    // 可选：更新显示
    // ui->humviewEdit->setText(QString::number(humi));
}




//开灯槽函数
void Widget::on_ledButton_clicked()
{
    static uint8_t led =0;
    char buf[15];

    led=!led;
    if(led == 1)    ui->ledButton->setStyleSheet("border-image: url(:/picture/led_on.png)");//更改样式表
    else            ui->ledButton->setStyleSheet("border-image: url(:/picture/led_off.png)");

    sprintf(buf,"{\"Led_SW\":%d}",led);
  //  client->publish(pubtopic,buf);//发布数据
}

//窗帘槽函数
void Widget::on_clButton_clicked()
{
    static uint8_t cl_flag =0;
    char buf[15];

    cl_flag=!cl_flag;
    if(cl_flag == 1)    ui->clButton->setStyleSheet("border-image: url(:/picture/cl_on.png)");//更改样式表
    else                ui->clButton->setStyleSheet("border-image: url(:/picture/cl_off.png)");

    sprintf(buf,"{\"Curtain_SW\":%d}",cl_flag);

   // client->publish(pubtopic,buf);//发布数据
}
//开门槽函数
void Widget::on_doorButton_clicked()
{
    static uint8_t door =0;
    char buf[15];

    door=!door;
    if(door == 1)       ui->doorButton->setStyleSheet("border-image: url(:/picture/door_on.png)");//更改样式表
    else                ui->doorButton->setStyleSheet("border-image: url(:/picture/door_off.png)");

    sprintf(buf,"{\"Door_SW\":%d}",door);

   // client->publish(pubtopic,buf);//发布数据
}
//开风扇槽函数
void Widget::on_fanButton_clicked()
{
    static uint8_t fan =0;
    char buf[15];

    fan=!fan;
    if(fan == 1)    ui->fanButton->setStyleSheet("border-image: url(:/picture/fan_on.png)");//更改样式表
    else            ui->fanButton->setStyleSheet("border-image: url(:/picture/fan_off.png)");

    sprintf(buf,"{\"Fan_SW\":%d}",fan);

  //  client->publish(pubtopic,buf);//发布数据
}
//开加湿器槽函数
void Widget::on_jsqButton_clicked() {

    sendCommand();
   // static int currentState = 0; // 0:手动, 1:睡眠, 2:关机
    currentState = (currentState + 1) % 3;

    // 生成带消息ID的命令
    char buf[64];
   m_currentMsgId = (m_currentMsgId == MAX_MSG_ID) ? 0 : m_currentMsgId + 1;
    sprintf(buf, "{\"set_state\":%d,\"msgId\":%d}", currentState, m_currentMsgId);
    client->publish(STATE_TOPIC_TO_STM32, buf,1);

    // 设置确认期望参数
   m_expectedAckType = "humidifier";
    m_waitingForAck = true;
    m_ackTimer->start();

    // 更新按钮图标
    QString iconPath = (currentState == 2) ? ":/picture/jsq_off.png" : ":/picture/jsq_on.png";
    ui->jsqButton->setStyleSheet("border-image: url(" + iconPath + ");");

    // 更新模式显示
    QString stateText;
    switch (currentState) {
        case 0: stateText = "手动"; break;
        case 1: stateText = "睡眠"; break;
        case 2: stateText = "关机"; break;
        default: stateText = "未知";
    }
    ui->CurrentModeLabel->setText(stateText);
}

void Widget::refreshUI() {
    // 计算当前时间与最后更新时间的时间差
    qint64 elapsedSeconds = m_lastUpdateTime.secsTo(QDateTime::currentDateTime());

    // 如果超过2秒未更新，显示旧数据并标记为灰色
    if (elapsedSeconds > 2) {
        ui->tempLable->setStyleSheet("color: gray;");
        ui->humiLable->setStyleSheet("color: gray;");
        ui->SoundLable->setStyleSheet("color: gray;");
        ui->WaterLable->setStyleSheet("color: gray;");
    } else {
        // 2秒内有更新，恢复默认颜色
        ui->tempLable->setStyleSheet("");
        ui->humiLable->setStyleSheet("");
        ui->SoundLable->setStyleSheet("");
        ui->WaterLable->setStyleSheet("");
    }

    // 始终显示最后一次有效数据
    ui->tempLable->setText(m_lastTemp);
    ui->humiLable->setText(m_lastHumi);
    ui->SoundLable->setText(m_lastSound);
    ui->WaterLable->setText(m_lastWater);
}

QString Widget::getExpectedAckTopic() const {
    if (m_expectedAckType == "state") return ACK_TOPIC_STATE;
    if (m_expectedAckType == "humi") return ACK_TOPIC_HUMI;
    if (m_expectedAckType == "humidifier") return ACK_TOPIC_HUMIDIFIER;
    return QString();
}
