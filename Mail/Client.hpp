/**
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 */


#ifndef ERTEBAT_MAIL_CLIENT_HPP
#define ERTEBAT_MAIL_CLIENT_HPP


#include <QObject>
#include "Mail.hpp"
#include "Message.hpp"
#include <memory>

class QString;

#undef SetPort

namespace vmime { namespace net {
    class message;
} }


namespace Ertebat { namespace Mail {

    class Mailbox;
    class Message;


    class Client : public QObject {

    public:

        virtual ~Client() { }

        //virtual Mailbox& GetMailbox() = 0;
        //virtual Mailbox const& GetMailbox() const = 0;

        virtual Q_INVOKABLE QString const&  GetHost() const;
        virtual Q_INVOKABLE void SetHost(QString const& host);
        virtual Q_INVOKABLE Mail::Port_t const& GetPort() const;
        virtual Q_INVOKABLE void SetPort(Mail::Port_t const& port);
        virtual Q_INVOKABLE QString const& GetUsername() const;
        virtual Q_INVOKABLE void SetUsername(QString const& x);
        virtual Q_INVOKABLE QString const& GetPassword() const;
        virtual Q_INVOKABLE void SetPassword(QString const& x);
        virtual Q_INVOKABLE void Connect();
        virtual Q_INVOKABLE void Disconnect();
        virtual Q_INVOKABLE std::size_t GetMessageCount();
        virtual Q_INVOKABLE std::vector<Message> Fetch(std::size_t i, std::size_t count);
        virtual Q_INVOKABLE QString FetchAsJson(std::size_t i, std::size_t count);

        // QML Hacks
        virtual Q_INVOKABLE void getMessageCount();
        virtual Q_INVOKABLE void fetchAsJson(int i, int count);

    protected:

#if !defined(Q_OS_ANDROID)
        static Message& ExtractMessage(Message& out, std::shared_ptr<vmime::net::message>& msg);
#endif

    };

} }


#endif // CLIENT_HPP
