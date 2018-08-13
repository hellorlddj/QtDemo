#ifndef JASONQT_TULING_H
#define JASONQT_TULING_H

#include <QObject>
// Qt lib import
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QBuffer>
#include <QDebug>

// JasonQt lib import

#include "JasonQt_Net.h"

class JasonQt_TuLing : public QObject
{
    Q_OBJECT
private:
    QString m_apiKey;

    JasonQt_Net::HTTP m_http;
public:
    JasonQt_TuLing(const QString &apiKey);
    QString getTuLingResult(QString strV);
};

#endif // JASONQT_TULING_H
