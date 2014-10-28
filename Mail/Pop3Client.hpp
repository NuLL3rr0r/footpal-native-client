/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 *
 */

#ifndef ERTEBAT_MAIL_POP3_CLIENT_HPP
#define ERTEBAT_MAIL_POP3_CLIENT_HPP


#include <memory>
#include <QObject>
#include "Mail.hpp"
#include "Client.hpp"
#include "Message.hpp"

class QString;

namespace Ertebat {
namespace Mail {
class Pop3Client;
class Message;
}
}

class Ertebat::Mail::Pop3Client : public Ertebat::Mail::Client
{
    Q_OBJECT

private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    Q_INVOKABLE Pop3Client();
    ~Pop3Client();

public:
    Q_INVOKABLE const Ertebat::Mail::SecurityType &GetSecurityType() const;
    Q_INVOKABLE void SetSecurityType(const Ertebat::Mail::SecurityType &securityType);

    Q_INVOKABLE const QString &GetHost() const;
    Q_INVOKABLE void SetHost(const QString &host);

    Q_INVOKABLE const Mail::Port_t &GetPort() const;
    Q_INVOKABLE void SetPort(const Mail::Port_t &port);

    Q_INVOKABLE const QString &GetUsername() const;
    Q_INVOKABLE void SetUsername(const QString &username);

    Q_INVOKABLE const QString &GetPassword() const;
    Q_INVOKABLE void SetPassword(const QString &password);


    // QML Hack
    Q_INVOKABLE void setSecurityType(const int securityType);
    Q_INVOKABLE void setPort(const int &port);

public:
    Q_INVOKABLE bool Connect();
    Q_INVOKABLE void Disconnect();

    Q_INVOKABLE std::size_t GetMessageCount();
    Q_INVOKABLE std::vector<Message> Fetch(std::size_t i, std::size_t count);
    Q_INVOKABLE bool Send(Message const& msg);

    Q_INVOKABLE virtual QString FetchAsJson(std::size_t i, std::size_t count);
};


#endif /* ERTEBAT_MAIL_POP3_CLIENT_HPP */

