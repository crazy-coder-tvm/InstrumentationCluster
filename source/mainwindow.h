#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSettings>
#include <QTimer>
#include <QMutex>
#include <QString>
#include <QPair>

#include "qcgaugewidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void timerCallback();

    void startUpDemo();

    void emulateCarData();

    void scrollText();

	void onAndroidSocketRead();

    void on_rainSensor_clicked();

    void on_dustSensor_clicked();

    void on_speechSensor_clicked();

    void on_distraction_clicked();

    void on_temperature_clicked();

    void on_fuelSensor_clicked();

    void on_gyroscope_clicked();

    void onNewConnection();
    void onServerReadyRead();
    void onSocketStateChanged(QAbstractSocket::SocketState socketState);

private:
    void addSpeedGuage(const QList<QPair<QColor, float> >& color);
    void addRpmGauge(const QList<QPair<QColor, float> >& color);

private:
    Ui::MainWindow *ui;
    QMovie *mMovie;
    QTcpServer  _mServerSocket;
    //QList<QTcpSocket*> _mClientSockets;
    QTcpSocket* _mClientSockets;

    QTcpSocket  mAndroidSocket;
    QcGaugeWidget * mSpeedGauge;
    QcNeedleItem *mSpeedNeedle;
    QcGaugeWidget * mRpmGauge;
    QcNeedleItem *mRpmNeedle;
    QList<QPair<QColor, float> > mRedBand;
    QList<QPair<QColor, float> > mGreenBand;
    QList<QPair<QColor, float> > mBlueBand;
    QList<QPair<QColor, float> > mGreyBand;
    QTimer *mDate;
    QTimer *mDemo;
    QTimer *mCarData;
    QTimer *mSrollText;

    QList<float> mSpeedData;
    QList<float> mRpmData;
    QList<float> mGearData;
    QList<float>::iterator mSpeedDataIt;
    QList<float>::iterator mRpmDataIt;
    QList<float>::iterator mGearDataIt;
    QMutex mMutex;

    bool mrainSensor;
    bool mdustSensor;
    bool mspeechSensor;
    bool mdistraction;
    bool mtemperature;
    bool mtripEvent;
    bool mfuelSensor;
    char mgyroscope;
};

#endif // MAINWINDOW_H
