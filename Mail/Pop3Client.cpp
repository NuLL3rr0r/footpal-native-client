#include "qsystemdetection.h"

#if !defined(Q_OS_ANDROID)

#include <cassert>
#include <QString>
#if !defined ( Q_OS_ANDROID )
#include <vmime/vmime.hpp>
#endif // !defined ( Q_OS_ANDROID )
#include "make_unique.hpp"
#include "Pop3Client.hpp"
#include "BlindCertificateVerifier.hpp"
#include "Log.hpp"

#define         UNKNOWN_ERROR               "  ** Pop3Client ->  Unknown Error!"

using namespace Ertebat;
using namespace Ertebat::Mail;

struct Pop3Client::Impl
{
    Mail::SecurityType SecurityType;
    QString Host;
    Mail::Port_t Port;
    QString Username;
    QString Password;

#if !defined ( Q_OS_ANDROID )
    vmime::shared_ptr<vmime::net::session> Session;
    vmime::shared_ptr<vmime::net::store> Store;
    vmime::shared_ptr<Ertebat::Mail::BlindCertificateVerifier> BlindCertificateVerifier;
#endif // !defined ( Q_OS_ANDROID )

    Impl();
};

Pop3Client::Pop3Client() :
    m_pimpl(std::make_unique<Pop3Client::Impl>())
{

}

Pop3Client::~Pop3Client()
{

}

std::size_t Pop3Client::GetMessageCount() {
    return 0;
}

const Ertebat::Mail::SecurityType &Pop3Client::GetSecurityType() const
{
    return m_pimpl->SecurityType;
}

void Pop3Client::SetSecurityType(const Ertebat::Mail::SecurityType &securityType)
{
    m_pimpl->SecurityType = securityType;
}

const QString &Pop3Client::GetHost() const
{
    return m_pimpl->Host;
}

void Pop3Client::SetHost(const QString &host)
{
    m_pimpl->Host = host;
}

const Mail::Port_t &Pop3Client::GetPort() const
{
    return m_pimpl->Port;
}

void Pop3Client::SetPort(const Mail::Port_t &port)
{
    m_pimpl->Port = port;
}

const QString &Pop3Client::GetUsername() const
{
    return m_pimpl->Username;
}

void Pop3Client::SetUsername(const QString &username)
{
    m_pimpl->Username = username;
}

const QString &Pop3Client::GetPassword() const
{
    return m_pimpl->Password;
}

void Pop3Client::SetPassword(const QString &password)
{
    m_pimpl->Password = password;
}

bool Pop3Client::Connect()
{
    try {
#if !defined ( Q_OS_ANDROID )
        std::string protocol;
        switch (m_pimpl->SecurityType) {
        case Mail::SecurityType::SSL_TLS:
            protocol = "pop3s";
            break;
        case Mail::SecurityType::None:
            // fall through
        case Mail::SecurityType::STARTTLS:
            // fall through
        default:
            protocol = "pop3";
            break;
        }

        vmime::utility::url url(protocol,
                                m_pimpl->Host.toStdString(),
                                static_cast<vmime::port_t>(m_pimpl->Port),
                                std::string(""),
                                std::string(""),
                                std::string("")
                                );
        m_pimpl->Session = vmime::make_shared<vmime::net::session>();
        m_pimpl->Store = m_pimpl->Session->getStore(url);
        m_pimpl->BlindCertificateVerifier = vmime::make_shared<Ertebat::Mail::BlindCertificateVerifier>();

        if (m_pimpl->SecurityType == Mail::SecurityType::STARTTLS) {
            m_pimpl->Store->setProperty("connection.tls", true);
        }

        if (m_pimpl->Password != "") {
            m_pimpl->Store->setProperty("auth.username", m_pimpl->Username.toStdString().c_str());
            m_pimpl->Store->setProperty("auth.password", m_pimpl->Password.toStdString().c_str());
            m_pimpl->Store->setProperty("options.need-authentication", true);
        }

        if (m_pimpl->SecurityType != Mail::SecurityType::None) {
             m_pimpl->Store->setCertificateVerifier(m_pimpl->BlindCertificateVerifier);
        }

        m_pimpl->Store->connect();
#endif // !defined ( Q_OS_ANDROID )

        return true;
    }
#if !defined ( Q_OS_ANDROID )
    catch (vmime::exception &ex) {
        LOG_ERROR(ex.what());
    }
#endif // !defined ( Q_OS_ANDROID )
    catch (std::exception &ex) {
        LOG_ERROR(ex.what());
    } catch(...) {
        LOG_ERROR(UNKNOWN_ERROR);
    }

    return false;
}

void Pop3Client::Disconnect()
{
    try {
#if !defined ( Q_OS_ANDROID )
        if (m_pimpl->Store != nullptr) {
            m_pimpl->Store->disconnect();
        }
#endif // !defined ( Q_OS_ANDROID )
    }
#if !defined ( Q_OS_ANDROID )
    catch (vmime::exception &ex) {
        LOG_ERROR(ex.what());
    }
#endif // !defined ( Q_OS_ANDROID )
    catch (std::exception &ex) {
        LOG_ERROR(ex.what());
    } catch(...) {
        LOG_ERROR(UNKNOWN_ERROR);
    }
}

Pop3Client::Impl::Impl()
{
    SecurityType = Mail::SecurityType::None;
    Host = "localhost";
    Port = 110;
}

#endif
