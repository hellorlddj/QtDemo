#ifndef JSONQT_VOC_H
#define JSONQT_VOC_H
// Qt lib import
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QBuffer>
#include <QDebug>
#include <QMediaPlayer>

// JasonQt lib import
#include "JasonQt_Net.h"

#include <QObject>

class JsonQt_VoC : public QObject
{
    Q_OBJECT
private:
    QString m_apiKey;
    QString m_secretKey;
    QString m_token;

    QByteArray m_buf;
    QBuffer *m_buffer;
    QMediaPlayer *player;
    JasonQt_Net::HTTP m_http;
public:
    QString voc_spd = "5";
    QString voc_vol = "5";
    QString voc_per = "0";
    QString voc_pit = "5";
    QString voc_MAC = "00-FF-F2-3E-66-2B";
public:
    JsonQt_VoC(const QString &apiKey, const QString &secretKey);
    bool getfreshToken(void);
    void getAudio(QString str);
};

#endif // JSONQT_VOC_H
