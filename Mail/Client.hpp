/**
 * @author  M. Kharatizadeh <info@babaei.net>
 */



#ifndef ERTEBAT_MAIL_CLIENT_HPP
#define ERTEBAT_MAIL_CLIENT_HPP


#include <QObject>
#include "Mail.hpp"
#include <memory>
#include <vmime/types.hpp>

class QString;

namespace vmime { namespace net {
    class message;
} }


namespace Ertebat { namespace Mail {

    class Mailbox;
    class Message;


    struct Client {
        virtual ~Client() { }

        //virtual Mailbox& GetMailbox() = 0;
        //virtual Mailbox const& GetMailbox() const = 0;

        virtual QString const&  GetHost() const = 0;
        virtual void SetHost(QString const& host) = 0;
        virtual Mail::Port_t const& GetPort() const = 0;
        virtual void SetPort(Mail::Port_t const& port) = 0;
        virtual QString const& GetUsername() const = 0;
        virtual void SetUsername(QString const& x) = 0;
        virtual QString const& GetPassword() const = 0;
        virtual void SetPassword(QString const& x) = 0;
        virtual bool Send(const Message &message) = 0;
        virtual bool Connect() = 0;
        virtual void Disconnect() = 0;
        virtual std::size_t GetMessageCount() = 0;
        virtual std::vector<Message> Fetch(std::size_t i, std::size_t count) = 0;

    protected:

        static Message& ExtractMessage(Message& out, vmime::shared_ptr<vmime::net::message>& msg);

    };

} }


#endif // CLIENT_HPP
