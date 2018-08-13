﻿#ifndef __JasonQt_Vop_h__
#define __JasonQt_Vop_h__

// Qt lib import
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QBuffer>
#include <QDebug>

#include <QAudioInput>
#include <QAudioDeviceInfo>

// JasonQt lib import
#include "JasonQt_Net.h"

namespace JasonQt_Vop
{

class BaiduVop: public QObject
{
    Q_OBJECT

private:
    QAudioDeviceInfo m_currentDevice;

    QString m_apiKey;
    QString m_secretKey;
    QString m_token;

    QAudioInput *m_audioInput;
    QByteArray m_buf;
    QBuffer *m_buffer;

    JasonQt_Net::HTTP m_http;

public:
    BaiduVop(const QString &apiKey, const QString &secretKey);

    void setDevice(const QAudioDeviceInfo &device);
   // QString getTuLingResult(QString strV);
public slots:
    bool refreshToken(void);

    bool start(void);

    void getAudio()
    {
  //      qDebug()<<"xxxxxxxxxxxx";
    }
    std::pair<bool, QString> finish(void);
};

}

#endif//__JasonQt_Vop_h__
