/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#ifndef ERTEBAT_MAIL_SMTP_CLIENT_HPP
#define ERTEBAT_MAIL_SMTP_CLIENT_HPP


#include <memory>
#include <QObject>
#include "Mail.hpp"

class QString;

namespace Ertebat {
namespace Mail {
class Message;
class SmtpClient;
}
}

class Ertebat::Mail::SmtpClient
{
private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    Q_INVOKABLE SmtpClient();
    ~SmtpClient();

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

public:
    Q_INVOKABLE bool Connect();
    Q_INVOKABLE void Disconnect();
    Q_INVOKABLE bool Send(const Message &message);
};


#endif /* ERTEBAT_MAIL_SMTP_CLIENT_HPP */
