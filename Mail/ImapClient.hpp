/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#ifndef ERTEBAT_MAIL_IMAP_CLIENT_HPP
#define ERTEBAT_MAIL_IMAP_CLIENT_HPP


#include <memory>
#include <QObject>
#include "Mail.hpp"
#include "Client.hpp"
#include "Message.hpp"

namespace Ertebat {
namespace Mail {
class ImapClient;
}
}

class Ertebat::Mail::ImapClient : public Client
{
    Q_OBJECT

private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    Q_INVOKABLE ImapClient();
    ~ImapClient();

public:
    Q_INVOKABLE const Ertebat::Mail::SecurityType &GetSecurityType() const;
    Q_INVOKABLE void SetSecurityType(const Ertebat::Mail::SecurityType &securityType);

    Q_INVOKABLE const QString &GetHost() const;
    Q_INVOKABLE void SetHost(const QString &host);

    Q_INVOKABLE const Mail::Port_t &GetPort() const;

    Q_INVOKABLE const QString &GetUsername() const;
    Q_INVOKABLE void SetUsername(const QString &username);

    Q_INVOKABLE const QString &GetPassword() const;
    Q_INVOKABLE void SetPassword(const QString &password);

    Q_INVOKABLE bool Send(const Message &message);

    Q_INVOKABLE void SetPort(Mail::Port_t const& port);


    // QML Hack
    Q_INVOKABLE void setSecurityType(const int securityType);
    Q_INVOKABLE void setPort(const int &port);

public:
    Q_INVOKABLE bool Connect();
    Q_INVOKABLE void Disconnect();
    Q_INVOKABLE std::size_t GetMessageCount();
    Q_INVOKABLE std::vector<Message> Fetch(std::size_t i, std::size_t count);

    Q_INVOKABLE QString FetchAsJson(std::size_t i, std::size_t count);
};


#endif /* ERTEBAT_MAIL_IMAP_CLIENT_HPP */

