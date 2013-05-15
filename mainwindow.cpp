#include "mainwindow.h"

#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeView>
#include <QGraphicsObject>
#include <QObject>
#include <QDebug>

static JavaVM *javaVM = 0;
static jclass Java_smsManager = 0;
static jmethodID Java_smsManager_sendTextMessage = 0;
static jmethodID Java_smsManager_Constructor = 0;

// метод вызывается сразу после загрузки модуля
JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
{
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        qCritical()<<"Can't get the enviroument";
        return -1;
    }
    javaVM = vm;

    // поиск класса
    jclass clazz = env->FindClass("org/kde/necessitas/origo/Java_smsManager");

    // создание глобальной ссылки на класс смс менеджера
    Java_smsManager = (jclass)env->NewGlobalRef(clazz);

    // получение конструктора класса смс менеджера
    Java_smsManager_Constructor = env->GetMethodID(Java_smsManager, "<init>", "()V");

    // получение метода для отправки смс
    Java_smsManager_sendTextMessage = env->GetMethodID(Java_smsManager,
                                                       "sendTextMessage", "(Ljava/lang/String;Ljava/lang/String;)Z");

    return JNI_VERSION_1_6;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui = new QDeclarativeView;
    ui->setSource(QUrl("qrc:/main.qml"));
    setCentralWidget(ui);
    ui->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    Root = ui->rootObject();
    ui->rootContext()->setContextProperty("window", this);

    messageEdit = Root->findChild<QObject*>("messageEdit");
    phoneEdit = Root->findChild<QObject*>("phoneEdit");
    sendButton = Root->findChild<QObject*>("sendButton");

    JNIEnv* env;
    // присоединение к потоку
    if (javaVM->AttachCurrentThread(&env, NULL)<0)
    {
        qCritical() << "AttachCurrentThread failed";
        return;
    }

    // создание глобальной ссылки на объект смс менеджера
    Java_smsManager_Object = env->NewGlobalRef(env->NewObject(Java_smsManager, Java_smsManager_Constructor));
    if (!Java_smsManager_Object)
    {
        qCritical() << "Не получилось создать Java_smsManager_Object";
        return;
    }

    // отключение от потока
    javaVM->DetachCurrentThread();
}

void MainWindow::sendSMS()
{
    JNIEnv* env;
    // присоединение к потоку
    if (javaVM->AttachCurrentThread(&env, NULL)<0)
    {
        qCritical() << "AttachCurrentThread failed";
        return;
    }

    QString qPhone = phoneEdit->property("text").toString();
    jstring phone = env->NewString(reinterpret_cast<const jchar*>(qPhone.constData()), qPhone.length());

    QString qText = messageEdit->property("text").toString();
    jstring text = env->NewString(reinterpret_cast<const jchar*>(qText.constData()), qText.length());

    // Вызов метода
    env->CallBooleanMethod(Java_smsManager_Object, Java_smsManager_sendTextMessage, phone, text);

    env->DeleteLocalRef(phone);
    env->DeleteLocalRef(text);
    javaVM->DetachCurrentThread();
}
