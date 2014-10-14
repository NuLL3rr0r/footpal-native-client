/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 */


#ifndef ERTEBAT_MAIL_IMAP_CLIENT_HPP
#define ERTEBAT_MAIL_IMAP_CLIENT_HPP


#include <memory>
#include <QObject>
#include "Mail.hpp"

namespace Ertebat {
namespace Mail {
class ImapClient;
}
}

class Ertebat::Mail::ImapClient
{
private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;

public:
    ImapClient();
    ~ImapClient();

public:
    const Ertebat::Mail::SecurityType &GetSecurityType() const;
    void SetSecurityType(const Ertebat::Mail::SecurityType &securityType);

    const QString &GetHost() const;
    void SetHost(const QString &host);

    const Mail::Port_t &GetPort() const;
    void SetPort(const Mail::Port_t &port);

    const QString &GetUsername() const;
    void SetUsername(const QString &username);

    const QString &GetPassword() const;
    void SetPassword(const QString &password);

public:
    bool Connect();
    void Disconnect();
    bool Fetch();
};


#endif /* ERTEBAT_MAIL_IMAP_CLIENT_HPP */

