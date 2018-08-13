#include "tuwidget.h"
#include "ui_tuwidget.h"

TuWidget::TuWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TuWidget)
{
    ui->setupUi(this);
    Speak = new QPushButton();
    Speak->setIcon(QIcon(":/mic.png"));
    Speak->setMaximumWidth(300);
   // Speak->setStyleSheet(QString("QPushButton{border-style:solid;border-top-left-radius:2px;border-top-right-radius:2px;}"));
    //Speak->setIconSize(QSize(64,64));
    Speak->setEnabled(false);
    Speak->setVisible(false);
    SendMsg = new QPushButton("Send");


    Isable2Speak = new QPushButton(QString::fromLocal8Bit("禁用朗读"));
    VoiceOrKeyBoard =new QPushButton(QString::fromLocal8Bit("语音"));
    VoiceOrKeyBoard->setMaximumWidth(100);
    isableVoc = true;
    isKeyBoard = true;

    inputLE = new QLineEdit();
    inputLE->setMinimumSize(300,50);
    inputLE->setMaximumSize(500,50);
    /*对话显示区*/
    showTE = new QTextEdit();
    showTE->setMinimumSize(500,400);
    showTE->setFont(QFont(tr("楷体"),10));
    showTE->setDisabled(true);
    /*操作区*/

    ControlTab = new QTabWidget();
    QWidget *tabShow,*tabSetTool;
    tabShow = new QWidget();
    tabSetTool = new QWidget();
    //ControlTab->setMaximumWidth(500);
    ControlTab->setFont(QFont(tr("楷体"),15));
    ControlTab->addTab(tabShow,"tab 0");
    ControlTab->addTab(tabSetTool,"tab 1");
    ControlTab->setTabText(0,QString::fromLocal8Bit("显示"));
    ControlTab->setTabText(1,QString::fromLocal8Bit("设置"));

    QVBoxLayout *showBox = new QVBoxLayout();
    showBox->addWidget(showTE);
    tabShow->setLayout(showBox);
    /*输入面板*/
    QHBoxLayout *inputBox = new QHBoxLayout();
    inputBox->addWidget(VoiceOrKeyBoard);
    inputBox->addWidget(inputLE);
    inputBox->addWidget(SendMsg);
    //inputBox->addStretch();
    inputBox->addWidget(Speak);

    /*设置面板*/

    //语音合成设置
    QFormLayout *vForm = new QFormLayout();
    vForm = SetVocLayout();
    tabSetTool->setLayout(vForm);

    /*主布局*/
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(ControlTab);
    vbox->addLayout(inputBox);
    setLayout(vbox);
    /*事件处理*/
    connect(Speak,SIGNAL(pressed()),this,SLOT(BtnSpeakPress()));
    connect(Speak,SIGNAL(released()),this,SLOT(BtnSpeakRelease()));
    connect(SendMsg,SIGNAL(clicked(bool)),this,SLOT(BtnSendMsg()));
    connect(Isable2Speak,SIGNAL(clicked(bool)),this,SLOT(BtnIsableVoc()));
    connect(VoiceOrKeyBoard,SIGNAL(clicked(bool)),this,SLOT(BtnIsVorK()));
    connect(inputLE,SIGNAL(returnPressed()),this,SLOT(BtnSendMsg()));
    /*语音识别*/
    m_baiduvop = new BaiduVop("gRbx5RDZ58bfVVOp5mBWWygi","esTST83cWEN4S3xDfM8FmC32Dqq99fKK");
    m_baiduvop->refreshToken();
    /*图灵机器*/
    u_tuling = new JasonQt_TuLing("b5071b21bac64407845954e079700d0e");
    /*语音合成*/
    baiduvoc = new JsonQt_VoC("gRbx5RDZ58bfVVOp5mBWWygi","esTST83cWEN4S3xDfM8FmC32Dqq99fKK");
    if(!(baiduvoc->getfreshToken())){
        showTE->append("something is wrong about voc");
    }

    hostMac = getHostMacAddress();//获取主机MAC地址
    if(hostMac.isNull()){
        showTE->append("system: mac get error");
        baiduvoc->voc_MAC = "none";
    }else{
        baiduvoc->voc_MAC = hostMac;
    }
}

QFormLayout* TuWidget::SetVocLayout(){
    /*
    语速，取值0-15，默认为5中语速
    音调，取值0-15，默认为5中语调
    音量，取值0-15，默认为5中音量
    发音人选择, 0为普通女声，1为普通男生，3为情感合成-度逍遥，4为情感合成-度丫丫，默认为普通女声
    */
    int i=0;
    int nMin = 0;
    int nMax = 15;
    int nSingleStep = 1;
    int defaultValue=5;
    QFormLayout *formL = new QFormLayout();
    QByteArray MsgN[3] = {"语速","音调","音量"};
    //语速、音调、音量
    QLabel *labelVoc[3];
    QHBoxLayout *boxVoc[3];


    for(i=0;i<3;i++){
        boxVoc[i] = new QHBoxLayout();
        labelVoc[i] = new QLabel(QString::fromLocal8Bit(MsgN[i]));

        sliderVoc[i] = new QSlider();

        sliderVoc[i]->setOrientation(Qt::Horizontal);  // 水平方向
        sliderVoc[i]->setMinimumWidth(150);
        sliderVoc[i]->setMinimum(nMin);  // 最小值
        sliderVoc[i]->setMaximum(nMax);  // 最大值
        sliderVoc[i]->setSingleStep(nSingleStep); // 步长
        //sliderVoc[i]->setTickInterval(40);  // 设置刻度间隔
        sliderVoc[i]->setTickPosition(QSlider::TicksAbove);  //刻度在上方1234567
        sliderVoc[i]->setValue(defaultValue);

        spinBoxVoc[i] = new QSpinBox();
        spinBoxVoc[i]->setMinimumWidth(50);
        spinBoxVoc[i]->setMinimum(nMin);
        spinBoxVoc[i]->setMaximum(nMax);
        spinBoxVoc[i]->setSingleStep(nSingleStep);
        spinBoxVoc[i]->setValue(defaultValue);

        boxVoc[i]->addWidget(labelVoc[i]);
        boxVoc[i]->addStretch(1);
        boxVoc[i]->addWidget(spinBoxVoc[i]);
        boxVoc[i]->addWidget(sliderVoc[i]);

        formL->addRow(boxVoc[i]);

        connect(sliderVoc[i],SIGNAL(valueChanged(int)),spinBoxVoc[i],SLOT(setValue(int)));
        connect(spinBoxVoc[i],SIGNAL(valueChanged(int)),sliderVoc[i],SLOT(setValue(int)));
    }
    //发音人
    QLabel *labelVoc_speaker = new QLabel(QString::fromLocal8Bit("发音人"));
    QHBoxLayout *boxVoc_speaker = new QHBoxLayout();
    voc_speaker = new QComboBox();
    voc_speaker->addItem(QString::fromLocal8Bit("普通女声"));
    voc_speaker->addItem(QString::fromLocal8Bit("普通男声"));
    voc_speaker->addItem(QString::fromLocal8Bit("度逍遥"));
    voc_speaker->addItem(QString::fromLocal8Bit("度丫丫"));
    voc_speaker->setCurrentIndex(0);
    boxVoc_speaker->addWidget(labelVoc_speaker);
    boxVoc_speaker->addStretch();
    boxVoc_speaker->addWidget(voc_speaker);
    formL->addRow(boxVoc_speaker);
    //保存设置与还原
    saveVocSetting = new QPushButton(QString::fromLocal8Bit("保存设置"));
    defaultVocSetting = new QPushButton(QString::fromLocal8Bit("还原默认设置"));
    QHBoxLayout *boxBtn = new QHBoxLayout();
    boxBtn->addWidget(saveVocSetting);
    boxBtn->addWidget(defaultVocSetting);
    formL->addRow(boxBtn);
    formL->addRow(Isable2Speak);
    connect(saveVocSetting,SIGNAL(clicked(bool)),this,SLOT(BtnSaveVocSetting()));
    connect(defaultVocSetting,SIGNAL(clicked(bool)),this,SLOT(BtnDefaultVocSetting()));

    return formL;
}

void TuWidget::BtnIsVorK(){
    if(isKeyBoard){
        VoiceOrKeyBoard->setText(QString::fromLocal8Bit("键盘"));
        Speak->setEnabled(true);
        Speak->setVisible(true);
        Speak->resize(300,30);
        inputLE->setEnabled(false);
        inputLE->setVisible(false);
        inputLE->resize(0,50);
        SendMsg->setEnabled(false);
        SendMsg->setVisible(false);
        SendMsg->resize(0,30);
        isKeyBoard = false;
    }else{
        VoiceOrKeyBoard->setText(QString::fromLocal8Bit("语音"));
        inputLE->setEnabled(true);
        inputLE->setVisible(true);
        inputLE->setMinimumSize(300,50);
        SendMsg->setEnabled(true);
        SendMsg->setVisible(true);
        SendMsg->resize(100,30);
        Speak->setEnabled(false);
        Speak->setVisible(false);
        isKeyBoard = true;
    }
}

void TuWidget::BtnSaveVocSetting(){
    baiduvoc->voc_spd = QString::number(sliderVoc[0]->value(),10);
    baiduvoc->voc_vol = QString::number(sliderVoc[1]->value(),10);
    baiduvoc->voc_pit = QString::number(sliderVoc[2]->value(),10);
    baiduvoc->voc_per = QString::number(voc_speaker->currentIndex(),10);

    showTE->append("system: voc setting saved");
}

void TuWidget::BtnDefaultVocSetting(){
    int defaultValue = 5,i=0;
    for(i=0;i<3;i++){
        sliderVoc[i]->setValue(defaultValue);
        spinBoxVoc[i]->setValue(defaultValue);
    }
    voc_speaker->setCurrentIndex(0);

    baiduvoc->voc_spd = QString::number(sliderVoc[0]->value(),10);
    baiduvoc->voc_vol = QString::number(sliderVoc[1]->value(),10) ;
    baiduvoc->voc_pit = QString::number(sliderVoc[2]->value(),10);
    baiduvoc->voc_per = QString::number(voc_speaker->currentIndex(),10);

    showTE->append("system: voc setting reback to default");
}

void TuWidget::SliderVoc(){
    QSlider *sr = static_cast<QSlider *>(sender());

}

void TuWidget::BtnSpeakPress(){
    QPushButton *bt = static_cast<QPushButton *>(sender());
    bt->setIcon(QIcon(":/mic1.png"));

    qDebug() << "start--------------------";
    m_baiduvop->refreshToken();
    m_baiduvop->start();
}

void TuWidget::BtnSpeakRelease(){

    QPushButton *bt = static_cast<QPushButton *>(sender());
    bt->setIcon(QIcon(":/mic.png"));

    qDebug() << "end------------------------";
    const auto &&data = m_baiduvop->finish();
    data_parsing(data.second.toStdString().data());
}

bool TuWidget::data_parsing(QByteArray vop_data){
    char str[10000];
    memset(str,0,sizeof(str));
    qDebug() << "BaiduBackInfo:" <<vop_data.data();

    char *head = strtok(vop_data.data(),":");
    if (strcmp(head,"{\"err_msg\"") == 0)
    {
        showTE->append("error: please speak clearly and loudly or something wrong about vioce input");
        sprintf(str,"\n%s\n","wrong");
        qDebug()<<"error:"<<str;
        //showTE->append(QString(str));

        //TuLing(str);
    }else{
        sprintf(str,"%s",vop_data.data());
        qDebug() << str;
        sprintf(str,"%s",str);
        QString strMsg = QString::fromLocal8Bit("用户：");
        strMsg+=QString(str);
        QString str1 = QString("<p><font size=\"3\" color=\"blue\">%3</font></p>").arg(strMsg);
        showTE->append(str1);
        TuLing(str);
    }

    if(strstr(vop_data.data(),"speech quality error") != NULL)
    {
        return false;
    }
    else
    {
        return true;
    }
    memset(str,0,sizeof(str));
    vop_data.clear();
}

void TuWidget::BtnSendMsg(){
    QString strMsg = QString::fromLocal8Bit("用户：");
    strMsg += inputLE->text();
    if(strcmp(QString::fromLocal8Bit("用户：").toStdString().c_str(),strMsg.toStdString().c_str())==0){
        showTE->append("system: you input nothing,please try again");
    }else{
        qDebug()<<"input"<<strMsg;
        QString str1 = QString("<p><font size=\"3\" color=\"blue\">%3</font></p>").arg(strMsg);
        showTE->append(str1);
        TuLing(strMsg.toStdString().c_str());
        inputLE->clear();
    }
}

void TuWidget::BtnIsableVoc(){
    if(isableVoc){
        Isable2Speak->setText(QString::fromLocal8Bit("启用朗读"));
        isableVoc = false;
        showTE->append("system: voc not able to use");
    }else{
        Isable2Speak->setText(QString::fromLocal8Bit("禁用朗读"));
        isableVoc = true;
        showTE->append("system: voc is able to use");
    }
}

void TuWidget::TuLing(const char *str){
    /*图灵处理*/
    QString strV =QObject::trUtf8(str);
    QString TuStr =QObject::tr((u_tuling->getTuLingResult(strV)).toStdString().c_str());
    if(isableVoc)
        baiduvoc->getAudio(TuStr);

    QString gbkTuStr = QString::fromLocal8Bit("灵度：");
    gbkTuStr += TuStr;
    QString str1 = QString("<p><font size=\"4\" color=\"red\">%3</font></p>").arg(gbkTuStr);
    //showTE->setFont(QFont(tr("楷体"),14));
    showTE->append(str1);
}

QString TuWidget::getHostMacAddress()
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
    int nCnt = nets.count();
    QString strMacAddr = "";
    for(int i = 0; i < nCnt; i ++)
    {
        // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
        if(nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            strMacAddr = nets[i].hardwareAddress();
            break;
        }
    }
    qDebug()<<"MAC: "<<strMacAddr;
    return strMacAddr;
}

TuWidget::~TuWidget()
{
    delete ui;
}
