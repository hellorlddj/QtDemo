#include "jsonqt_voc.h"

JsonQt_VoC::JsonQt_VoC(const QString &apiKey, const QString &secretKey):
    m_apiKey(apiKey),//拿参数apikey 初始化BaiduVop lei的构造函数中的m_apikey成员
    m_secretKey(secretKey)
{
    player = new QMediaPlayer();
}

bool JsonQt_VoC::getfreshToken(void)
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
    qDebug()<<"m_token:"<<m_token;
    return true;
}
/*
tex	必填	合成的文本，使用UTF-8编码。小于2048个中文字或者英文数字。（文本在百度服务器内转换为GBK后，长度必须小于4096字节）
tok	必填	开放平台获取到的开发者access_token（见上面的“鉴权认证机制”段落）
cuid	必填	用户唯一标识，用来计算UV值。建议填写能区分用户的机器 MAC 地址或 IMEI 码，长度为60字符以内
ctp	必填	客户端类型选择，web端填写固定值1
lan	必填	固定值zh。语言选择,目前只有中英文混合模式，填写固定值zh
spd	选填	语速，取值0-15，默认为5中语速
pit	选填	音调，取值0-15，默认为5中语调
vol	选填	音量，取值0-15，默认为5中音量
per	选填	发音人选择, 0为普通女声，1为普通男生，3为情感合成-度逍遥，4为情感合成-度丫丫，默认为普通女声
aue	选填	3为mp3格式(默认)； 4为pcm-16k；5为pcm-8k；6为wav（内容同pcm-16k）; 注意aue=4或者6是语音识别要求的格式，但是音频内容不是语音识别要求的自然人发音，所以识别效果会受影响。
*/
void JsonQt_VoC::getAudio(QString str){
    if(!getfreshToken()){
        qDebug()<<"there is nothing about m_token";
    }else{
        QByteArray url="http://tsn.baidu.com/text2audio?";
        url.append(QString("&lan=zh&ctp=2"));
        url.append("&cuid=");
        url.append(QUrl::toPercentEncoding(voc_MAC));//MAC地址
        url.append("&spd=");
        url.append(QUrl::toPercentEncoding(voc_spd));//语速
        url.append("&vol=");
        url.append(QUrl::toPercentEncoding(voc_vol));//音量
        url.append("&pit=");
        url.append(QUrl::toPercentEncoding(voc_pit));//音调
        url.append("&per=");
        url.append(QUrl::toPercentEncoding(voc_per));//发音人
        url.append("&tok=");
        url.append(m_token);
        url.append("&tex=");
        url.append(QUrl::toPercentEncoding(str));
        qDebug()<<url;
        if(player == NULL){
            player = new QMediaPlayer();
        }
        player->setMedia(QUrl::fromLocalFile(url));
        player->play();
    }
}
