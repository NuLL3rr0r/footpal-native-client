/**
 * @author  Mohammad S. Babaei <info@babaei.net>
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

//static JNINativeMethod s_nativeMethods[] = {
//};

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

    //    if (env->RegisterNatives(
    //                clazz,
    //                s_nativeMethods,
    //                sizeof(s_nativeMethods) / sizeof(s_nativeMethods[0])
    //                ) < 0) {
    //        qCritical() << "   * Failed to Register native methods !!";
    //        return -1;
    //    }

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

        this->callMethod<void>(
                    "<init>", "()V");
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

void Android::MailProfile_SetHost(QString const& host, QString const& protocol)
{
    ExceptionClear();
    qDebug() << __FUNCTION__;

    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_setHost",
                "(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)V",
                QAndroidJniObject::fromString(host).object<jstring>(),
                QAndroidJniObject::fromString(protocol).object<jstring>());
}

void Android::MailProfile_init() {
    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_init",
                "()V"
                );
}

void Android::MailProfile_setSmtpHost(QString const& host) {
    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_setSmtpHost",
                "(Ljava/lang/CharSequence;)V",
                QAndroidJniObject::fromString(host).object<jstring>()
                );
}

void Android::MailProfile_setImapHost(QString const& host) {
    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_setImapHost",
                "(Ljava/lang/CharSequence;)V",
                QAndroidJniObject::fromString(host).object<jstring>()
                );
}

void Android::MailProfile_setPort(short port, QString const& protocol) {
    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_setPort",
                "(SLjava/lang/CharSequence;)V",
                port,
                QAndroidJniObject::fromString(protocol).object<jstring>()
                );
}

void Android::MailProfile_setSmtpPort(short port) {
    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_setSmtpPort",
                "(S)V",
                port
                );
}

void Android::MailProfile_setImapPort(short port) {
    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_setImapPort",
                "(S)V",
                port
                );
}

void Android::MailProfile_setUsername(QString const& username) {
    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_setUsername",
                "(Ljava/lang/CharSequence;)V",
                QAndroidJniObject::fromString(username).object<jstring>()
                );
}

void Android::MailProfile_setUsername(QString const& username, QString const& protocol) {
    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_setUsername",
                "(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)V",
                QAndroidJniObject::fromString(username).object<jstring>(),
                QAndroidJniObject::fromString(protocol).object<jstring>()
                );
}

void Android::MailProfile_setSmtpUsername(QString const& username) {
    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_setSmtpUsername",
                "(Ljava/lang/CharSequence;)V",
                QAndroidJniObject::fromString(username).object<jstring>()
                );
}

void Android::MailProfile_setImapUsername(QString const& username) {
    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_setImapUsername",
                "(Ljava/lang/CharSequence;)V",
                QAndroidJniObject::fromString(username).object<jstring>()
                );
}

void Android::MailProfile_setPassword(QString const& password) {
    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_setPassword",
                "(Ljava/lang/CharSequence;)V",
                QAndroidJniObject::fromString(password).object<jstring>()
                );
}

void Android::MailProfile_setPassword(QString const& password, QString const& protocol) {
    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_setPassword",
                "(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)V",
                QAndroidJniObject::fromString(password).object<jstring>(),
                QAndroidJniObject::fromString(protocol).object<jstring>()
                );
}

void Android::MailProfile_setSmtpPassword(QString const& password) {
    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_setSmtpPassword",
                "(Ljava/lang/CharSequence;)V",
                QAndroidJniObject::fromString(password).object<jstring>()
                );
}

void Android::MailProfile_setImapPassword(QString const& password) {
    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_setImapPassword",
                "(Ljava/lang/CharSequence;)V",
                QAndroidJniObject::fromString(password).object<jstring>()
                );
}

void Android::MailProfile_setSecurity(Mail::SecurityType securityType) {
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
                "(I)V",
                sec
                );
}

void Android::MailProfile_setSecurity(Mail::SecurityType securityType, QString const& protocol) {
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
                "(ILjava/lang/CharSequence)V",
                sec,
                QAndroidJniObject::fromString(protocol).object<jstring>()
                );
}

void Android::MailProfile_setSmtpSecurity(Mail::SecurityType securityType) {
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
                "mailProfile_setSmtpSecurity",
                "(I)V",
                sec
                );
}

void Android::MailProfile_setImapSecurity(Mail::SecurityType securityType) {
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
                "mailProfile_setImapSecurity",
                "(I)V",
                sec
                );
}

void Android::MailProfile_connect(QString const& protocol) {
    qDebug() << "MAILPROFILE CONNECCTTTTTTT";
    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_connect",
                "(Ljava/lang/CharSequence;)V",
                QAndroidJniObject::fromString(protocol).object<jstring>()
                );
}

void Android::MailProfile_connectSmtp() {
    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_connectSmtp",
                "()V"
                );
}

void Android::MailProfile_disconnect(QString const& protocol) {
    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_disconnect",
                "(Ljava/lang/CharSequence;)V",
                QAndroidJniObject::fromString(protocol).object<jstring>()
                );
}

void Android::MailProfile_disconnectSmtp() {
    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "mailProfile_disconnectSmtp",
                "()V"
                );
}

void Android::MailProfile_send(QString const& jsonMessage) {

    QAndroidJniObject::callStaticMethod<void>(
                JAVA_CLASS,
                "send",
                "(Ljava/lang/CharSequence;)V",
                QAndroidJniObject::fromString(jsonMessage).object<jstring>()
                );
}

QString Android::MailProfile_fetchMessage(int from, int count) {

    qDebug() << "T1";

    jstring  ar = QAndroidJniObject::callStaticObjectMethod(
                JAVA_CLASS,
                "mailProfile_fetchMessages",
                "(II)Ljava/lang/CharSequence;",
                from,
                count
                ).object<jstring>();


   qDebug() << "T1.5";

   const char* str = m_pimpl->AndroidJniEnvironment->GetStringUTFChars(ar, 0);
   qDebug() << "T2";
   QString s = QString::fromUtf8(str);
   qDebug() << "T3";
   m_pimpl->AndroidJniEnvironment->ReleaseStringUTFChars(ar, str);
   qDebug() << "T4";

   qDebug() << s;

   qDebug() << "T5";

   return s;
}

std::size_t Android::MailProfile_getMessageCount() {
    //mailProfile_getMessageCount
    return QAndroidJniObject::callStaticMethod<int>(
                JAVA_CLASS,
                "mailProfile_getMessageCount",
                "()I"
                );
}
