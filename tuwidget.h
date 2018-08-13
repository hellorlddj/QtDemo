#ifndef TUWIDGET_H
#define TUWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QTabWidget>
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include <QComboBox>
#include <QList>
#include <QNetworkInterface>

#include <JasonQt_Vop.h>
#include <jsonqt_voc.h>
#include <jasonqt_tuling.h>
using namespace JasonQt_Vop;

namespace Ui {
class TuWidget;
}

class TuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TuWidget(QWidget *parent = 0);
    ~TuWidget();
    void TuLing(const char *str);
    QFormLayout* SetVocLayout();
    QString getHostMacAddress();
    bool data_parsing(QByteArray vop_data);
    QTcpSocket *commandSocket,*recvDataSocket;

    BaiduVop *m_baiduvop;
    JasonQt_TuLing *u_tuling;
    JsonQt_VoC *baiduvoc;
    char*dataStr;

    QTabWidget *ControlTab;
    QPushButton *Speak;
    QPushButton *SendMsg,*saveVocSetting,*defaultVocSetting;
    QPushButton *Isable2Speak,*VoiceOrKeyBoard;
    QLineEdit *inputLE;
    QTextEdit *showTE;
    QSlider *sliderVoc[3];
    QSpinBox *spinBoxVoc[3];
    QComboBox *voc_speaker;
    bool isableVoc,isKeyBoard;
    QString hostMac;

public slots:
    void BtnSpeakPress();
    void BtnSpeakRelease();
    void BtnSendMsg();
    void BtnIsableVoc();
    void BtnSaveVocSetting();
    void BtnDefaultVocSetting();
    void BtnIsVorK();
    void SliderVoc();
private:
    Ui::TuWidget *ui;
};

#endif // TUWIDGET_H
