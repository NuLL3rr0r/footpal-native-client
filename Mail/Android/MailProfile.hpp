/**
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 */


#ifndef ERTEBAT_MAIL_ANDROID_MAILPROFILE_HPP
#define ERTEBAT_MAIL_ANDROID_MAILPROFILE_HPP

#include <qsystemdetection.h>
#if defined(Q_OS_ANDROID)

#include <QObject>
#include <jni.h>
#include <QtAndroidExtras/QAndroidJniObject>
#include <memory>
#include "Mail/Mail.hpp"

namespace Ertebat { namespace Mail {

    class Message;

} }

namespace Ertebat { namespace Mail { namespace Android {

    class MailProfile : public QAndroidJniObject {

        struct impl;
        std::shared_ptr<impl> m_pimpl;

    public:

        MailProfile();
        ~MailProfile();

        bool ExceptionCheck();
        void ExceptionClear();
        void Initialize();
        void Release();
        bool IsInitialized();

        void init();
        void setHost(QString const& host, QString const& protocol);
        void setSmtpHost(QString const& host);
        void setImapHost(QString const& host);
        void setPort(short port, QString const& protocol);
        void setSmtpPort(short port);
        void setImapPort(short port);
        void setUsername(QString const& username);
        void setUsername(QString const& username, QString const& protcol);
        void setSmtpUsername(QString const& username);
        void setImapUsername(QString const& username);
        void setPassword(QString const& password);
        void setPassword(QString const& password, QString const& protocol);
        void setSmtpPassword(QString const& password);
        void setImapPassword(QString const& password);
        void setSecurity(Mail::SecurityType securityType);
        void setSecurity(Mail::SecurityType securityType, QString const& protocol);
        void setSmtpSecurity(Mail::SecurityType securityType);
        void setImapSecurity(Mail::SecurityType securityType);
        void connect(QString const& protocol);
        void connectSmtp();
        void disconnect(QString const& protocol);
        void disconnectSmtp();
        void send(Message const& jsonMessage);
        std::vector<Message> fetchMessage(int from, int count);
        std::size_t getMessageCount();
    };

} } }

#endif

#endif // MAILPROFILE_H
