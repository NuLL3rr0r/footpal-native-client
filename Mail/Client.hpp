/**
 * @author  M. Kharatizadeh <info@babaei.net>
 */



#ifndef ERTEBAT_MAIL_CLIENT_HPP
#define ERTEBAT_MAIL_CLIENT_HPP


#include <QObject>
#include "Mail.hpp"

class QString;


namespace Ertebat { namespace Mail {

    class Mailbox;


    struct Client {
        virtual ~Client();

        virtual Mailbox& GetMailbox() = 0;
        virtual Mailbox const& GetMailbox() const;

        virtual QString const&  GetHost() const = 0;
        virtual void SetHost(QString const& host) = 0;
        virtual Mail::Port_t GetPort() const;
        virtual void SetPort(Mail::Port_t port);
        virtual QString const& GetUsername() const = 0;
        virtual void SetUsername(QString const& x);
        virtual QString const& GetPassword() const = 0;
        virtual void SetPassword(QString const& x) = 0;
        virtual void Fetch() = 0;

    };

} }


#endif // CLIENT_HPP
