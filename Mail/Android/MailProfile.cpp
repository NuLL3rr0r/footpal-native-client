/**
 * @author  Mohamad mehdi Kharatizadeh <m_kharatizadeh@yahoo.com>
 */


#include "MailProfile.hpp"

#if defined(Q_OS_ANDROID)

#include "../Message.hpp"
#include "../Mailbox.hpp"
#include "../Mailbox.hpp"
#include "../Mail.hpp"
#include <QtAndroidExtras/QAndroidJniEnvironment>
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QtAndroidExtras>
#include "Android.hpp"
#include "Pool.hpp"
#include <QDateTime>

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "boost/foreach.hpp"

#include <sstream>

#define     MAIL_CLASS      "top/social/mailapplication/MailProfile"
#define     JAVA_CLASS      "com/ertebat/client/Android"


namespace Ertebat { namespace Mail { namespace Android {

    struct MailProfile::impl {
        QAndroidJniEnvironment AndroidJniEnvironment;
    };


    MailProfile::MailProfile()
        : m_pimpl(new MailProfile::impl)
    {

    }

    MailProfile::~MailProfile() = default;

    bool MailProfile::ExceptionCheck() {
        return m_pimpl->AndroidJniEnvironment->ExceptionCheck();
    }

    void MailProfile::ExceptionClear() {
        if (m_pimpl->AndroidJniEnvironment->ExceptionCheck()) {
            m_pimpl->AndroidJniEnvironment->ExceptionClear();
        }
    }

    void MailProfile::Initialize() {
        if (!IsInitialized()) {
            ExceptionClear();

            this->callMethod<void>(
                        "<init>", "()V");
        }
    }

    void MailProfile::Release() {
        ExceptionClear();

        QAndroidJniObject::callStaticMethod<jboolean>(
                    MAIL_CLASS, "release", "()Z");
    }

    bool MailProfile::IsInitialized() {
        ExceptionClear();

        return QAndroidJniObject::callStaticMethod<jboolean>(
                    MAIL_CLASS, "isInitialized", "()Z");
    }

    void MailProfile::init() {
        Pool::Android()->MailProfile_init();
    }

    void MailProfile::setHost(QString const& host, QString const& protocol) {
        Pool::Android()->MailProfile_SetHost(host, protocol);
    }

    void MailProfile::setSmtpHost(QString const& host) {
        Pool::Android()->MailProfile_setSmtpHost(host);
    }

    void MailProfile::setImapHost(QString const& host) {
        Pool::Android()->MailProfile_setImapHost(host);
    }

    void MailProfile::setPort(short port, QString const& protocol) {
        Pool::Android()->MailProfile_setPort(port, protocol);
    }

    void MailProfile::setSmtpPort(short port) {
        Pool::Android()->MailProfile_setSmtpPort(port);
    }

    void MailProfile::setImapPort(short port) {
        Pool::Android()->MailProfile_setImapPort(port);
    }

    void MailProfile::setUsername(QString const& username) {
        Pool::Android()->MailProfile_setUsername(username);
    }

    void MailProfile::setUsername(QString const& username, QString const& protocol) {
        Pool::Android()->MailProfile_setUsername(username, protocol);
    }

    void MailProfile::setSmtpUsername(QString const& username) {
        Pool::Android()->MailProfile_setSmtpUsername(username);
    }

    void MailProfile::setImapUsername(QString const& username) {
        Pool::Android()->MailProfile_setImapUsername(username);
    }

    void MailProfile::setPassword(QString const& password) {
        Pool::Android()->MailProfile_setPassword(password);
    }

    void MailProfile::setPassword(QString const& password, QString const& protocol) {
        Pool::Android()->MailProfile_setPassword(password, protocol);
    }

    void MailProfile::setSmtpPassword(QString const& password) {
        Pool::Android()->MailProfile_setSmtpPassword(password);
    }

    void MailProfile::setImapPassword(QString const& password) {
        Pool::Android()->MailProfile_setImapPassword(password);
    }

    void MailProfile::setSecurity(Mail::SecurityType securityType) {
        Pool::Android()->MailProfile_setSecurity(securityType);
    }

    void MailProfile::setSecurity(Mail::SecurityType securityType, QString const& protocol) {
        Pool::Android()->MailProfile_setSecurity(securityType, protocol);
    }

    void MailProfile::setSmtpSecurity(Mail::SecurityType securityType) {
        Pool::Android()->MailProfile_setSmtpSecurity(securityType);
    }

    void MailProfile::setImapSecurity(Mail::SecurityType securityType) {
        Pool::Android()->MailProfile_setImapSecurity(securityType);
    }

    void MailProfile::connect(QString const& protocol) {
        Pool::Android()->MailProfile_connect(protocol);
    }

    void MailProfile::connectSmtp() {
        Pool::Android()->MailProfile_connectSmtp();
    }

    void MailProfile::disconnect(QString const& protocol) {
        Pool::Android()->MailProfile_disconnect(protocol);
    }

    void MailProfile::disconnectSmtp() {
        Pool::Android()->MailProfile_disconnectSmtp();
    }

    void MailProfile::send(Message const& message) {
        boost::property_tree::ptree pt;
        boost::property_tree::ptree from, to, headers;
        pt.put("message_id", message.GetMessageId().toStdString());
        from.put("email", message.GetFrom().GetAddress().toStdString());
        from.put("name", message.GetFrom().GetName().toStdString());
        pt.add_child("from", from);
        pt.put("subject", message.GetSubject().toStdString());
        auto recep = message.GetRecipients();
        for(auto i = recep.begin(), _i = recep.end(); i != _i; ++i) {
            Mail::Recipient iv = *i;
            boost::property_tree::ptree p;
            p.put("email", iv.Mailbox.get().GetName().toStdString());
            p.put("name", iv.Mailbox.get().GetAddress().toStdString());
            if(iv.Type == Mail::RecipientType::To) {
                p.put("type", "TO");
            } else if(iv.Type == Mail::RecipientType::Cc) {
                p.put("type", "CC");
            } else if(iv.Type == Mail::RecipientType::Bcc) {
                p.put("type", "BCC");
            }
            to.push_back(std::make_pair("", p));
        }
        pt.add_child("to", to);
        headers.put("Reply-To", message.GetReplyTo().GetAddress().toStdString());
        pt.add_child("headers", headers);
        pt.put("text", message.GetHtmlBody().toStdString());

        std::stringstream ss;
        boost::property_tree::write_json(ss, pt);
        std::string _ss = ss.str();

        Pool::Android()->MailProfile_send(QString::fromUtf8(_ss.c_str()));
    }

    std::vector<Message> MailProfile::fetchMessage(int from, int count) {
        QString raw = Pool::Android()->MailProfile_fetchMessage(from, count);
        std::stringstream ss;
        ss << raw.toStdString();
        boost::property_tree::ptree pt;
        boost::property_tree::read_json(ss, pt);

        std::vector<Message> ret;

        BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("DATA"))
        {
            auto _message_id = v.second.get<std::string>("ORIG.message_id");
            auto _date = v.second.get<std::string>("ORIG.date");
            auto _from_email = v.second.get<std::string>("ORIG.from.email");
            auto _from_name = v.second.get<std::string>("ORIG.from.name");
            auto _subject = v.second.get<std::string>("ORIG.subject");
            auto _replyto = v.second.get<std::string>("ORIG.headers.Reply-To");
            auto _text = v.second.get<std::string>("ORIG.text");

            QString message_id = QString::fromUtf8(_message_id.c_str());
            auto v_date = QString::fromUtf8(_date.c_str()).split(":");
            QDateTime date;
            date.setDate(QDate(v_date[3].toInt(), v_date[2].toInt(), v_date[1].toInt()));
            date.setTime(QTime(v_date[4].toInt(), v_date[5].toInt(), v_date[6].toInt()));
            QString from_email = QString::fromUtf8(_from_email.c_str());
            QString from_name = QString::fromUtf8(_from_name.c_str());
            QString subject = QString::fromUtf8(_subject.c_str());
            QString text = QString::fromUtf8(_text.c_str());

            Ertebat::Mail::Message msg;
            msg.SetMessageId(message_id);
            msg.SetTime(date);
            msg.SetFrom(Mail::Mailbox(from_email, from_name));
            msg.SetSubject(subject);
            msg.SetHtmlBody(text);


            std::vector<QString> to_name,
                    to_email, cc_name, cc_email, bcc_name, bcc_email;

            BOOST_FOREACH(boost::property_tree::ptree::value_type &v2, v.second.get_child("to"))
            {
                auto _email = v2.second.get<std::string>("email");
                auto _name = v2.second.get<std::string>("name");
                auto _type = v2.second.get<std::string>("type");
                QString email = QString::fromUtf8(_email.c_str());
                QString name = QString::fromUtf8(_name.c_str());
                QString type = QString::fromUtf8(_type.c_str());
                if(type.toLower() == "to") {
                    to_email.push_back(email);
                    to_name.push_back(name);
                } else if(type.toLower() == "cc") {
                    cc_email.push_back(email);
                    cc_name.push_back(name);
                } else if(type.toLower() == "bcc") {
                    bcc_email.push_back(email);
                    bcc_name.push_back(name);
                }
            }

            for(std::size_t i=0,_i=to_name.size();i<_i;++i) {
                msg.AddRecipient(Mail::RecipientType::To, Mail::Mailbox(to_email[i], to_name[i]));
            }
            for(std::size_t i=0,_i=cc_name.size();i<_i;++i) {
                msg.AddRecipient(Mail::RecipientType::Cc, Mail::Mailbox(cc_email[i], cc_name[i]));
            }
            for(std::size_t i=0,_i=bcc_name.size();i<_i;++i) {
                msg.AddRecipient(Mail::RecipientType::Bcc, Mail::Mailbox(bcc_email[i], bcc_name[i]));
            }

            ret.push_back(msg);
        }

        return ret;
    }

    std::size_t MailProfile::getMessageCount() {
        return Pool::Android()->MailProfile_getMessageCount();
    }

} } }

#endif

