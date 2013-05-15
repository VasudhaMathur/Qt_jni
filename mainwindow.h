#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <jni.h>

class QDeclarativeView;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:

    Q_INVOKABLE void sendSMS();

    explicit MainWindow(QWidget *parent = 0);

private:
    QDeclarativeView *ui;
    QObject *Root;

    QObject *messageEdit;
    QObject *phoneEdit;
    QObject *sendButton;

    jobject Java_smsManager_Object;
};

#endif // MAINWINDOW_H
