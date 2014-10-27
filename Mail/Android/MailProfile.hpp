/**
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 */


#ifndef ERTEBAT_MAIL_ANDROID_MAILPROFILE_HPP
#define ERTEBAT_MAIL_ANDROID_MAILPROFILE_HPP

namespace Ertebat { namespace Mail {

    class Message;

} }

#include <qsystemdetection.h>
#if defined( Q_OS_ANDROID )

#include <QObject>
#include <jni.h>
#include <QtAndroidExtras/QAndroidJniObject>
#include <memory>
#include "Mail/Mail.hpp"

namespace Ertebat { namespace Mail { namespace Android {

    class MailProfile : public QAndroidJniObject {

        struct impl;
        std::shared_ptr<impl> m_pimpl;

    public:

        static std::vector<Message> ParseJson(QString const& s);

        MailProfile();
        ~MailProfile();

        bool ExceptionCheck();
        void ExceptionClear();
        void Initialize();
        void Release();
        bool IsInitialized();

        void init();
        void setHost(QString const& host, QString const& protocol);
        void setPort(short port, QString const& protocol);
        void setUsername(QString const& username, QString const& protcol);
        void setPassword(QString const& password, QString const& protocol);
        void setSecurity(Mail::SecurityType securityType, QString const& protocol);
        void connect(QString const& protocol);
        void disconnect(QString const& protocol);
        void send(Message const& jsonMessage);
        std::vector<Message> fetchMessage(int from, int count);
        std::size_t getMessageCount();
    };

} } }

#else

namespace Ertebat { namespace Mail { namespace Android {

    class MailProfile {

    public:

        static std::vector<Message> ParseJson(QString const& s);

    };

} } }

#endif

#endif // MAILPROFILE_H
