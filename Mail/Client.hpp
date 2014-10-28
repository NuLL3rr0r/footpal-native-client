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

        virtual QString const&  GetHost() const;
        virtual void SetHost(QString const& host);
        virtual Mail::Port_t const& GetPort() const;
        virtual void SetPort(Mail::Port_t const& port);
        virtual QString const& GetUsername() const;
        virtual void SetUsername(QString const& x);
        virtual QString const& GetPassword() const;
        virtual void SetPassword(QString const& x);
        virtual bool Send(const Message &message);
        virtual bool Connect();
        virtual void Disconnect();
        virtual std::size_t GetMessageCount();
        virtual std::vector<Message> Fetch(std::size_t i, std::size_t count);
        virtual QString FetchAsJson(std::size_t i, std::size_t count);

        virtual int getMessageCount();

    protected:

#if !defined(Q_OS_ANDROID)
        static Message& ExtractMessage(Message& out, std::shared_ptr<vmime::net::message>& msg);
#endif

    };

} }


#endif // CLIENT_HPP
