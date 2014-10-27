/**
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 */


#include "MailProfile.hpp"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "boost/foreach.hpp"
#include <QDateTime>
#include <QStringList>
#include "../Message.hpp"
#include <QDebug>
#include "../Json.hpp"

#if defined(Q_OS_ANDROID)

#include "../Mailbox.hpp"
#include "../Mailbox.hpp"
#include "../Mail.hpp"
#include <QtAndroidExtras/QAndroidJniEnvironment>
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QtAndroidExtras>
#include "Android.hpp"
#include "Pool.hpp"


#include <sstream>

#define     MAIL_CLASS      "top/social/mailapplication/MailProfile"
#define     JAVA_CLASS      "com/ertebat/client/Android"


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
        Pool::Android()->MailProfile_init();
    }

    void MailProfile::setHost(QString const& host, QString const& protocol) {
        Pool::Android()->MailProfile_SetHost(host, protocol);
    }

    void MailProfile::setPort(short port, QString const& protocol) {
        Pool::Android()->MailProfile_setPort(port, protocol);
    }

    void MailProfile::setUsername(QString const& username, QString const& protocol) {
        Pool::Android()->MailProfile_setUsername(username, protocol);
    }

    void MailProfile::setPassword(QString const& password, QString const& protocol) {
        Pool::Android()->MailProfile_setPassword(password, protocol);
    }

    void MailProfile::setSecurity(Mail::SecurityType securityType, QString const& protocol) {
        Pool::Android()->MailProfile_setSecurity(securityType, protocol);
    }

    void MailProfile::connect(QString const& protocol) {
        Pool::Android()->MailProfile_connect(protocol);
    }

    void MailProfile::disconnect(QString const& protocol) {
        Pool::Android()->MailProfile_disconnect(protocol);
    }

    void MailProfile::send(Message const& message) {
        Pool::Android()->MailProfile_send(Ertebat::Mail::Json::EncodeMessage(message));
    }

    std::vector<Message> MailProfile::fetchMessage(int from, int count) {
        QString raw = Pool::Android()->MailProfile_fetchMessage(from, count);
        return Ertebat::Mail::Json::DecodeMessage(raw);
    }

    std::size_t MailProfile::getMessageCount() {
        return Pool::Android()->MailProfile_getMessageCount();
    }

} } }

#endif

