#include "MailProfile.hpp"
#include "../Message.hpp"
#include <QtAndroidExtras/QAndroidJniEnvironment>
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QtAndroidExtras>

#define     MAIL_CLASS      "top/social/mailapplication/MailProfile"


namespace Ertebat { namespace Mail { namespace Android {

    struct MailProfile::impl {
        QAndroidJniEnvironment AndroidJniEnvironment;
    };


    MailProfile::MailProfile()
        : m_pimpl(new MailProfile::impl)
    {

    }

    MailProfile::~MailProfile() = default;

    bool MailProfile::ExceptionCheck() {
        return m_pimpl->AndroidJniEnvironment->ExceptionCheck();
    }

    void MailProfile::ExceptionClear() {
        if (m_pimpl->AndroidJniEnvironment->ExceptionCheck()) {
            m_pimpl->AndroidJniEnvironment->ExceptionClear();
        }
    }

    void MailProfile::Initialize() {
        if (!IsInitialized()) {
            ExceptionClear();

            this->callMethod<void>(
                        "<init>", "()V");
        }
    }

    void MailProfile::Release() {
        ExceptionClear();

        QAndroidJniObject::callStaticMethod<jboolean>(
                    MAIL_CLASS, "release", "()Z");
    }

    bool MailProfile::IsInitialized() {
        ExceptionClear();

        return QAndroidJniObject::callStaticMethod<jboolean>(
                    MAIL_CLASS, "isInitialized", "()Z");
    }

    void MailProfile::init() {
        QAndroidJniObject::callStaticMethod<void>(
                    MAIL_CLASS,
                    "init",
                    "()V"
                    );
    }

    void MailProfile::setHost(QString const& host, QString const& protocol) {
        QAndroidJniObject::callStaticMethod<void>(
                    MAIL_CLASS,
                    "setHost",
                    "(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)V",
                    QAndroidJniObject::fromString(host).object<jstring>(),
                    QAndroidJniObject::fromString(protocol).object<jstring>()
                    );
    }

    void MailProfile::setSmtpHost(QString const& host) {
        QAndroidJniObject::callStaticMethod<void>(
                    MAIL_CLASS,
                    "setSmtpHost",
                    "(Ljava/lang/CharSequence;)V",
                    QAndroidJniObject::fromString(host).object<jstring>()
                    );
    }

    void MailProfile::setImapHost(QString const& host) {
        QAndroidJniObject::callStaticMethod<void>(
                    MAIL_CLASS,
                    "setImapHost",
                    "(Ljava/lang/CharSequence;)V",
                    QAndroidJniObject::fromString(host).object<jstring>()
                    );
    }

    void MailProfile::setPort(short port, QString const& protocol) {
        QAndroidJniObject::callStaticMethod<void>(
                    MAIL_CLASS,
                    "setPort",
                    "(SLjava/lang/CharSequence;)V",
                    port,
                    QAndroidJniObject::fromString(protocol).object<jstring>()
                    );
    }

    void MailProfile::setSmtpPort(short port) {
        QAndroidJniObject::callStaticMethod<void>(
                    MAIL_CLASS,
                    "setSmtpPort",
                    "(S)V",
                    port
                    );
    }

    void MailProfile::setImapPort(short port) {
        QAndroidJniObject::callStaticMethod<void>(
                    MAIL_CLASS,
                    "setImapPort",
                    "(S)V",
                    port
                    );
    }

    void MailProfile::setUsername(QString const& username) {
        QAndroidJniObject::callStaticMethod<void>(
                    MAIL_CLASS,
                    "setUsername",
                    "(Ljava/lang/CharSequence;)V",
                    QAndroidJniObject::fromString(username).object<jstring>()
                    );
    }

    void MailProfile::setUsername(QString const& username, QString const& protocol) {
        QAndroidJniObject::callStaticMethod<void>(
                    MAIL_CLASS,
                    "setUsername",
                    "(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)V",
                    QAndroidJniObject::fromString(username).object<jstring>(),
                    QAndroidJniObject::fromString(protocol).object<jstring>()
                    );
    }

    void MailProfile::setSmtpUsername(QString const& username) {
        QAndroidJniObject::callStaticMethod<void>(
                    MAIL_CLASS,
                    "setSmtpUsername",
                    "(Ljava/lang/CharSequence;)V",
                    QAndroidJniObject::fromString(username).object<jstring>()
                    );
    }

    void MailProfile::setImapUsername(QString const& username) {
        QAndroidJniObject::callStaticMethod<void>(
                    MAIL_CLASS,
                    "setImapUsername",
                    "(Ljava/lang/CharSequence;)V",
                    QAndroidJniObject::fromString(username).object<jstring>()
                    );
    }

    void MailProfile::setPassword(QString const& password) {
        QAndroidJniObject::callStaticMethod<void>(
                    MAIL_CLASS,
                    "setPassword",
                    "(Ljava/lang/CharSequence;)V",
                    QAndroidJniObject::fromString(password).object<jstring>()
                    );
    }

    void MailProfile::setPassword(QString const& password, QString const& protocol) {
        QAndroidJniObject::callStaticMethod<void>(
                    MAIL_CLASS,
                    "setPassword",
                    "(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)V",
                    QAndroidJniObject::fromString(password).object<jstring>(),
                    QAndroidJniObject::fromString(protocol).object<jstring>()
                    );
    }

    void MailProfile::setSmtpPassword(QString const& password) {
        QAndroidJniObject::callStaticMethod<void>(
                    MAIL_CLASS,
                    "setSmtpPassword",
                    "(Ljava/lang/CharSequence;)V",
                    QAndroidJniObject::fromString(password).object<jstring>()
                    );
    }

    void MailProfile::setImapPassword(QString const& password) {
        QAndroidJniObject::callStaticMethod<void>(
                    MAIL_CLASS,
                    "setImapPassword",
                    "(Ljava/lang/CharSequence;)V",
                    QAndroidJniObject::fromString(password).object<jstring>()
                    );
    }

    void MailProfile::setSecurity(Mail::SecurityType securityType) {
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
                    MAIL_CLASS,
                    "setSecurity",
                    "(I)V",
                    sec
                    );
    }

    void MailProfile::setSecurity(Mail::SecurityType securityType, QString const& protocol) {
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
                    MAIL_CLASS,
                    "setSecurity",
                    "(ILjava/lang/CharSequence)V",
                    sec,
                    QAndroidJniObject::fromString(protocol).object<jstring>()
                    );
    }

    void MailProfile::setSmtpSecurity(Mail::SecurityType securityType) {
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
                    MAIL_CLASS,
                    "setSmtpSecurity",
                    "(I)V",
                    sec
                    );
    }

    void MailProfile::setImapSecurity(Mail::SecurityType securityType) {
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
                    MAIL_CLASS,
                    "setImapSecurity",
                    "(I)V",
                    sec
                    );
    }

    void MailProfile::connect(QString const& protocol) {
        QAndroidJniObject::callStaticMethod<void>(
                    MAIL_CLASS,
                    "connect",
                    "(Ljava/lang/CharSequence;)V",
                    QAndroidJniObject::fromString(protocol).object<jstring>()
                    );
    }

    void MailProfile::connectSmtp() {
        QAndroidJniObject::callStaticMethod<void>(
                    MAIL_CLASS,
                    "connectSmtp",
                    "()V"
                    );
    }

    void MailProfile::disconnect(QString const& protocol) {
        QAndroidJniObject::callStaticMethod<void>(
                    MAIL_CLASS,
                    "disconnect",
                    "(Ljava/lang/CharSequence;)V",
                    QAndroidJniObject::fromString(protocol).object<jstring>()
                    );
    }

    void MailProfile::disconnectSmtp() {
        QAndroidJniObject::callStaticMethod<void>(
                    MAIL_CLASS,
                    "disconnectSmtp",
                    "()V"
                    );
    }

    void MailProfile::send(Message const& message) {

        QString jsonMessage;
        QAndroidJniObject::callStaticMethod<void>(
                    MAIL_CLASS,
                    "send",
                    "(Ljava/lang/CharSequence;)V",
                    QAndroidJniObject::fromString(jsonMessage).object<jstring>()
                    );
    }

    std::vector<Message> MailProfile::fetchMessage(int from, int count) {
        jobjectArray  ar = QAndroidJniObject::callStaticObjectMethod(
                    MAIL_CLASS,
                    "fetchMessage",
                    "(II)[Ljava/lang/CharSequence;",
                    from,
                    count
                    ).object<jobjectArray>();
       jsize len = m_pimpl->AndroidJniEnvironment->GetArrayLength(ar);

    }

    std::size_t MailProfile::getMessageCount() {
        return 0;
    }

} } }



