#include <cassert>
#include <QString>
#if !defined ( Q_OS_ANDROID )
#include <vmime/vmime.hpp>
#endif // !defined ( Q_OS_ANDROID )
#include "make_unique.hpp"
#include "ImapClient.hpp"
#include "BlindCertificateVerifier.hpp"
#include "Log.hpp"
#include "Message.hpp"
#include "Mailbox.hpp"
#include <boost/filesystem.hpp>

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
#endif // !defined ( Q_OS_ANDROID )

    Impl();
};


std::vector<Message> ImapClient::Fetch() {
    std::vector<Message> ret;

    try {

        vmime::shared_ptr<vmime::net::folder> f = m_pimpl->Store->getDefaultFolder();
        f->open(vmime::net::folder::MODE_READ_WRITE);

        int count = f->getMessageCount();
        for(int i = count; i >= 1; --i) {
            vmime::shared_ptr<vmime::net::message> msg = f->getMessage(i);

            f->fetchMessage(msg, vmime::net::fetchAttributes::FULL_HEADER);
            Message out;
             ExtractMessage(out, msg);
        }


        return ret;
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
                                    vmime::text(r.Mailbox.get().GetName().toStdString()),
                                    vmime::emailAddress(r.Mailbox.get().GetAddress().toStdString()))
                                );
                    break;
                case RecipientType::Cc:
                    mb.getCopyRecipients().appendAddress(
                                vmime::make_shared<vmime::mailbox>(
                                    vmime::text(r.Mailbox.get().GetName().toStdString()),
                                    vmime::emailAddress(r.Mailbox.get().GetAddress().toStdString()))
                                );
                    break;
                case RecipientType::Bcc:
                    mb.getBlindCopyRecipients().appendAddress(
                                vmime::make_shared<vmime::mailbox>(
                                    vmime::text(r.Mailbox.get().GetName().toStdString()),
                                    vmime::emailAddress(r.Mailbox.get().GetAddress().toStdString()))
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

