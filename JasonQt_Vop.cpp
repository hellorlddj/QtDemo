#include "JasonQt_Vop.h"

using namespace JasonQt_Vop;

BaiduVop::BaiduVop(const QString &apiKey, const QString &secretKey):
    m_apiKey(apiKey),//拿参数apikey 初始化BaiduVop lei的构造函数中的m_apikey成员
    m_secretKey(secretKey)
{
    //获取输入设备
    const auto &&availableDevices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    if(!availableDevices.isEmpty())
    {
        m_currentDevice = availableDevices.first();

        QAudioFormat format;
        format.setSampleRate(16000);//设置声音的采样频率
        format.setChannelCount(1);//设置声音的声道
        format.setSampleSize(16);//16位深
        format.setSampleType(QAudioFormat::SignedInt);
        format.setByteOrder(QAudioFormat::LittleEndian);
        format.setCodec("audio/pcm");
        //获取默认的音频输入设备，判断是否支持指定的格式，如果不支持则使用一个邻近的格式
        QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
        if (!info.isFormatSupported(format))
        {
            format = info.nearestFormat(format);
        }

        m_audioInput = new QAudioInput(m_currentDevice, format, this);
        //connect(m_audioInput,SIGNAL(stateChanged(QAudio::State)), this, SLOT(getAudio()));
    }
}

void BaiduVop::setDevice(const QAudioDeviceInfo &device)
{
    m_currentDevice = device;
}

bool BaiduVop::refreshToken(void)
{
    QNetworkRequest request(QUrl(QString("http://openapi.baidu.com/oauth/2.0/token?")));
    QByteArray append = QString("grant_type=client_credentials&client_id=%1&client_secret=%2&").arg(m_apiKey).arg(m_secretKey).toLatin1();
    QByteArray buf;
    request.setRawHeader("Content-Type", "application/json");
    const auto &&flag = m_http.post(request, append, buf, 15000);
    if(!flag) { return false; }

    const auto &&data = QJsonDocument::fromJson(buf).object();
    if(data.isEmpty() || !data.contains("access_token")) {
        return false;
    }
    m_token = data["access_token"].toString();
    return true;
}

bool BaiduVop::start(void)
{
    if(m_token.isEmpty())
    {
        qDebug("BaiduVop::start fail, Need refresh token befor start.");
        return false;
    }
    m_buffer = new QBuffer;
    m_buffer->open(QIODevice::ReadWrite);
    m_audioInput->start(m_buffer);

    return true;
}

/*
dev_pid	语言	模型	是否有标点	备注
1536	普通话(支持简单的英文识别)	搜索模型	无标点	支持自定义词库
1537	普通话(纯中文识别)	输入法模型	有标点	不支持自定义词库
1737	英语		有标点	不支持自定义词库
1637	粤语		有标点	不支持自定义词库
1837	四川话		有标点	不支持自定义词库
1936	普通话远场	远场模型	有标点	不支持
*/
std::pair<bool, QString> BaiduVop::finish(void)
{
    m_audioInput->stop();
    if((m_buffer->data()).isNull()){
        qDebug()<<"NULL ERROR"<<m_buffer->data();
    }
    qDebug()<<" no NULL-ERROR"<<m_buffer->data();
    const auto &sendData = m_buffer->data();
    m_buffer->deleteLater();
    QNetworkRequest request(QUrl("http://vop.baidu.com/server_api"));
    QJsonObject append;
    request.setRawHeader("Content-Type", "application/json");
    append["format"] = "pcm";
    append["rate"] = 16000;
    append["dev_pid"] = 1537;
    append["channel"] = 1;
    append["token"] = m_token;
    //append["lan"] = "zh";
    append["cuid"] = "JasonQt";
    append["speech"] = QString(sendData.toBase64());
    append["len"] = sendData.size();

    QByteArray buf;
    m_http.post(request, QJsonDocument(append).toJson(), buf, 15000);
    QJsonObject acceptedData(QJsonDocument::fromJson(buf).object());

    if(buf.isEmpty() || acceptedData.isEmpty() || !acceptedData.contains("result")) { return { false, buf }; }

    const auto &&message = acceptedData["result"].toArray()[0].toString();
    return { true, message.mid(0, message.size() - 1) };
}
/*
QString BaiduVop::getTuLingResult(QString strV){
    QNetworkRequest request(QUrl("http://openapi.tuling123.com/openapi/api/v2"));
    request.setRawHeader("Content-Type", "application/json");

    QJsonObject append;

    append.insert("reqType",0);
    QJsonObject perception;

    QJsonObject InputText;
    InputText.insert("text",strV);
    perception.insert("inputText",InputText);
    append.insert("perception",perception);

    QJsonObject userInfo;
    QString apiKeyStr = QObject::trUtf8("b5071b21bac64407845954e079700d0e");
    userInfo.insert("apiKey",apiKeyStr);
    userInfo.insert("userId","1");
    append.insert("userInfo",userInfo);

    QByteArray buf;
    m_http.post(request, QJsonDocument(append).toJson(), buf, 15000);
    QJsonObject acceptedData(QJsonDocument::fromJson(buf).object());

    if(buf.isEmpty() || acceptedData.isEmpty() || !acceptedData.contains("results"))
    {
        return  QString(buf);
    }
    QString resultText = "error:get nothing back from tuling";
    QJsonValue valueR = acceptedData.value("results");
    if(valueR.isArray()){
        qDebug()<<"get results";
        QJsonArray resultsJson = valueR.toArray();
        int nsize = resultsJson.size();
        for(int i=0;i<nsize;i++){
            QJsonValue valueRSub = resultsJson.at(i);
            if(valueRSub.isObject()){
                QJsonObject jsonSub = valueRSub.toObject();
                QJsonValue valueV = jsonSub.value("values");
                if(valueV.isObject()){
                    QJsonObject resultsValue = valueV.toObject();
                    if(resultsValue.contains("text")){
                        QJsonValue textJ = resultsValue.value("text");
                        if(textJ.isString()){
                            resultText = textJ.toString();
                        }
                    }
                }
            }
        }
    }

    //const auto &&message = resultsJson["results"].toArray()[0].toString();
    //return message.mid(0, message.size() - 1);
    return resultText;
}
*/
