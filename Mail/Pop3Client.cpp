

#include <cassert>
#include <QString>
#include "qsystemdetection.h"

#if !defined ( Q_OS_ANDROID )
#include <vmime/vmime.hpp>
#include "BlindCertificateVerifier.hpp"
#elif defined ( Q_OS_ANDROID )
#include "Android/MailProfile.hpp"
#endif // !defined ( Q_OS_ANDROID )

#include "make_unique.hpp"
#include "Pop3Client.hpp"
#include "Log.hpp"
#include "Message.hpp"
#include "Mailbox.hpp"
#include <boost/filesystem.hpp>
#include <QDebug>

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
    vmime::shared_ptr<vmime::net::transport> Transport;
#elif defined( Q_OS_ANDROID )

    Ertebat::Mail::Android::MailProfile profile;

#endif // !defined ( Q_OS_ANDROID )

    Impl();
};

Pop3Client::Pop3Client() :
    m_pimpl(std::make_unique<Pop3Client::Impl>())
{
#if defined ( Q_OS_ANDROID )
    m_pimpl->profile.init();
#endif
}

Pop3Client::~Pop3Client()
{

}

std::size_t Pop3Client::GetMessageCount() {
#if !defined(Q_OS_ANDROID)
    vmime::shared_ptr<vmime::net::folder> f = m_pimpl->Store->getDefaultFolder();
    f->open(vmime::net::folder::MODE_READ_WRITE);
    return ((std::size_t) f->getMessageCount());
#elif defined(Q_OS_ANDROID)
    return m_pimpl->profile.getMessageCount();
#endif
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

#elif defined( Q_OS_ANDROID )

        m_pimpl->profile.init();
        m_pimpl->profile.setHost(m_pimpl->Host, "pop3");
        m_pimpl->profile.setPort(m_pimpl->Port, "pop3");
        m_pimpl->profile.setUsername(m_pimpl->Username, "pop3");
        m_pimpl->profile.setPassword(m_pimpl->Password, "pop3");
        m_pimpl->profile.setSecurity(m_pimpl->SecurityType, "pop3");
        m_pimpl->profile.connect("pop3");

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
#elif defined( Q_OS_ANDROID )
        m_pimpl->profile.disconnect("pop3");
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

std::vector<Message> Pop3Client::Fetch(std::size_t from, std::size_t count) {
    std::vector<Message> ret;

    try {

#if !defined(Q_OS_ANDROID)

        vmime::shared_ptr<vmime::net::folder> f = m_pimpl->Store->getDefaultFolder();
        auto all = m_pimpl->Store->getRootFolder()->getFolders();
        for(std::size_t j = 0, _j = all.size(); j < _j; ++j) {
            //qDebug() << "+ Folder " << j << " of " << _j;
            auto folder = all[j]->getFullPath();
            //std::vector<QString> my;
            for(std::size_t k = 0, _k = folder.getSize(); k < _k; ++k) {
                auto kv = folder[k].getConvertedText("utf-8");
                QString _kv = QString::fromUtf8(kv.c_str());
                if(_kv.toLower() == "inbox") {
                    f = all[j];
                }
                //qDebug() << "- part " << _kv;
                //my.push_back(_kv);
            }
        }

//        vmime::shared_ptr<vmime::net::folder> f = m_pimpl->Store->getFolder(
//                m_pimpl->Store->getRootFolder() /
//                    vmime::net::folder::path::component("Inbox")
//                    );
        f->open(vmime::net::folder::MODE_READ_WRITE);

        int c = f->getMessageCount();
        int to = std::max(0, c - ((int)(from + count)));

        for(int i = c - ((int)from); i > to; --i) {
        //for(int i = ((int)from) + 1; i <= ((int)(from + count)); ++i) {
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

bool Pop3Client::Send(Message const& message) {
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

Pop3Client::Impl::Impl()
{
    SecurityType = Mail::SecurityType::None;
    Host = "localhost";
    Port = 110;
}

