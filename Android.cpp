/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 */


#include <cassert>
#include <jni.h>
#include <QtAndroidExtras/QAndroidJniEnvironment>
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QtAndroidExtras>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include "make_unique.hpp"
#include "Android.hpp"

#define     JAVA_CLASS      "com/ertebat/client/Android"

using namespace Ertebat;

Android::OpenFileDialogAcceptedHandler_t OpenFileDialogAcceptedHandler;
Android::OpenFileDialogRejectedHandler_t OpenFileDialogRejectedHandler;

static void OpenFileDialogAccepted(JNIEnv *, jobject, jstring path)
{
    if (OpenFileDialogAcceptedHandler)
        OpenFileDialogAcceptedHandler(QAndroidJniObject(path).toString());
}

static void OpenFileDialogRejected(JNIEnv *, jobject)
{
    if (OpenFileDialogRejectedHandler)
        OpenFileDialogRejectedHandler();
}

static JNINativeMethod s_nativeMethods[] = {
    { "OpenFileDialogAccepted", "(Ljava/lang/String;)V", (void *)OpenFileDialogAccepted },
    { "OpenFileDialogRejected", "()V", (void *)OpenFileDialogRejected }
};

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *)
{
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        qCritical() << "  * Could not get the JNI enviroument !!";
        return -1;
    }

    jclass clazz = env->FindClass(JAVA_CLASS);
    if (!clazz) {
        qCritical() << "  * Could not find the Android interface class !!";
        return -1;
    }

    if (env->RegisterNatives(
                clazz,
                s_nativeMethods,
                sizeof(s_nativeMethods) / sizeof(s_nativeMethods[0])
                ) < 0) {
        qCritical() << "   * Failed to Register native methods !!";
        return -1;
    }

    return JNI_VERSION_1_6;
}

struct Android::Impl
{
    QAndroidJniEnvironment AndroidJniEnvironment;
};

Android::Android() :
    m_pimpl(std::make_unique<Android::Impl>())
{

}

Android::~Android() = default;

void Android::OpenFileDialogAccepted(OpenFileDialogAcceptedHandler_t handler)
{
    OpenFileDialogAcceptedHandler = handler;
}

void Android::OpenFileDialogRejected(OpenFileDialogRejectedHandler_t handler)
{
    OpenFileDialogRejectedHandler = handler;
}

bool Android::ExceptionCheck()
{
    return m_pimpl->AndroidJniEnvironment->ExceptionCheck();
}

void Android::ExceptionClear()
{
    if (m_pimpl->AndroidJniEnvironment->ExceptionCheck()) {
        m_pimpl->AndroidJniEnvironment->ExceptionClear();
    }
}

void Android::Initialize()
{
    if (!IsInitialized()) {
        ExceptionClear();

        this->callMethod<void>("<init>", "()V");
    }
}

void Android::Release()
{
    ExceptionClear();

    QAndroidJniObject::callStaticMethod<jboolean>(
                JAVA_CLASS, "release", "()Z");
}

bool Android::IsInitialized()
{
    ExceptionClear();

    return QAndroidJniObject::callStaticMethod<jboolean>(
                JAVA_CLASS, "isInitialized", "()Z");
}

QString Android::GetScreenType()
{
    ExceptionClear();

    QAndroidJniObject ret = QAndroidJniObject::callStaticObjectMethod(
                JAVA_CLASS,
                "getScreenType",
                "()Ljava/lang/CharSequence;");

    return ret.toString();
}

bool Android::Notify(const QString &title, const QString &text, const int id)
{
    ExceptionClear();

    jboolean ret = QAndroidJniObject::callStaticMethod<jboolean>(
                JAVA_CLASS,
                "notify",
                "(Ljava/lang/CharSequence;Ljava/lang/CharSequence;I)Z",
                QAndroidJniObject::fromString(title).object<jstring>(),
                QAndroidJniObject::fromString(text).object<jstring>(),
                id);

    return ret;
}

bool Android::OpenFileDialog()
{
    ExceptionClear();

    jboolean ret = QAndroidJniObject::callStaticMethod<jboolean>(
                JAVA_CLASS,
                "openFileDialog",
                "()Z");

    return ret;
}

bool Android::ShowToast(const QString &text, const int duration)
{
    ExceptionClear();

    jboolean ret = QAndroidJniObject::callStaticMethod<jboolean>(
                JAVA_CLASS,
                "showToast",
                "(Ljava/lang/CharSequence;I)Z",
                QAndroidJniObject::fromString(text).object<jstring>(),
                duration);

    return ret;
}

void Android::MailProfile_init() {
    ExceptionClear();

    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_init",
                "()V"
                );
}

void Android::MailProfile_setHost(QString const& host, QString const& protocol)
{
    ExceptionClear();

    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_setHost",
                "(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)V",
                QAndroidJniObject::fromString(host).object<jstring>(),
                QAndroidJniObject::fromString(protocol).object<jstring>());
}

void Android::MailProfile_setPort(short port, QString const& protocol) {
    ExceptionClear();

    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_setPort",
                "(SLjava/lang/CharSequence;)V",
                port,
                QAndroidJniObject::fromString(protocol).object<jstring>()
                );
}

void Android::MailProfile_setUsername(QString const& username, QString const& protocol) {
    ExceptionClear();

    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_setUsername",
                "(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)V",
                QAndroidJniObject::fromString(username).object<jstring>(),
                QAndroidJniObject::fromString(protocol).object<jstring>()
                );
}

void Android::MailProfile_setPassword(QString const& password, QString const& protocol) {
    ExceptionClear();

    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_setPassword",
                "(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)V",
                QAndroidJniObject::fromString(password).object<jstring>(),
                QAndroidJniObject::fromString(protocol).object<jstring>()
                );
}

void Android::MailProfile_setSecurity(Mail::SecurityType securityType, QString const& protocol) {
    ExceptionClear();

    int sec;
    switch(securityType) {
        case Mail::SecurityType::None:
            sec = 0;
            break;
        case Mail::SecurityType::SSL_TLS:
            sec = 1;
            break;
        case Mail::SecurityType::STARTTLS:
            sec = 2;
            break;
        default:
            sec = -1;
    }
    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_setSecurity",
                "(ILjava/lang/CharSequence;)V",
                sec,
                QAndroidJniObject::fromString(protocol).object<jstring>()
                );
}


bool Android::MailProfile_connect(QString const& protocol) {
    ExceptionClear();

    /*QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_connect",
                "(Ljava/lang/CharSequence;)Z",
                QAndroidJniObject::fromString(protocol).object<jstring>()
                );*/
    jboolean ret = QAndroidJniObject::callStaticMethod<jboolean>(
                JAVA_CLASS,
                "mailProfile_connect",
                "(Ljava/lang/CharSequence;)Z",
                QAndroidJniObject::fromString(protocol).object<jstring>());

    return ret;
}


void Android::MailProfile_disconnect(QString const& protocol) {
    ExceptionClear();

    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_disconnect",
                "(Ljava/lang/CharSequence;)V",
                QAndroidJniObject::fromString(protocol).object<jstring>()
                );
}

void Android::MailProfile_send(QString const& jsonMessage) {
    ExceptionClear();


    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_send",
                "(Ljava/lang/CharSequence;)V",
                QAndroidJniObject::fromString(jsonMessage).object<jstring>()
                );
}

QString Android::MailProfile_fetchMessage(int from, int count) {
    ExceptionClear();

    return QAndroidJniObject::callStaticObjectMethod(
                JAVA_CLASS,
                "mailProfile_fetchMessages",
                "(II)Ljava/lang/CharSequence;",
                from,
                count
                ).toString();
}

std::size_t Android::MailProfile_getMessageCount() {
    ExceptionClear();

    return QAndroidJniObject::callStaticMethod<int>(
                JAVA_CLASS,
                "mailProfile_getMessageCount",
                "()I"
                );
}
