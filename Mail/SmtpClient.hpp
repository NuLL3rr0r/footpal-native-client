/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 */

#ifndef ERTEBAT_MAIL_SMTP_CLIENT_HPP
#define ERTEBAT_MAIL_SMTP_CLIENT_HPP


#include <memory>
#include <QObject>
#include "Mail.hpp"
#include "Client.hpp"
#include "Message.hpp"

class QString;

namespace Ertebat {
namespace Mail {
class Message;
class SmtpClient;
}
}

class Ertebat::Mail::SmtpClient : public Client
{
    Q_OBJECT

private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    Q_INVOKABLE SmtpClient();
    virtual ~SmtpClient();

signals:
    void signal_ConnectCompleted(bool);
    void signal_DisconnectCompleted(bool);
    void signal_SendCompleted(const QString &);

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

    // QML Hacks
    Q_INVOKABLE void setSecurityType(const int securityType);
    Q_INVOKABLE void setPort(const int &port);

public:
    Q_INVOKABLE void Connect();
    Q_INVOKABLE void Disconnect();
    Q_INVOKABLE void Send(const Message &message);

    // QML Hacks
    Q_INVOKABLE void sendAsJson(const QString &msg);

private:
    void ConnectAsync();
    void DisconnectAsync();
    void sendAsJsonAsync(const QString &msg);
};


#endif /* ERTEBAT_MAIL_SMTP_CLIENT_HPP */

