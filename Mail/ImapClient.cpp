#include <cassert>
#include <QString>
#if !defined ( Q_OS_ANDROID )
#if !defined ( _WIN32 )
#include <vmime/platforms/posix/posixHandler.hpp>
#else
#include <vmime/platforms/windows/windowsHandler.hpp>
#endif // !defined (_WIN32 )
#include <vmime/vmime.hpp>
#endif // !defined ( Q_OS_ANDROID )
#include "make_unique.hpp"
#include "ImapClient.hpp"
#include "Log.hpp"

#define         UNKNOWN_ERROR               "  ** ImapClient ->  Unknown Error!"

using namespace Ertebat;
using namespace Ertebat::Mail;

struct ImapClient::Impl
{
    Mail::SecurityType SecurityType;
    QString Host;
    Mail::Port_t Port;
    QString Username;
    QString Password;

#if !defined ( Q_OS_ANDROID )
    vmime::shared_ptr<vmime::net::session> Session;
    vmime::shared_ptr<vmime::net::transport> Transport;
#endif // !defined ( Q_OS_ANDROID )

    Impl();
};

ImapClient::ImapClient() :
    m_pimpl(std::make_unique<ImapClient::Impl>())
{

}

ImapClient::~ImapClient()
{

}

const Ertebat::Mail::SecurityType &ImapClient::GetSecurityType() const
{
    return m_pimpl->SecurityType;
}

void ImapClient::SetSecurityType(const Ertebat::Mail::SecurityType &securityType)
{
    m_pimpl->SecurityType = securityType;
}

const QString &ImapClient::GetHost() const
{
    return m_pimpl->Host;
}

void ImapClient::SetHost(const QString &host)
{
    m_pimpl->Host = host;
}

const Mail::Port_t &ImapClient::GetPort() const
{
    return m_pimpl->Port;
}

void ImapClient::SetPort(const Mail::Port_t &port)
{
    m_pimpl->Port = port;
}

const QString &ImapClient::GetUsername() const
{
    return m_pimpl->Username;
}

void ImapClient::SetUsername(const QString &username)
{
    m_pimpl->Username = username;
}

const QString &ImapClient::GetPassword() const
{
    return m_pimpl->Password;
}

void ImapClient::SetPassword(const QString &password)
{
    m_pimpl->Password = password;
}

bool ImapClient::Connect()
{
    try {
#if !defined ( Q_OS_ANDROID )
        vmime::utility::url url(std::string(""),
                                m_pimpl->Host.toStdString(),
                                static_cast<vmime::port_t>(m_pimpl->Port),
                                std::string(""),
                                std::string(""),
                                std::string("")
                                );
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

void ImapClient::Disconnect()
{
    try {
#if !defined ( Q_OS_ANDROID )
        if (m_pimpl->Transport != nullptr) {
            m_pimpl->Transport->disconnect();
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

ImapClient::Impl::Impl()
{
    SecurityType = Mail::SecurityType::None;
    Host = "localhost";
    Port = 143;
}

