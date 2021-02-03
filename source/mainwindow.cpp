#include <mainwindow.h>

#include <QDebug>
#include <QTime>
#include <QFile>
#include <QHostAddress>
#include <QMovie>
#include <QThread>

#include <ui_mainwindow.h>

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#define LINE_GREY_GRADIENT "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, x3:0, y3:0,x4:1, y4:0,\
    stop:0 rgba(0, 0, 0, 255),\
    stop:0.25 rgba(136, 136, 136, 255),\
    stop:0.75 rgba(136, 136, 136, 255),\
    stop:1.0 rgba(0, 0, 0, 255));"

#define LINE_GREEN_GRADIENT "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, x3:0, y3:0,x4:1, y4:0,\
    stop:0 rgba(0, 0, 0, 255),\
    stop:0.25 rgba(154, 205, 50, 255),\
    stop:0.75 rgba(154, 205, 50, 255),\
    stop:1.0 rgba(0, 0, 0, 255));"

#define LINE_RED_GRADIENT "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, x3:0, y3:0,x4:1, y4:0,\
    stop:0 rgba(0, 0, 0, 255),\
    stop:0.25 rgba(255, 0, 0, 255),\
    stop:0.75 rgba(255, 0, 0, 255),\
    stop:1.0 rgba(0, 0, 0, 255));"


#define LINE_BLUE_GRADIENT "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, x3:0, y3:0,x4:1, y4:0,\
    stop:0 rgba(0, 0, 0, 255),\
    stop:0.25 rgba(0, 255, 255, 255),\
    stop:0.75 rgba(0, 255, 255, 255),\
    stop:1.0 rgba(0, 0, 0, 255));"

#define SCROLL_TEXT_MAX_SIZE 28

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Hide title bar
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    //setProperty("windowOpacity", 0.5);

    move(450, 75);

    mrainSensor = false;
    mdustSensor = false;
    mspeechSensor = false;
    mdistraction = false;
    mtemperature = false;
    mtripEvent = false;
    mfuelSensor = false;
    mgyroscope = 0;

    mRpmGauge = NULL;
    mSpeedGauge = NULL;

    // Draw album art
    QPixmap* albumart = new QPixmap(":/drawable/albumart");
    ui->albumart->setPixmap(*albumart);
    ui->albumart->setScaledContents(true);
    ui->albumart->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    ui->albumart->show();

    QPixmap* artist = new QPixmap(":/drawable/artist");
    ui->artist->setPixmap(*artist);
    ui->artist->setScaledContents(true);
    ui->artist->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    ui->artist->show();

    QPixmap* album = new QPixmap(":/drawable/album");
    ui->album->setPixmap(*album);
    ui->album->setScaledContents(true);
    ui->album->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    ui->album->show();

    QPixmap* title = new QPixmap(":/drawable/title");
    ui->title->setPixmap(*title);
    ui->title->setScaledContents(true);
    ui->title->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    ui->title->show();

    QPixmap* lCornerLight = new QPixmap(":/drawable/lCornerLight");
    ui->lCornerLight->setPixmap(*lCornerLight);
    ui->lCornerLight->setScaledContents(true);
    ui->lCornerLight->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    ui->lCornerLight->show();

    QPixmap* rCornerLight = new QPixmap(":/drawable/rCornerLight");
    ui->rCornerLight->setPixmap(*rCornerLight);
    ui->rCornerLight->setScaledContents(true);
    ui->rCornerLight->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    ui->rCornerLight->show();

    QPixmap* headlights = new QPixmap(":/drawable/headlights");
    ui->headlights->setPixmap(*headlights);
    ui->headlights->setScaledContents(true);
    ui->headlights->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    ui->headlights->show();

    QPixmap* engine = new QPixmap(":/drawable/engine");
    ui->engine->setPixmap(*engine);
    ui->engine->setScaledContents(true);
    ui->engine->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    ui->engine->show();

    QPixmap* fog = new QPixmap(":/drawable/fog");
    ui->fog->setPixmap(*fog);
    ui->fog->setScaledContents(true);
    ui->fog->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    ui->fog->show();

    QPixmap* heat = new QPixmap(":/drawable/heat");
    ui->heat->setPixmap(*heat);
    ui->heat->setScaledContents(true);
    ui->heat->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    ui->heat->show();

    QPixmap* airbag = new QPixmap(":/drawable/airbag");
    ui->airbag->setPixmap(*airbag);
    ui->airbag->setScaledContents(true);
    ui->airbag->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    ui->airbag->show();

    QPixmap* leftturn = new QPixmap(":/drawable/leftturn");
    ui->leftturn->setPixmap(*leftturn);
    ui->leftturn->setScaledContents(true);
    ui->leftturn->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    ui->leftturn->show();

    QPixmap* rightturn = new QPixmap(":/drawable/rightturn");
    ui->rightturn->setPixmap(*rightturn);
    ui->rightturn->setScaledContents(true);
    ui->rightturn->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    ui->rightturn->show();

    QPixmap* fuel = new QPixmap(":/drawable/fuel");
    ui->fuel->setPixmap(*fuel);
    ui->fuel->setScaledContents(true);
    ui->fuel->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    ui->fuel->show();

    QPixmap* battery = new QPixmap(":/drawable/battery");
    ui->battery->setPixmap(*battery);
    ui->battery->setScaledContents(true);
    ui->battery->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    ui->battery->show();

    QPixmap* wiper = new QPixmap(":/drawable/wiper");
    ui->wiper->setPixmap(*wiper);
    ui->wiper->setScaledContents(true);
    ui->wiper->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    ui->wiper->show();

    //    QPixmap* arch = new QPixmap(":/drawable/arch");
    //    ui->arch->setPixmap(*arch);
    //    ui->arch->setScaledContents(true);
    //    ui->arch->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    //    ui->arch->show();

    //    mMovie = new QMovie(":/drawable/wiper");
    //    ui->wiper->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    //    ui->wiper->setScaledContents(true);
    //    ui->wiper->setMovie(mMovie);
    //    ui->wiper->hide();

    QPair<QColor, float> pair;
    pair.first = Qt::gray;
    pair.second = 8.3;
    mRedBand.append(pair);
    pair.first = Qt::darkGray;
    pair.second = 83.33;
    mRedBand.append(pair);
    pair.first = Qt::red;
    pair.second = 100;
    mRedBand.append(pair);

    pair.first = Qt::gray;
    pair.second = 8.3;
    mGreenBand.append(pair);
    pair.first = Qt::darkGray;
    pair.second = 83.33;
    mGreenBand.append(pair);
    //pair.first = Qt::green;
    pair.first = *(new QColor(154,205,50));
    pair.second = 100;
    mGreenBand.append(pair);

    pair.first = Qt::gray;
    pair.second = 8.3;
    mBlueBand.append(pair);
    pair.first = Qt::darkGray;
    pair.second = 83.33;
    mBlueBand.append(pair);
    pair.first = Qt::cyan;
    pair.second = 100;
    mBlueBand.append(pair);

    pair.first = Qt::gray;
    pair.second = 8.3;
    mGreyBand.append(pair);
    pair.first = Qt::darkGray;
    pair.second = 83.33;
    mGreyBand.append(pair);
    pair.first = Qt::darkGray;
    pair.second = 100;
    mGreyBand.append(pair);


    // Time
    ui->time->setText(QTime::currentTime().toString("hh:mm"));
    QFont font("Ubuntu condensed", 12.0, QFont::Bold);
    ui->time->setFont(font);
    ui->time->setAlignment(Qt::AlignCenter);

    ui->temp->setText("28 °c");
    ui->temp->setFont(font);
    ui->temp->setAlignment(Qt::AlignLeft);

    ui->kilometer->setText("3486 km");
    ui->kilometer->setFont(font);
    ui->kilometer->setAlignment(Qt::AlignLeft);

    ui->titleName->setText("Take It Easy");
    ui->titleName->setFont(font);
    ui->titleName->setAlignment(Qt::AlignLeft);

    ui->artistName->setText("The Eagles");
    ui->artistName->setFont(font);
    ui->artistName->setAlignment(Qt::AlignLeft);

    ui->albumName->setText("Eagles - Take It Easy - released on May 1, 1972");
    ui->albumName->setFont(font);
    ui->albumName->setAlignment(Qt::AlignLeft);
    ui->albumName->setMaximumWidth(200);
    ui->albumName->scroll(0, 100);

    ui->rainSensor->setFont(font);
    ui->dustSensor->setFont(font);
    ui->speechSensor->setFont(font);
    ui->distraction->setFont(font);
    ui->temperature->setFont(font);
    ui->fuelSensor->setFont(font);
    ui->gyroscope->setFont(font);

    //Gear
    QFont fontGear("Pirulen", 12.0, QFont::Thin);
    ui->park->setFont(fontGear);
    ui->neutral->setFont(fontGear);
    ui->reverse->setFont(fontGear);
    ui->drive->setFont(fontGear);
    ui->sport->setFont(fontGear);

    ui->park->setStyleSheet("color : green");
    ui->neutral->setStyleSheet("color : yellow");
    ui->reverse->setStyleSheet("color : red");
    ui->drive->setStyleSheet("color : cyan");
    ui->sport->setStyleSheet("color : red");

    ui->rainSensor->setStyleSheet("color : gray");
    ui->dustSensor->setStyleSheet("color : gray");
    ui->speechSensor->setStyleSheet("color : gray");
    ui->distraction->setStyleSheet("color : gray");
    ui->temperature->setStyleSheet("color : gray");
    ui->fuelSensor->setStyleSheet("color : gray");
    ui->gyroscope->setStyleSheet("color : gray");

    addSpeedGuage(mGreyBand);
    addRpmGauge(mGreyBand);

    mDate = new QTimer(this);
    connect(mDate, SIGNAL(timeout()), this, SLOT(timerCallback()));
    mDate->start(5000);

    // Gradient lines
    ui->topLine->setStyleSheet(LINE_GREY_GRADIENT);
    ui->bottomLine->setStyleSheet(LINE_GREY_GRADIENT);
    ui->endLine->setStyleSheet(LINE_GREY_GRADIENT);

    mDemo = new QTimer(this);
    connect(mDemo, SIGNAL(timeout()), this, SLOT(startUpDemo()));
    mDemo->start(5);

    mCarData = new QTimer(this);
    mCarData->setInterval(10);
    connect(mCarData, SIGNAL(timeout()), this, SLOT(emulateCarData()));

    mSrollText = new QTimer(this);
    mSrollText->setInterval(250);
    connect(mSrollText, SIGNAL(timeout()), this, SLOT(scrollText()));

    // Car data
    QFile file("RRData.csv");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return;
    } else {
        QStringList wordList;
        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            wordList.append(line.split('\n').at(0));
        }


        for (QStringList::iterator it = wordList.begin(); it != wordList.end(); ++it)
        {
            QString row = *it;
            std::vector<float> vect;
            std::stringstream ss(row.toStdString());

            for (float i; ss >> i;) {
                vect.push_back(i);
                if (ss.peek() == ',')
                    ss.ignore();
            }
            //qInfo()<<vect[8];
            mSpeedData.append(vect[0]);
            mRpmData.append(vect[1]);
            mGearData.append(vect[2]);

        }

        mSpeedDataIt  = mSpeedData.begin();
        mRpmDataIt = mRpmData.begin();
        mGearDataIt = mGearData.begin();
    }

    // Network
    QSettings settings("ip.ini", QSettings::IniFormat);
    qInfo() << settings.fileName();
    settings.sync();
    settings.beginGroup("Tcp");
    QString ipAddressE = settings.value("emulator_ip_address", "127.0.0.1").toString();
    QVariant portNumE = settings.value("emulator_port_number", "7999").toInt();
    QString ipAddressS = settings.value("host_ip_address", "127.0.0.1").toString();
    QVariant portNumS = settings.value("host_port_number", "7999").toInt();
    settings.endGroup();
    qInfo() <<ipAddressE << " "  << qvariant_cast<qint16>(portNumE);
    qInfo() <<ipAddressS << " "  << qvariant_cast<qint16>(portNumS);

    // Connect to emulator
    mAndroidSocket.setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    mAndroidSocket.connectToHost(QHostAddress(ipAddressE), qvariant_cast<qint16>(portNumE));
    connect(&mAndroidSocket, SIGNAL(readyRead()), this, SLOT(onAndroidSocketRead()));

    // Setup server
    _mServerSocket.listen(QHostAddress::Any, qvariant_cast<qint16>(portNumS));
    connect(&_mServerSocket, SIGNAL(newConnection()), this, SLOT(onNewConnection()));

    _mClientSockets = NULL;
}

void MainWindow::onNewConnection()
{
    qDebug()<<"New connection received from Arduino";
    _mClientSockets = _mServerSocket.nextPendingConnection();
    connect(_mClientSockets, SIGNAL(readyRead()), this, SLOT(onServerReadyRead()));
    connect(_mClientSockets, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(onSocketStateChanged(QAbstractSocket::SocketState)));
}

void MainWindow::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState)
    {
        qDebug()<<"Arduino got disconected";
        _mClientSockets = NULL;
    }
}

void MainWindow::onServerReadyRead()
{
    // Forward the data received from Adrino to Android
    QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
    QByteArray data = sender->readAll();
    if(data.length() > 3) {
        qDebug()<<"Data received from Arduino" << data;
        data.append('\n');
        mAndroidSocket.write(data);
    } else {
        qDebug()<<"Invalid length data received from Arduino"<<data;
    }
}

void MainWindow::addSpeedGuage(const QList<QPair<QColor, float> > & color)
{
    QMutexLocker lock(&mMutex);
    if(mSpeedGauge != NULL) delete mSpeedGauge;
    // Speedometer
    mSpeedGauge = new QcGaugeWidget;
    //mSpeedGauge->addArc(120);
    //mSpeedGauge->addBackground(98);
    QcBackgroundItem *bkg0 = mSpeedGauge->addBackground(95);
    bkg0->clearrColors();
    bkg0->addColor(0.1,Qt::darkGray);
    bkg0->addColor(0.6,Qt::black);

    //this will add side color to gauge
    QcBackgroundItem *bkg1 = mSpeedGauge->addBackground(92);
    bkg1->clearrColors();
    bkg1->addColor(0.7,Qt::black);
    bkg1->addColor(1.0,Qt::darkGray);

    QcBackgroundItem *bkg2 = mSpeedGauge->addBackground(88);
    bkg2->clearrColors();
    //this two line below will set background color of gauge
    bkg2->addColor(0.7,Qt::black);
    bkg2->addColor(1.0,Qt::darkGray);
    //add some shadow like glass
    mSpeedGauge->addColorBand(57)->setColors(color);

    mSpeedGauge->addDegrees(88)->setValueRange(0,240);

    // add value rage
    mSpeedGauge->addValues(70)->setValueRange(0, 240);

    //set gauge title
    mSpeedGauge->addLabel(30)->setText("kmph");

    //QcLabelItem *lab = mSpeedGauge->addLabel(40);
    //lab->setText("0");
    mSpeedNeedle = mSpeedGauge->addNeedle(70);

    //add lable
    //mSpeedNeedle->setLabel(lab);
    // set needle color
    mSpeedNeedle->setColor(Qt::white);
    //add range for your gauge => it should be just like the gauge range
    mSpeedNeedle->setValueRange(0,240);
    mSpeedGauge->addBackground(7);
    //add some shadow like glass
    //mSpeedGauge->addGlass(88);
    //add this to your vertical layout
    //mSpeedNeedle->setCurrentValue(20);
    mSpeedGauge->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    ui->speed->addWidget(mSpeedGauge);
}

void MainWindow::addRpmGauge(const QList<QPair<QColor, float> >& color)
{
    QMutexLocker lock(&mMutex);
    if(mRpmGauge != NULL) delete mRpmGauge;

    // Odometer
    mRpmGauge = new QcGaugeWidget;
    //mRpmGauge->addArc(120);
    //mRpmGauge->addBackground(98);
    QcBackgroundItem *bkg0 = mRpmGauge->addBackground(95);
    bkg0->clearrColors();
    bkg0->addColor(0.1,Qt::darkGray);
    bkg0->addColor(0.6,Qt::black);

    //this will add side color to gauge
    QcBackgroundItem *bkgRpm = mRpmGauge->addBackground(92);
    bkgRpm->clearrColors();
    bkgRpm->addColor(0.7,Qt::black);
    bkgRpm->addColor(1.0,Qt::darkGray);

    QcBackgroundItem *bkg2Rpm = mRpmGauge->addBackground(88);
    bkg2Rpm->clearrColors();
    //this two line below will set background color of gauge
    bkg2Rpm->addColor(0.7,Qt::black);
    bkg2Rpm->addColor(1.0,Qt::darkGray);
    mRpmGauge->addColorBand(57)->setColors(color);

    mRpmGauge->addDegrees(88)->setValueRange(0,12);

    // add value rage
    mRpmGauge->addValues(70)->setValueRange(0, 12);

    //set gauge title
    mRpmGauge->addLabel(30)->setText("x1000 rpm");
    //QcLabelItem *lab = mSpeedGauge->addLabel(40);
    //lab->setText("0");
    mRpmNeedle = mRpmGauge->addNeedle(70);

    //add lable
    //mSpeedNeedle->setLabel(lab);
    // set needle color
    mRpmNeedle->setColor(Qt::white);
    //add range for your gauge => it should be just like the gauge range
    mRpmNeedle->setValueRange(0,12);
    mRpmGauge->addBackground(7);

    //mRpmGauge->addGlass(90);
    //add this to your vertical layout
    //mSpeedNeedle->setCurrentValue(20);
    mRpmGauge->setStyleSheet("background-color: rgba(0, 0, 0, 0);");
    ui->rpm->addWidget(mRpmGauge);
}

void MainWindow::onAndroidSocketRead()
{
    QByteArray qdata = mAndroidSocket.readAll();

    // Forward the packet to Arduino as well
    if((_mClientSockets != NULL) && (qdata.length() > 3)) {
        //qdata.append('\n');
        _mClientSockets->write(qdata);
    } else {
        qDebug() << "Arduino socket not active: " << qdata;
    }

    //qDebug() << "Received data from Android : " << qdata;

    unsigned char command = (unsigned char) qdata[0];
    unsigned char dlc = (unsigned char) qdata[1];
    unsigned char subcommand = (unsigned char) qdata[2];
    if(command == 0x1 && dlc == 5 && subcommand == 0x9) {
        if((unsigned char)qdata[3] == 0x1) {
            if((unsigned char)qdata[4] == 0x1) {
                addSpeedGuage(mRedBand);
                addRpmGauge(mRedBand);
                ui->topLine->setStyleSheet(LINE_RED_GRADIENT);
                ui->bottomLine->setStyleSheet(LINE_RED_GRADIENT);
                ui->endLine->setStyleSheet(LINE_RED_GRADIENT);
            } else if((unsigned char)qdata[5] == 1) {
                addSpeedGuage(mGreenBand);
                addRpmGauge(mGreenBand);
                ui->topLine->setStyleSheet(LINE_GREEN_GRADIENT);
                ui->bottomLine->setStyleSheet(LINE_GREEN_GRADIENT);
                ui->endLine->setStyleSheet(LINE_GREEN_GRADIENT);
            } else if((unsigned char)qdata[6] == 1) {
                addSpeedGuage(mBlueBand);
                addRpmGauge(mBlueBand);
                ui->topLine->setStyleSheet(LINE_BLUE_GRADIENT);
                ui->bottomLine->setStyleSheet(LINE_BLUE_GRADIENT);
                ui->endLine->setStyleSheet(LINE_BLUE_GRADIENT);
            } else {
                addSpeedGuage(mGreyBand);
                addRpmGauge(mGreyBand);
                ui->topLine->setStyleSheet(LINE_GREY_GRADIENT);
                ui->bottomLine->setStyleSheet(LINE_GREY_GRADIENT);
                ui->endLine->setStyleSheet(LINE_GREY_GRADIENT);
            }
        } else {
            addSpeedGuage(mGreyBand);
            addRpmGauge(mGreyBand);
            ui->topLine->setStyleSheet(LINE_GREY_GRADIENT);
            ui->bottomLine->setStyleSheet(LINE_GREY_GRADIENT);
            ui->endLine->setStyleSheet(LINE_GREY_GRADIENT);
        }

    } else if (command == 0x1 && dlc == 0x3 && subcommand == 0x3) {
        if((unsigned char)qdata[3] == 1) {
            ui->wiper->show();
        } else {
            ui->wiper->hide();
        }
    } else if(command == 0x1 && dlc == 0x3 && subcommand == 0x6) {
        if((unsigned char)qdata[3] == 1) {
            if((unsigned char)qdata[4] == 1) {
                ui->rCornerLight->show();
                ui->lCornerLight->hide();
            } else {
                ui->rCornerLight->hide();
                ui->lCornerLight->show();
            }
        } else {
            ui->rCornerLight->hide();
            ui->lCornerLight->hide();
        }

    } else if(command == 0x1 && dlc == 0x4 && subcommand == 0x6) {
        if((unsigned char)qdata[3] == 1) {
            if((unsigned char)qdata[4] == 1) {
                ui->rCornerLight->show();
                ui->lCornerLight->hide();
            } else {
                ui->rCornerLight->hide();
                ui->lCornerLight->show();
            }
        } else {
            ui->rCornerLight->hide();
            ui->lCornerLight->hide();
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::timerCallback()
{
    ui->time->setText(QTime::currentTime().toString("hh:mm"));
}

void MainWindow::scrollText()
{
    static const char* titleName = "Take It Easy ";
    static const char* artistName = "The Eagles";
    static const char* albumName = "Eagles - Take It Easy - released on May 1, 1972";
    static size_t iTitle = 0;
    static size_t iArtist = 0;
    static size_t iAlbum = 0;

    QByteArray titleNameScroll[SCROLL_TEXT_MAX_SIZE];
    QByteArray artistNameScroll[SCROLL_TEXT_MAX_SIZE];
    QByteArray albumNameScroll[SCROLL_TEXT_MAX_SIZE];

    if(strlen(titleName) > SCROLL_TEXT_MAX_SIZE){
        for(int j = 0; j < SCROLL_TEXT_MAX_SIZE; ++j)
            titleNameScroll->append(titleName[iTitle + j]);

        if(iTitle == strlen(titleName)) iTitle = 0;
        else ++iTitle;
    } else {
        for(size_t j = 0; j < strlen(titleName); ++j)
            titleNameScroll->append(titleName[j]);
    }

    if(strlen(artistName) > SCROLL_TEXT_MAX_SIZE){
        for(int j = 0; j < SCROLL_TEXT_MAX_SIZE; ++j)
            artistNameScroll->append(artistName[iArtist + j]);

        if(iArtist == strlen(artistName)) iArtist = 0;
        else ++iArtist;
    } else {
        for(size_t j = 0; j < strlen(artistName); ++j)
            artistNameScroll->append(artistName[j]);
    }

    if(strlen(albumName) > SCROLL_TEXT_MAX_SIZE){
        for(int j = 0; j < SCROLL_TEXT_MAX_SIZE; ++j)
            albumNameScroll->append(albumName[iAlbum + j]);

        if(iAlbum == strlen(albumName)) iAlbum = 0;
        else ++iAlbum;
    } else {
        for(size_t j = 0; j < strlen(albumName); ++j)
            albumNameScroll->append(albumName[j]);
    }

    ui->titleName->setText(titleNameScroll->data());
    ui->artistName->setText(artistNameScroll->data());
    ui->albumName->setText(albumNameScroll->data());

}

void MainWindow::startUpDemo()
{
    static float speed = 0.0f;
    static float rpm = 0.0f;
    static int count = 0;
    static bool blink = false;

    if(count <= 600) {
        QMutexLocker lock(&mMutex);
        mSpeedNeedle->setCurrentValue(speed);
        mRpmNeedle->setCurrentValue(rpm/1000.0f);
        speed += 0.4;
        rpm += 20.0f;
        count++;
    } else {
        QMutexLocker lock(&mMutex);
        mSpeedNeedle->setCurrentValue(speed);
        mRpmNeedle->setCurrentValue(rpm/1000.0f);
        speed -= 0.4;
        rpm -= 20.0f;
    }

    if(count % 100 == 0) {
        if(!blink) {
            ui->headlights->hide();
            ui->airbag->hide();
            ui->battery->hide();
            ui->leftturn->hide();
            ui->rightturn->hide();
            ui->fuel->hide();
            ui->engine->hide();
            ui->fog->hide();
            ui->lCornerLight->hide();
            ui->rCornerLight->hide();
            ui->wiper->hide();
            ui->heat->hide();
            ui->park->setStyleSheet("color : gray");
            ui->neutral->setStyleSheet("color : gray");
            ui->reverse->setStyleSheet("color : gray");
            ui->drive->setStyleSheet("color : gray");
            ui->sport->setStyleSheet("color : gray");
            blink = true;
        } else {
            ui->headlights->show();
            ui->airbag->show();
            ui->battery->show();
            ui->leftturn->show();
            ui->rightturn->show();
            ui->fuel->show();
            ui->engine->show();
            ui->fog->show();
            ui->lCornerLight->show();
            ui->rCornerLight->show();
            ui->wiper->show();
            ui->heat->show();
            ui->park->setStyleSheet("color : green");
            ui->neutral->setStyleSheet("color : yellow");
            ui->reverse->setStyleSheet("color : red");
            ui->drive->setStyleSheet("color : cyan");
            ui->sport->setStyleSheet("color : red");
            blink = false;
        }
        if(count < 300) {
            ui->topLine->setStyleSheet(LINE_BLUE_GRADIENT);
            ui->bottomLine->setStyleSheet(LINE_BLUE_GRADIENT);
            ui->endLine->setStyleSheet(LINE_BLUE_GRADIENT);
        } else if(count < 600) {
            ui->topLine->setStyleSheet(LINE_GREEN_GRADIENT);
            ui->bottomLine->setStyleSheet(LINE_GREEN_GRADIENT);
            ui->endLine->setStyleSheet(LINE_GREEN_GRADIENT);
        } else {
            ui->topLine->setStyleSheet(LINE_RED_GRADIENT);
            ui->bottomLine->setStyleSheet(LINE_RED_GRADIENT);
            ui->endLine->setStyleSheet(LINE_RED_GRADIENT);
        }
    }

    if(speed < 0.1 && count > 600) {
        ui->leftturn->hide();
        ui->rightturn->hide();
        ui->fuel->hide();
        ui->lCornerLight->hide();
        ui->rCornerLight->hide();
        ui->wiper->hide();
        ui->heat->hide();
        ui->park->setStyleSheet("color : gray");
        ui->neutral->setStyleSheet("color : gray");
        ui->reverse->setStyleSheet("color : gray");
        ui->drive->setStyleSheet("color : cyan");
        ui->sport->setStyleSheet("color : gray");
        mDemo->stop();
        mCarData->start();
        mSrollText->start();
        ui->topLine->setStyleSheet(LINE_GREY_GRADIENT);
        ui->bottomLine->setStyleSheet(LINE_GREY_GRADIENT);
        ui->endLine->setStyleSheet(LINE_GREY_GRADIENT);
    }
}

void MainWindow::emulateCarData()
{
    static bool gear = true;
    if(mSpeedDataIt == mSpeedData.end() || mRpmDataIt == mRpmData.end() || mGearDataIt == mGearData.end()) {
        mSpeedDataIt = mSpeedData.begin();
        mRpmDataIt   = mRpmData.begin();
        mGearDataIt  = mGearData.begin();
    }

    {
        QMutexLocker lock(&mMutex);
        mSpeedNeedle->setCurrentValue(*mSpeedDataIt);
        mRpmNeedle->setCurrentValue(*mRpmDataIt /1000.0f);
    }

    if(*mGearDataIt == 0 && gear) {
        qInfo()<<"Neutral";
        ui->drive->setStyleSheet("color : gray");
        ui->neutral->setStyleSheet("color : yellow");
        gear = false;
    } else if (*mGearDataIt == 1 && !gear) {
        qInfo()<<"Drive";
        ui->neutral->setStyleSheet("color : gray");
        ui->drive->setStyleSheet("color : cyan");
        gear = true;
    }

    ++mSpeedDataIt;
    ++mRpmDataIt;
    ++mGearDataIt;
}

void MainWindow::on_rainSensor_clicked()
{
    if(!mrainSensor) {
        mrainSensor = true;
        ui->rainSensor->setStyleSheet("color : white");

        char data[5] = {0x2, 0x2, 0x2, 0x1, '\n'};
        QByteArray qdata(data, 5);
        mAndroidSocket.write(qdata);
    } else {
        mrainSensor = false;
        ui->rainSensor->setStyleSheet("color : gray");

        char data[5] = {0x2, 0x2, 0x2, 0x0, '\n'};
        QByteArray qdata(data, 5);
        mAndroidSocket.write(qdata);
    }
}

void MainWindow::on_dustSensor_clicked()
{
    if(!mdustSensor) {
        mdustSensor = true;
        ui->dustSensor->setStyleSheet("color : white");

        char data[5] = {0x42, 0x2, 0x16, 0x1, '\n'};
        QByteArray qdata(data, 5);
        mAndroidSocket.write(qdata);
    } else {
        mdustSensor = false;
        ui->dustSensor->setStyleSheet("color : gray");

        char data[5] = {0x42, 0x2, 0x16, 0x0, '\n'};
        QByteArray qdata(data, 5);
        mAndroidSocket.write(qdata);
    }
}

void MainWindow::on_speechSensor_clicked()
{
    if(!mspeechSensor) {
        mspeechSensor = true;
        ui->speechSensor->setStyleSheet("color : white");

        char data[5] = {0x6, 0x2, 0x10, 0x1, '\n'};
        QByteArray qdata(data, 5);
        mAndroidSocket.write(qdata);
    } else {
        mspeechSensor = false;
        ui->speechSensor->setStyleSheet("color : gray");

        char data[5] = {0x6, 0x2, 0x10, 0x0, '\n'};
        QByteArray qdata(data, 5);
        mAndroidSocket.write(qdata);
    }
}

void MainWindow::on_distraction_clicked()
{
    if(!mdistraction) {
        mdistraction = true;
        ui->distraction->setStyleSheet("color : white");

        char data[6] = {0x5, 0x3, 0x7, 0x1, 0x0, '\n'};
        QByteArray qdata(data, 6);
        mAndroidSocket.write(qdata);
    } else {
        mdistraction = false;
        ui->distraction->setStyleSheet("color : gray");

        char data[6] = {0x5, 0x3, 0x7, 0x0, 0x0, '\n'};
        QByteArray qdata(data, 6);
        mAndroidSocket.write(qdata);
    }
}

void MainWindow::on_temperature_clicked()
{
    if(!mtemperature) {
        mtemperature = true;
        ui->temperature->setStyleSheet("color : white");

        char data[5] = {0x3, 0x2, 0x4, 0x2D, '\n'};
        QByteArray qdata(data, 5);
        mAndroidSocket.write(qdata);
    } else {
        mtemperature = false;
        ui->temperature->setStyleSheet("color : gray");

        char data[5] = {0x3, 0x2, 0x4, 0x24, '\n'};
        QByteArray qdata(data, 5);
        mAndroidSocket.write(qdata);
    }
}

void MainWindow::on_fuelSensor_clicked()
{
    if(!mfuelSensor) {
        mfuelSensor = true;
        ui->fuelSensor->setStyleSheet("color : white");
        ui->fuel->show();

        char data[6] = {0x41, 0x3, 0x15, 0x10, 0x2, '\n'};
        QByteArray qdata(data, 6);
        mAndroidSocket.write(qdata);
    } else {
        mfuelSensor = false;
        ui->fuelSensor->setStyleSheet("color : gray");
        ui->fuel->hide();

        char data[6] = {0x41, 0x3, 0x15, 0xF, 0xB, '\n'};
        QByteArray qdata(data, 6);
        mAndroidSocket.write(qdata);
    }
}

void MainWindow::on_gyroscope_clicked()
{
    if(mgyroscope < 2) {
        if(mgyroscope == 0) {
            // Steered right
            ui->gyroscope->setStyleSheet("color : white");
            char data[7] = {0x4, 0x4, 0x5, 0x1, 0x1, 0x0, '\n'};
            QByteArray qdata(data, 7);
            mAndroidSocket.write(qdata);
        } else if(mgyroscope == 1) {
            // Steered right
            ui->gyroscope->setStyleSheet("color : white");
            char data[7] = {0x4, 0x4, 0x5, 0x1, 0x0, 0x0, '\n'};
            QByteArray qdata(data, 7);
            mAndroidSocket.write(qdata);
        }
        ++mgyroscope;
    } else {
        // Straight drive
        ui->gyroscope->setStyleSheet("color : gray");
        char data[7] = {0x4, 0x4, 0x5, 0x0, 0x0, 0x0, '\n'};
        QByteArray qdata(data, 7);
        mAndroidSocket.write(qdata);
        mgyroscope = 0;
    }
}
