#include "jasonqt_tuling.h"

JasonQt_TuLing::JasonQt_TuLing(const QString &apiKey) : m_apiKey(apiKey)
{

}

QString JasonQt_TuLing::getTuLingResult(QString strV){
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
    //QString apiKeyStr = QObject::trUtf8("b5071b21bac64407845954e079700d0e");
    userInfo.insert("apiKey",m_apiKey);
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
