/**
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 */


#include <cassert>
#include <QString>

#if !defined ( Q_OS_ANDROID )
#   include <vmime/vmime.hpp>
#   include "BlindCertificateVerifier.hpp"
#elif defined(Q_OS_ANDROID)
#   include "Android/MailProfile.hpp"
#endif // !defined ( Q_OS_ANDROID )]

#include "make_unique.hpp"
#include "ImapClient.hpp"
#include "Log.hpp"
#include "Message.hpp"
#include "Mailbox.hpp"
#include <boost/filesystem.hpp>
#include <QDebug>

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
    vmime::shared_ptr<vmime::net::store> Store;
    vmime::shared_ptr<vmime::net::transport> Transport;
    vmime::shared_ptr<Ertebat::Mail::BlindCertificateVerifier> BlindCertificateVerifier;

#elif defined( Q_OS_ANDROID )

    Android::MailProfile profile;

#endif // !defined ( Q_OS_ANDROID )

    Impl();
};

std::size_t ImapClient::GetMessageCount() {

#if !defined(Q_OS_ANDROID)
    vmime::shared_ptr<vmime::net::folder> f = m_pimpl->Store->getDefaultFolder();
    f->open(vmime::net::folder::MODE_READ_WRITE);
    return ((std::size_t) f->getMessageCount());
#elif defined(Q_OS_ANDROID)
    return m_pimpl->profile.getMessageCount();
#endif

}


std::vector<Message> ImapClient::Fetch(std::size_t from, std::size_t count) {
    std::vector<Message> ret;

    try {

#if !defined(Q_OS_ANDROID)
        vmime::shared_ptr<vmime::net::folder> f = m_pimpl->Store->getDefaultFolder();
        f->open(vmime::net::folder::MODE_READ_WRITE);

        int c = f->getMessageCount();
        int to = std::max(0, c - ((int)(from + count)));
        for(int i = c - ((int)from); i > to; --i) {
            vmime::shared_ptr<vmime::net::message> msg = f->getMessage(i);

            f->fetchMessage(msg, vmime::net::fetchAttributes::FULL_HEADER);
            Message out;
            ExtractMessage(out, msg);
            ret.push_back(out);
        }


        return ret;
#elif defined(Q_OS_ANDROID)
        return m_pimpl->profile.fetchMessage((int) from, (int) count);
#endif
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

    return std::vector<Message>();
}

QString ImapClient::FetchAsJson(std::size_t i, std::size_t count)
{
    return Client::FetchAsJson(i, count);
}

int ImapClient::getMessageCount()
{
    return (int)GetMessageCount();
}

QString ImapClient::fetchAsJson(int i, int count)
{
    return FetchAsJson((size_t)i, (size_t)count);
}

ImapClient::ImapClient() :
    m_pimpl(std::make_unique<ImapClient::Impl>())
{
#if defined(Q_OS_ANDROID)
    m_pimpl->profile.init();
#endif
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

// QML Hack
void ImapClient::setSecurityType(const int securityType)
{
    SetSecurityType(static_cast<Mail::SecurityType>(securityType));
}

// QML Hack
void ImapClient::setPort(const int &port)
{
    SetPort(static_cast<Mail::Port_t>(port));
}

bool ImapClient::Connect()
{
    try {
#if !defined ( Q_OS_ANDROID )
        std::string protocol;
        switch (m_pimpl->SecurityType) {
        case Mail::SecurityType::SSL_TLS:
            protocol = "imaps";
            break;
        case Mail::SecurityType::None:
            // fall through
        case Mail::SecurityType::STARTTLS:
            // fall through
        default:
            protocol = "imap";
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
        m_pimpl->Transport = m_pimpl->Session->getTransport(url);
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

#elif defined(Q_OS_ANDROID)

        m_pimpl->profile.init();
        m_pimpl->profile.setHost(m_pimpl->Host, "imap");
        m_pimpl->profile.setPort(m_pimpl->Port, "imap");
        m_pimpl->profile.setUsername(m_pimpl->Username, "imap");
        m_pimpl->profile.setPassword(m_pimpl->Password, "imap");
        m_pimpl->profile.setSecurity(m_pimpl->SecurityType, "imap");
        m_pimpl->profile.connect("imap");

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


bool ImapClient::Send(const Message &message)
{
    try {
#if !defined ( Q_OS_ANDROID )
        if (m_pimpl->Transport != nullptr) {
            vmime::messageBuilder mb;

            if (!message.GetFrom().IsEmpty()) {
                mb.setExpeditor(vmime::mailbox(
                                    vmime::text(message.GetFrom().GetName().toStdString()),
                                    vmime::emailAddress(message.GetFrom().GetAddress().toStdString()))
                                );
            }

            for (const Mail::Recipient &r : message.GetRecipients()) {
                switch (r.Type) {
                case RecipientType::To:
                    mb.getRecipients().appendAddress(
                                vmime::make_shared<vmime::mailbox>(
                                    vmime::text(r.Mailbox.GetName().toStdString()),
                                    vmime::emailAddress(r.Mailbox.GetAddress().toStdString()))
                                );
                    break;
                case RecipientType::Cc:
                    mb.getCopyRecipients().appendAddress(
                                vmime::make_shared<vmime::mailbox>(
                                    vmime::text(r.Mailbox.GetName().toStdString()),
                                    vmime::emailAddress(r.Mailbox.GetAddress().toStdString()))
                                );
                    break;
                case RecipientType::Bcc:
                    mb.getBlindCopyRecipients().appendAddress(
                                vmime::make_shared<vmime::mailbox>(
                                    vmime::text(r.Mailbox.GetName().toStdString()),
                                    vmime::emailAddress(r.Mailbox.GetAddress().toStdString()))
                                );
                    break;
                default:
                    break;
                }
            }

            if (message.GetSubject() != "") {
                mb.setSubject(*vmime::text::newFromString(message.GetSubject().toStdString(),
                                                          vmime::charsets::UTF_8));
            }

            mb.constructTextPart(vmime::mediaType(vmime::mediaTypes::TEXT, vmime::mediaTypes::TEXT_HTML));
            mb.getTextPart()->setCharset(vmime::charsets::UTF_8);

            vmime::htmlTextPart &textPart = *vmime::dynamicCast <vmime::htmlTextPart>(mb.getTextPart());

            if (message.GetPlainBody() != "") {
                textPart.setPlainText(
                            vmime::make_shared<vmime::stringContentHandler>(message.GetPlainBody().toStdString()));
            }

            if (message.GetHtmlBody() != "") {
                textPart.setText(
                            vmime::make_shared<vmime::stringContentHandler>((message.GetHtmlBody().toStdString())));
            }

            if (message.GetAttachments().size() > 0) {
                for (const QString &a : message.GetAttachments()) {
                    vmime::shared_ptr<vmime::attachment> attachment = vmime::make_shared<vmime::fileAttachment>
                            (a.toStdString(), vmime::mediaType("application/octet-stream"),
                             vmime::text(boost::filesystem::path(a.toStdString()).stem().string()));
                    mb.appendAttachment(attachment);
                }
            }

            m_pimpl->Transport->send(mb.construct());

            return true;
        }
#elif defined(Q_OS_ANDROID)

        m_pimpl->profile.send(message);

#else
        (void)message;

        return true;
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

    return false;
}


void ImapClient::Disconnect()
{
    try {
#if !defined ( Q_OS_ANDROID )
        if (m_pimpl->Store != nullptr) {
            m_pimpl->Store->disconnect();
        }
#elif defined(Q_OS_ANDROID)
        m_pimpl->profile.disconnect("imap");
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

