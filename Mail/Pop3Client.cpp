/**
 * @author  Mohammad S. Babaei <info@babaei.net>
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 */


#include <cassert>
#include <QString>
#if !defined ( Q_OS_ANDROID )
#include <vmime/vmime.hpp>
#include "BlindCertificateVerifier.hpp"
#elif defined ( Q_OS_ANDROID )
#include "Android.hpp"
#endif // !defined ( Q_OS_ANDROID )
#include "make_unique.hpp"
#include "Pop3Client.hpp"
#include "Log.hpp"
#include "Message.hpp"
#include "Mailbox.hpp"
#include "../Pool.hpp"
#include "Json.hpp"
#include <boost/thread.hpp>
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

void Pop3Client::GetMessageCountAsync() {
#if !defined(Q_OS_ANDROID)
    vmime::shared_ptr<vmime::net::folder> f = m_pimpl->Store->getDefaultFolder();
    f->open(vmime::net::folder::MODE_READ_WRITE);
    emit signal_GetMessageCountCompleted((int) f->getMessageCount());
#elif defined(Q_OS_ANDROID)
    emit signal_GetMessageCountCompleted((int)Pool::Android()->MailProfile_getMessageCount());
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

// QML Hack
void Pop3Client::setSecurityType(const int securityType)
{
    SetSecurityType(static_cast<Mail::SecurityType>(securityType));
}

// QML Hack
void Pop3Client::setPort(const int &port)
{
    SetPort(static_cast<Mail::Port_t>(port));
}

void Pop3Client::ConnectAsync()
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

        ////////////

        qDebug("00000000000000");
        Pool::Android()->MailProfile_init();
        qDebug("a0000000000000");
        Pool::Android()->MailProfile_init();
        qDebug("b00000000000000");
        Pool::Android()->MailProfile_setHost(m_pimpl->Host, "pop3");
        qDebug("c00000000000000");
        Pool::Android()->MailProfile_setPort(m_pimpl->Port, "pop3");
        qDebug("d00000000000000");
        Pool::Android()->MailProfile_setUsername(m_pimpl->Username, "pop3");
        qDebug("e00000000000000");
        Pool::Android()->MailProfile_setPassword(m_pimpl->Password, "pop3");
        qDebug("f0000000000000");
        Pool::Android()->MailProfile_setSecurity(m_pimpl->SecurityType, "pop3");
        qDebug("g0000000000000");
        qDebug() << Pool::Android()->MailProfile_connect("pop3");
        qDebug("1111111111");

//        m_pimpl->profile.init();
//        m_pimpl->profile.setHost(m_pimpl->Host, "pop3");
//        m_pimpl->profile.setPort(m_pimpl->Port, "pop3");
//        m_pimpl->profile.setUsername(m_pimpl->Username, "pop3");
//        m_pimpl->profile.setPassword(m_pimpl->Password, "pop3");
//        m_pimpl->profile.setSecurity(m_pimpl->SecurityType, "pop3");
//        m_pimpl->profile.connect("pop3");

#endif // !defined ( Q_OS_ANDROID )

        emit signal_ConnectCompleted(true);
        ///return true;
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

    ///return false;
}

void Pop3Client::DisconnectAsync()
{
    try {
#if !defined ( Q_OS_ANDROID )
        if (m_pimpl->Store != nullptr) {
            m_pimpl->Store->disconnect();
            emit signal_DisconnectCompleted();
        }
#elif defined( Q_OS_ANDROID )
        Pool::Android()->MailProfile_disconnect("pop3");
        emit signal_DisconnectCompleted();
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

void Pop3Client::FetchAsync(std::size_t from, std::size_t count) {
    QVector<Message> ret;

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

        emit signal_FetchCompleted(ret);
#elif defined(Q_OS_ANDROID)
        auto v = Json::DecodeMessage(Pool::Android()->MailProfile_fetchMessage(from, count));
        for(auto i = v.begin(), _i = v.end(); i != _i; ++i) {
            ret.push_back(*i);
        }
        emit signal_FetchCompleted(ret);
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
}

void Pop3Client::FetchAsJsonAsync(std::size_t from, std::size_t count)
{
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

        onFetchJsonMessage(Json::EncodeMessage(ret));
#elif defined(Q_OS_ANDROID)
        onFetchJsonMessage(Pool::Android()->MailProfile_fetchMessage(from, count));
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
}

void Pop3Client::getMessageCount()
{
    GetMessageCount();
}

void Pop3Client::fetchAsJson(int i, int count)
{
    FetchAsJson((std::size_t) i, (std::size_t) count);
}

void Pop3Client::Connect() {
    boost::thread t(static_cast<void (Pop3Client::*)()>(&Pop3Client::ConnectAsync), this);
    t.detach();
}

void Pop3Client::Disconnect() {
    boost::thread t(static_cast<void (Pop3Client::*)()>(&Pop3Client::DisconnectAsync), this);
    t.detach();
}

void Pop3Client::GetMessageCount() {
    boost::thread t(static_cast<void (Pop3Client::*)()>(&Pop3Client::GetMessageCountAsync), this);
    t.detach();
}

void Pop3Client::Fetch(std::size_t i, std::size_t count) {
    boost::thread t(static_cast<void (Pop3Client::*)(std::size_t, std::size_t)>(&Pop3Client::FetchAsync), this, i, count);
    t.detach();
}

void Pop3Client::FetchAsJson(std::size_t i, std::size_t count) {
    boost::thread t(static_cast<void (Pop3Client::*)(std::size_t, std::size_t)>(&Pop3Client::FetchAsJsonAsync), this, i, count);
    t.detach();
}

Pop3Client::Impl::Impl()
{
    SecurityType = Mail::SecurityType::None;
    Host = "localhost";
    Port = 110;
}

